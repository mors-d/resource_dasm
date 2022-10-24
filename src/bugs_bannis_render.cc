#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include <phosg/Image.hh>
#include <phosg/Filesystem.hh>
#include <phosg/Strings.hh>
#include <stdexcept>
#include <string>

#include "TextCodecs.hh"

using namespace std;



Image render_Levs(const string& data, const Image& tile_sheet) {
  if (data.size() != 0x800) {
    throw runtime_error("data size is incorrect");
  }
  StringReader r(data);

  // The time and required carrots are encoded in the high bytes of the first
  // few tiles.
  uint8_t minutes = r.pget_u8(0x00);
  uint8_t seconds = r.pget_u8(0x02);
  uint8_t carrots = r.pget_u8(0x04);

  static const vector<uint16_t> tile_remap({
    /* 00 */ 0xFFFF, 0x0000, 0xFFFF, 0x0002, 0x0003, 0x0130, 0x000C, 0x0006,
    /* 08 */ 0xFFFF, 0x0010, 0x001C, 0xFFFF, 0x0032, 0x0023, 0x0084, 0x002A,
    /* 10 */ 0xFFFF, 0xFFFF, 0xFFFF, 0x003E, 0x0042, 0x0132, 0xFFFF, 0xFFFF,
    /* 18 */ 0xFFFF, 0xFFFF, 0xFFFF, 0x0021, 0xFFFF, 0x006E, 0xFFFF, 0xFFFF,
    /* 20 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0x007F, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 28 */ 0x0004, 0x0123, 0xFFFF, 0x007A, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 30 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 38 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 40 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 48 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 50 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 58 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 60 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 68 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 70 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 78 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 80 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 88 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 90 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* 98 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* A0 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* A8 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* B0 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* B8 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* C0 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* C8 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* D0 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* D8 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* E0 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* E8 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* F0 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
    /* F8 */ 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF,
  });

  Image ret(32 * 32, 32 * 32, false);
  for (size_t y = 0; y < 0x20; y++) {
    for (size_t x = 0; x < 0x20; x++) {
      // Seems like only the low byte is relevant?
      uint16_t tile_id = r.get_u16b() & 0xFF;
      uint16_t remapped_tile_id = tile_remap.at(tile_id);
      uint16_t effective_tile_id = (remapped_tile_id == 0xFFFF) ? tile_id : remapped_tile_id;
      size_t tile_sheet_x = (effective_tile_id & 0x000F) << 5;
      size_t tile_sheet_y = (effective_tile_id & 0xFFF0) << 1;
      if (remapped_tile_id == 0xFFFF) {
        ret.fill_rect(x << 5, y << 5, 32, 32, 0xFF0000FF);
        ret.draw_text((x << 5) + 1, (y << 5) + 1, 0x00000000, 0xFF0000FF,
            "%02hX", tile_id);
      } else {
        ret.blit(tile_sheet, x << 5, y << 5, 32, 32, tile_sheet_x, tile_sheet_y);
      }
    }
  }

  ret.draw_text(1, 1, 0xFFFFFFFF, 0x00000080, "Time: %hhu:%02hhu - Carrots: %hhu",
      minutes, seconds, carrots);
  return ret;
}



int main(int argc, char** argv) {
  if (argc < 3 || argc > 4) {
    fprintf(stderr, "\
Usage: bugs_bannis_render <Levs-file.bin> <PICT-132.bmp> [output-filename]\n\
\n\
You can get Levs files by using resource_dasm on the Bugs Bannis game itself.\n\
PICT-132.bmp also comes from Bugs Bannis, but you may need to use replace_clut\n\
to correct its palette.\n\
\n\
If no output filename is given, the output is written to <Levs-file>.bmp.\n\
\n");
    return 2;
  }

  const char* input_filename = (argc > 1) ? argv[1] : nullptr;
  const char* tile_sheet_filename = (argc > 2) ? argv[2] : nullptr;
  string output_filename = (argc > 3) ? argv[3] : "";

  if (!input_filename) {
    throw runtime_error("input filename must be given");
  }
  if (!tile_sheet_filename) {
    throw runtime_error("tile sheet filename must be given");
  }
  if (output_filename.empty()) {
    output_filename = string_printf("%s.bmp", input_filename);
  }

  string input_data = load_file(input_filename);

  Image tile_sheet(tile_sheet_filename);
  if (tile_sheet.get_width() < 16 * 32) {
    throw runtime_error("tile sheet is too narrow");
  }
  if (tile_sheet.get_height() < 16 * 32) {
    throw runtime_error("tile sheet is too short");
  }

  Image map = render_Levs(input_data, tile_sheet);
  map.save(output_filename, Image::Format::WINDOWS_BITMAP);

  fprintf(stderr, "... %s\n", output_filename.c_str());
  return 0;
}