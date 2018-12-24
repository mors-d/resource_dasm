#include "dc_decode_sprite.hh"

#include <phosg/Encoding.hh>
#include <stdexcept>

using namespace std;



struct InputFormat {
  int16_t height;
  int16_t width;
  uint8_t bits_per_pixel; // actually bits per pixel - 1, but whatever
  uint8_t unknown[2];
  uint8_t generate_transparency_map;
  uint8_t data[0];
};

void generate_transparency_map(size_t count, void* data) {
  uint32_t* src_ptr = reinterpret_cast<uint32_t*>(data) + (count / 4);
  uint32_t* dst_ptr = src_ptr + (count / 4);
  while (src_ptr != data) {
    uint32_t values = bswap32(*(src_ptr--));
    uint32_t t_values = 0;
    if ((values & 0xFF) == 0) {
      t_values = 0x000000FF;
    }
    if (((values >> 8) & 0xFF) == 0) {
      t_values |= 0x0000FF00;
    }
    if (((values >> 16) & 0xFF) == 0) {
      t_values |= 0x00FF0000;
    }
    if (((values >> 24) & 0xFF) == 0) {
      t_values |= 0xFF000000;
    }

    *(dst_ptr--) = bswap32(t_values);
    *(dst_ptr--) = bswap32(values);
  }
}

bool get_bit_at_offset(const void* data, size_t bit_offset) {
  // note: the original implementation is ((offset >> 4) << 1); this should be
  // equivalent. the right shift is signed, but args to this function always
  // appear to be positive so it shouldn't matter
  const uint8_t* u8_data = reinterpret_cast<const uint8_t*>(data);
  size_t byte_offset = (bit_offset >> 3) & 0xFFFFFFFE;
  uint32_t value = bswap32(*reinterpret_cast<const uint32_t*>(u8_data + byte_offset)) << (bit_offset & 0x0F);
  return (value >> 31) & 1;
}

uint8_t get_2bit_at_offset(const void* data, size_t bit_offset) {
  // note: the original implementation is ((offset >> 4) << 1); this should be
  // equivalent. the right shift is signed, but args to this function always
  // appear to be positive so it shouldn't matter
  const uint8_t* u8_data = reinterpret_cast<const uint8_t*>(data);
  size_t byte_offset = (bit_offset >> 3) & 0xFFFFFFFE;
  uint32_t value = bswap32(*reinterpret_cast<const uint32_t*>(u8_data + byte_offset)) << (bit_offset & 0x0F);
  return (value >> 30) & 3;
}

uint32_t get_bits_at_offset(const void* data, size_t bit_offset, size_t count) { // fn658
  // note: the original implementation is ((offset >> 4) << 1); this should be
  // equivalent. the right shift is signed, but args to this function always
  // appear to be positive so it shouldn't matter
  const uint8_t* u8_data = reinterpret_cast<const uint8_t*>(data);
  size_t byte_offset = (bit_offset >> 3) & 0xFFFFFFFE;
  uint32_t value = bswap32(*reinterpret_cast<const uint32_t*>(u8_data + byte_offset)) << (bit_offset & 0x0F);
  return (value >> (32 - count));
}


void decode_dc2_sprite(const void* input_data, void* output_data) {
  // not part of the original; added to improve readability
  const InputFormat* input = reinterpret_cast<const InputFormat*>(input_data);

  // the result of this appears to be unused
  // InterfaceLib::Gestalt('cput', &var56); // TOC entry at offset 4

  // note: the original code appears to have a missing bounds check here
  uint8_t max_color = 1 << input->bits_per_pixel;
  const void* input_bitstream = &input->data[(max_color - 2) << 1];

  // TODO: make the code for the following computation not look dumb as hell
  uint8_t chunk_count_bits;
  {
    uint8_t r4;
    for (chunk_count_bits = 7, r4 = 0x80;
         (chunk_count_bits > 3) && (r4 >= bswap16(input->width));
         chunk_count_bits--, r4 >>= 1);
  }

  uint8_t* output_ptr = reinterpret_cast<uint8_t*>(output_data);
  size_t output_count = bswap16(input->height) * bswap16(input->width);
  uint8_t transparent_color = max_color - 1;
  size_t input_bitstream_offset = 0;
  ssize_t output_count_remaining = output_count;
  for (output_count_remaining = output_count; output_count_remaining > 0;) {
    uint8_t opcode = get_bits_at_offset(input_bitstream, input_bitstream_offset, 3);
    input_bitstream_offset += 3;

    size_t chunk_count;

    switch (opcode) {
      case 0: // label228
        chunk_count = get_bits_at_offset(input_bitstream, input_bitstream_offset, chunk_count_bits);
        input_bitstream_offset += chunk_count_bits;

        // write chunk_count + 1 zeroes to output
        for (size_t x = 0; x < chunk_count + 1; x++) {
          (*output_ptr++) = 0;
        }

        break;

      case 1: { // label26C
        chunk_count = get_bits_at_offset(input_bitstream, input_bitstream_offset, chunk_count_bits);
        input_bitstream_offset += chunk_count_bits;
        uint8_t color = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
        input_bitstream_offset += input->bits_per_pixel;

        // probably this is for transparency
        if (color == transparent_color) {
          color = 0xFF;
        }

        // write chunk_count + 1 copies of r3 to output
        for (size_t x = 0; x < chunk_count + 1; x++) {
          (*output_ptr++) = color;
        }

        break;
      }

      case 2: { // label2D4
        chunk_count = get_bits_at_offset(input_bitstream, input_bitstream_offset, chunk_count_bits);
        input_bitstream_offset += chunk_count_bits;

        uint8_t values[2];
        values[0] = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
        input_bitstream_offset += input->bits_per_pixel;
        if (values[0] == transparent_color) {
          values[0] = 0xFF;
        }

        values[1] = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
        input_bitstream_offset += input->bits_per_pixel;
        if (values[1] == transparent_color) {
          values[1] = 0xFF;
        }

        // write first color followed by a bitstream-determined alternation of
        // the two colors. note that we write one fewer than the count,
        // presumably because the first color is always written to save 1 bit.
        // wow such hyper-optimization jeez
        *(output_ptr++) = values[0];
        for (size_t x = 1; x < chunk_count + 1; x++) {
          bool which = get_bit_at_offset(input_bitstream, input_bitstream_offset);
          input_bitstream_offset++;
          *(output_ptr++) = values[which];
        }

        break;
      }

      case 3: { // label3A0
        chunk_count = get_bits_at_offset(input_bitstream, input_bitstream_offset, chunk_count_bits);
        input_bitstream_offset += chunk_count_bits;

        uint8_t values[4];

        values[0] = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
        input_bitstream_offset += input->bits_per_pixel;
        if (values[0] == transparent_color) {
          values[0] = 0xFF;
        }

        values[1] = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
        input_bitstream_offset += input->bits_per_pixel;
        if (values[1] == transparent_color) {
          values[1] = 0xFF;
        }

        values[2] = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
        input_bitstream_offset += input->bits_per_pixel;
        if (values[2] == transparent_color) {
          values[2] = 0xFF;
        }

        values[3] = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
        input_bitstream_offset += input->bits_per_pixel;
        if (values[3] == transparent_color) {
          values[3] = 0xFF;
        }

        // similar to opcode 2, but 4 possible values instead of 2
        *(output_ptr++) = values[0];
        for (size_t x = 1; x < chunk_count + 1; x++) {
          uint8_t which = get_2bit_at_offset(input_bitstream, input_bitstream_offset);
          input_bitstream_offset += 2;
          *(output_ptr++) = values[which];
        }

        break;
      }

      default: // 4, 5, 6, or 7. label4E4
        if (opcode == 7) {
          chunk_count = get_bits_at_offset(input_bitstream, input_bitstream_offset, chunk_count_bits);
          input_bitstream_offset += chunk_count_bits;
        } else {
          chunk_count = opcode - 4;
        }

        // copy chunk_count + 1 items from the input bitstream to the output
        for (size_t x = 0; x < chunk_count + 1; x++) {
          uint8_t value = get_bits_at_offset(input_bitstream, input_bitstream_offset, input->bits_per_pixel);
          input_bitstream_offset += input->bits_per_pixel;

          if (value == transparent_color) {
            value = 0xFF;
          }
          *(output_ptr++) = value;
        }
    }

    output_count_remaining -= (chunk_count + 1);
  }
  if (output_count_remaining < 0) {
    // InterfaceLib::DebugStr("Uh-Oh. too many pixels."); // TOC entry at offset 0
    throw runtime_error("Uh-Oh. too many pixels.");
  }

  // the original code does this, but we don't because it just makes the output
  // harder to parse. probably they did this for some draw-time optimizations
  // if (input->generate_transparency_map) {
  //   generate_transparency_map(output_count, output_data);
  // }
  return;
}

pair<Image, Image> decode_dc2_sprite(const void* input_data, size_t size) {
  // not part of the original; added to improve readability
  const InputFormat* input = reinterpret_cast<const InputFormat*>(input_data);
  int16_t h = bswap16(input->height);
  int16_t w = bswap16(input->width);
  size_t output_size = h * w * (input->generate_transparency_map ? 2 : 1);

  string output_data(output_size, '\0');
  decode_dc2_sprite(input_data, const_cast<char*>(output_data.data()));

  const int16_t* colors = reinterpret_cast<const int16_t*>(&input->data[0]);

  Image i(w, h);
  Image m(w, h);
  for (size_t y = 0; y < h; y++) {
    for (size_t x = 0; x < w; x++) {
      uint8_t color_index = output_data[y * w + x];
      if (color_index == 0) {
        i.write_pixel(x, y, 0x00, 0x00, 0x00);
        m.write_pixel(x, y, 0x00, 0x00, 0x00);
      } else if (color_index == 0xFF) {
        i.write_pixel(x, y, 0x00, 0x00, 0x00);
        m.write_pixel(x, y, 0xFF, 0xFF, 0xFF);
      } else {
        // guess: it's rgb 565
        int16_t color = bswap16(colors[color_index - 1]);
        uint8_t r = (((color >> 10) & 0x1F) * 0xFF) / 0x1F;
        uint8_t g = (((color >> 5) & 0x1F) * 0xFF) / 0x1F;
        uint8_t b = (((color >> 0) & 0x1F) * 0xFF) / 0x1F;
        i.write_pixel(x, y, r, g, b);
        m.write_pixel(x, y, 0xFF, 0xFF, 0xFF);
      }
    }
  }

  return make_pair(i, m);
}