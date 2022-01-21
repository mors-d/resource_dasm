#include <string.h>

#include <phosg/Filesystem.hh>
#include <phosg/Image.hh>
#include <phosg/Strings.hh>
#include <stdexcept>

#include "ResourceFile.hh"
#include "M68KEmulator.hh"

using namespace std;

struct SpriHeader {
  uint16_t side;
  uint16_t area;
  // The TMPL says that in this field, 0 = mask and 1 = 68k executable code, but
  // this appears not to be the case. Every sprite in the file has 0 here, and
  // all of them contain executable code.
  uint8_t mask_type;
  uint8_t unused;
  // Variable-length fields:
  // uint8_t sprite_data[area]
  // uint8_t blitter_code[...EOF]

  void byteswap() {
    this->side = bswap16(this->side);
    this->area = bswap16(this->area);
  }
};

Image render_spri(const void* spri_data, size_t size,
    const std::vector<ColorTableEntry>& clut, bool trace) {
  StringReader r(spri_data, size);

  SpriHeader header = r.get_sw<SpriHeader>();
  if (header.area != header.side * header.side) {
    throw runtime_error("sprite is not square");
  }
  string data = r.read(header.area);
  string code = r.read(r.size() - r.where());

  shared_ptr<MemoryContext> mem(new MemoryContext());

  // Memory map:
  // 10000000 - output color data
  // 20000000 - output alpha data
  // 40000000 - input color data (original sprite data)
  // 50000000 - input alpha data ("\xFF" * size of sprite data)
  // 80000000 - stack (4KB)
  // C0000000 - renderer code
  // F0000000 - wrapper code (entry point is here)

  // Set up the output regions
  uint32_t output_color_addr = mem->allocate_at(0x10000000, header.area);
  memset(mem->at(output_color_addr, header.area), 0, header.area);
  uint32_t output_alpha_addr = mem->allocate_at(0x20000000, header.area);
  memset(mem->at(output_alpha_addr, header.area), 0, header.area);

  // Set up the input regions
  uint32_t input_color_addr = mem->allocate_at(0x40000000, header.area);
  memcpy(mem->at(input_color_addr, header.area), data.data(), header.area);
  uint32_t input_alpha_addr = mem->allocate_at(0x50000000, header.area);
  memset(mem->at(input_alpha_addr, header.area), 0xFF, header.area);

  // Set up the stack
  const uint32_t stack_size = 0x1000;
  uint32_t stack_addr = mem->allocate_at(0x80000000, stack_size);
  memset(mem->at(stack_addr, stack_size), 0x00, stack_size);

  // Set up the code region
  uint32_t code_addr = mem->allocate_at(0xC0000000, code.size());
  memcpy(mem->at(code_addr, code.size()), code.data(), code.size());

  // The sprite renderer code expects the following stack at entry time:
  // [A7+00] return addr
  // [A7+04] input row_bytes
  // [A7+08] output row_bytes
  // [A7+0C] input buffer addr
  // [A7+10] output buffer addr

  // Write a short bit of 68K code to call the sprite renderer twice.
  StringWriter wrapper_code_w;
  // pea.l output_color_buffer
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(output_color_addr);
  // pea.l input_color_buffer
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(input_color_addr);
  // pea.l row_bytes
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(header.side);
  // pea.l row_bytes
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(header.side);
  // jsr [code_addr]
  wrapper_code_w.put_u16r(0x4EB9);
  wrapper_code_w.put_u32r(code_addr);
  // adda.w A7, 0x10
  wrapper_code_w.put_u32r(0xDEFC0010);
  // pea.l output_alpha_buffer
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(output_alpha_addr);
  // pea.l input_alpha_buffer
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(input_alpha_addr);
  // pea.l row_bytes
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(header.side);
  // pea.l row_bytes
  wrapper_code_w.put_u16r(0x4879);
  wrapper_code_w.put_u32r(header.side);
  // jsr [code_addr]
  wrapper_code_w.put_u16r(0x4EB9);
  wrapper_code_w.put_u32r(code_addr);
  // reset (this terminates emulation cleanly)
  wrapper_code_w.put_u16r(0x4E70);

  // Set up the wrapper code region. The initial pc is at the start of this
  // region.
  const string& wrapper_code = wrapper_code_w.str();
  uint32_t wrapper_code_addr = mem->allocate_at(0xF0000000, wrapper_code.size());
  memcpy(mem->at(wrapper_code_addr, wrapper_code.size()), wrapper_code.data(), wrapper_code.size());

  // Set up registers
  M68KRegisters regs;
  regs.a[7] = stack_addr + stack_size;
  regs.pc = wrapper_code_addr;

  // Run the renderer
  M68KEmulator emu(mem);
  if (trace) {
    emu.print_state_header(stderr);
    emu.set_debug_hook([&](M68KEmulator& emu, M68KRegisters&) -> bool {
      emu.print_state(stderr);
      return true;
    });
  }
  emu.execute(regs);

  // The sprite renderer code has executed, giving us two buffers: one with the
  // sprite's (indexed) color data, and another with the alpha channel. Convert
  // these to an Image and return it.
  const uint8_t* output_color = mem->obj<const uint8_t>(output_color_addr, header.area);
  const uint8_t* output_alpha = mem->obj<const uint8_t>(output_alpha_addr, header.area);
  Image ret(header.side, header.side, true);
  for (size_t y = 0; y < header.side; y++) {
    for (size_t x = 0; x < header.side; x++) {
      size_t z = (y * header.side) + x;
      auto color = clut.at(output_color[z]).c.as8();
      ret.write_pixel(x, y, color.r, color.g, color.b, output_alpha[z]);
    }
  }

  return ret;
}



int main(int argc, char* argv[]) {
  if (argc != 3) {
    fprintf(stderr, "Usage: thezone_render Spri_filename clut_filename\n");
    return 2;
  }
  const char* spri_filename = argv[1];
  const char* clut_filename = argv[2];

  string data = load_file(spri_filename);

  string clut_data = load_file(clut_filename);
  auto clut = ResourceFile::decode_clut(clut_data.data(), clut_data.size());

  auto img = render_spri(data.data(), data.size(), clut, false);
  string out_filename = string_printf("%s.bmp", spri_filename);
  img.save(out_filename.c_str(), Image::ImageFormat::WindowsBitmap);
  fprintf(stderr, "... %s\n", out_filename.c_str());

  return 0;
}
