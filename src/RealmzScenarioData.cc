#include "RealmzScenarioData.hh"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <array>
#include <deque>
#include <phosg/Encoding.hh>
#include <phosg/Image.hh>
#include <phosg/Strings.hh>
#include <set>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "IndexFormats/Formats.hh"
#include "ResourceFile.hh"

using namespace std;

RealmzScenarioData::RealmzScenarioData(
    RealmzGlobalData& global, const string& scenario_dir, const string& name)
    : global(global),
      scenario_dir(scenario_dir),
      name(name) {

  string scenario_metadata_name = this->scenario_dir + "/" + this->name;
  string global_metadata_name = first_file_that_exists({(this->scenario_dir + "/global"),
      (this->scenario_dir + "/Global")});
  string monster_index_name = first_file_that_exists({(this->scenario_dir + "/data_md"),
      (this->scenario_dir + "/Data MD"),
      (this->scenario_dir + "/DATA MD"),
      (this->scenario_dir + "/data_md1"),
      (this->scenario_dir + "/Data MD1"),
      (this->scenario_dir + "/DATA MD1"),
      (this->scenario_dir + "/data_md_1"),
      (this->scenario_dir + "/data_md-1"),
      (this->scenario_dir + "/Data MD-1"),
      (this->scenario_dir + "/DATA MD-1")});
  string battle_index_name = first_file_that_exists({(this->scenario_dir + "/data_bd"),
      (this->scenario_dir + "/Data BD"),
      (this->scenario_dir + "/DATA BD")});
  string dungeon_map_index_name = first_file_that_exists({(this->scenario_dir + "/data_dl"),
      (this->scenario_dir + "/Data DL"),
      (this->scenario_dir + "/DATA DL")});
  string land_map_index_name = first_file_that_exists({(this->scenario_dir + "/data_ld"),
      (this->scenario_dir + "/Data LD"),
      (this->scenario_dir + "/DATA LD")});
  string string_index_name = first_file_that_exists({(this->scenario_dir + "/data_sd2"),
      (this->scenario_dir + "/Data SD2"),
      (this->scenario_dir + "/DATA SD2")});
  string option_string_index_name = first_file_that_exists({(this->scenario_dir + "/data_od"),
      (this->scenario_dir + "/Data OD"),
      (this->scenario_dir + "/DATA OD")});
  string ecodes_index_name = first_file_that_exists({(this->scenario_dir + "/data_edcd"),
      (this->scenario_dir + "/Data EDCD"),
      (this->scenario_dir + "/DATA EDCD")});
  string land_ap_index_name = first_file_that_exists({(this->scenario_dir + "/data_dd"),
      (this->scenario_dir + "/Data DD"),
      (this->scenario_dir + "/DATA DD")});
  string dungeon_ap_index_name = first_file_that_exists({(this->scenario_dir + "/data_ddd"),
      (this->scenario_dir + "/Data DDD"),
      (this->scenario_dir + "/DATA DDD")});
  string extra_ap_index_name = first_file_that_exists({(this->scenario_dir + "/data_ed3"),
      (this->scenario_dir + "/Data ED3"),
      (this->scenario_dir + "/DATA ED3")});
  string land_metadata_index_name = first_file_that_exists({(this->scenario_dir + "/data_rd"),
      (this->scenario_dir + "/Data RD"),
      (this->scenario_dir + "/DATA RD")});
  string dungeon_metadata_index_name = first_file_that_exists({(this->scenario_dir + "/data_rdd"),
      (this->scenario_dir + "/Data RDD"),
      (this->scenario_dir + "/DATA RDD")});
  string simple_encounter_index_name = first_file_that_exists({(this->scenario_dir + "/data_ed"),
      (this->scenario_dir + "/Data ED"),
      (this->scenario_dir + "/DATA ED")});
  string complex_encounter_index_name = first_file_that_exists({(this->scenario_dir + "/data_ed2"),
      (this->scenario_dir + "/Data ED2"),
      (this->scenario_dir + "/DATA ED2")});
  string party_map_index_name = first_file_that_exists({(this->scenario_dir + "/data_md2"),
      (this->scenario_dir + "/Data MD2"),
      (this->scenario_dir + "/DATA MD2")});
  string custom_item_index_name = first_file_that_exists({(this->scenario_dir + "/data_ni"),
      (this->scenario_dir + "/Data NI"),
      (this->scenario_dir + "/DATA NI")});
  string shop_index_name = first_file_that_exists({(this->scenario_dir + "/data_sd"),
      (this->scenario_dir + "/Data SD"),
      (this->scenario_dir + "/DATA SD")});
  string treasure_index_name = first_file_that_exists({(this->scenario_dir + "/data_td"),
      (this->scenario_dir + "/Data TD"),
      (this->scenario_dir + "/DATA TD")});
  string rogue_encounter_index_name = first_file_that_exists({(this->scenario_dir + "/data_td2"),
      (this->scenario_dir + "/Data TD2"),
      (this->scenario_dir + "/DATA TD2")});
  string time_encounter_index_name = first_file_that_exists({(this->scenario_dir + "/data_td3"),
      (this->scenario_dir + "/Data TD3"),
      (this->scenario_dir + "/DATA TD3")});
  string scenario_resources_name = first_file_that_exists({(this->scenario_dir + "/scenario.rsf"),
      (this->scenario_dir + "/Scenario.rsf"),
      (this->scenario_dir + "/SCENARIO.RSF"),
      (this->scenario_dir + "/scenario/rsrc"),
      (this->scenario_dir + "/Scenario/rsrc"),
      (this->scenario_dir + "/SCENARIO/rsrc"),
      (this->scenario_dir + "/scenario/..namedfork/rsrc"),
      (this->scenario_dir + "/Scenario/..namedfork/rsrc"),
      (this->scenario_dir + "/SCENARIO/..namedfork/rsrc")});

  this->monsters = this->load_monster_index(monster_index_name);
  this->battles = this->load_battle_index(battle_index_name);
  this->dungeon_maps = this->load_dungeon_map_index(dungeon_map_index_name);
  this->land_maps = this->load_land_map_index(land_map_index_name);
  this->strings = this->load_string_index(string_index_name);
  this->option_strings = this->load_option_string_index(option_string_index_name);
  this->ecodes = this->load_ecodes_index(ecodes_index_name);
  this->dungeon_aps = this->load_ap_index(dungeon_ap_index_name);
  this->land_aps = this->load_ap_index(land_ap_index_name);
  this->xaps = this->load_xap_index(extra_ap_index_name);
  this->dungeon_metadata = this->load_map_metadata_index(dungeon_metadata_index_name);
  this->land_metadata = this->load_map_metadata_index(land_metadata_index_name);
  this->simple_encounters = this->load_simple_encounter_index(simple_encounter_index_name);
  this->complex_encounters = this->load_complex_encounter_index(complex_encounter_index_name);
  this->party_maps = this->load_party_map_index(party_map_index_name);
  this->custom_items = this->load_custom_item_index(custom_item_index_name);
  this->shops = this->load_shop_index(shop_index_name);
  this->treasures = this->load_treasure_index(treasure_index_name);
  this->rogue_encounters = this->load_rogue_encounter_index(rogue_encounter_index_name);
  this->time_encounters = this->load_time_encounter_index(time_encounter_index_name);
  // Some scenarios apparently don't have global metadata
  if (!global_metadata_name.empty()) {
    this->global_metadata = this->load_global_metadata(global_metadata_name);
  }
  this->scenario_metadata = this->load_scenario_metadata(scenario_metadata_name);
  this->scenario_rsf = parse_resource_fork(load_file(scenario_resources_name));

  this->item_info = RealmzGlobalData::parse_item_info(this->scenario_rsf);
  this->spell_names = RealmzGlobalData::parse_spell_names(this->scenario_rsf);

  // Load layout separately because it doesn't have to exist
  {
    string fname = first_file_that_exists({(this->scenario_dir + "/layout"),
        (this->scenario_dir + "/Layout")});
    if (!fname.empty()) {
      this->layout = this->load_land_layout(fname);
    } else {
      fprintf(stderr, "note: this scenario has no land layout information\n");
    }
  }

  // Load tilesets
  for (int z = 1; z < 4; z++) {
    string fname = first_file_that_exists({string_printf("%s/data_custom_%d_bd", this->scenario_dir.c_str(), z),
        string_printf("%s/Data Custom %d BD", this->scenario_dir.c_str(), z),
        string_printf("%s/DATA CUSTOM %d BD", this->scenario_dir.c_str(), z)});
    if (!fname.empty()) {
      string land_type = string_printf("custom_%d", z);
      this->land_type_to_tileset_definition.emplace(
          std::move(land_type), load_tileset_definition(fname));
    }
  }
}

const string& RealmzScenarioData::name_for_spell(uint16_t id) const {
  try {
    return this->spell_names.at(id);
  } catch (const out_of_range&) {
    return this->global.name_for_spell(id);
  }
}

string RealmzScenarioData::desc_for_spell(uint16_t id) const {
  try {
    const auto& name = this->global.name_for_spell(id);
    return string_printf("%d(%s)", id, name.c_str());
  } catch (const out_of_range&) {
    return string_printf("%d", id);
  }
}

const ItemInfo& RealmzScenarioData::info_for_item(uint16_t id) const {
  try {
    return this->item_info.at(id);
  } catch (const out_of_range&) {
    return this->global.info_for_item(id);
  }
}

string RealmzScenarioData::desc_for_item(uint16_t id) const {
  try {
    const auto& info = this->info_for_item(id);
    return string_printf("%d(%s)", id, info.name.c_str());
  } catch (const out_of_range&) {
    return string_printf("%d", id);
  }
}

static string render_string_reference(
    const vector<string>& strings, int index) {
  if (index == 0) {
    return "0";
  }
  if ((size_t)abs(index) >= strings.size()) {
    return string_printf("%d", index);
  }

  // Strings in Realmz scenarios often end with a bunch of spaces, which looks
  // bad in the disassembly and serves no purpose, so we trim them off here.
  string s = strings[abs(index)];
  strip_trailing_whitespace(s);
  s = escape_quotes(s);
  return string_printf("\"%s\"#%d", s.c_str(), index);
}

////////////////////////////////////////////////////////////////////////////////
// DATA MD2

vector<RealmzScenarioData::PartyMap> RealmzScenarioData::load_party_map_index(
    const string& filename) {
  return load_vector_file<PartyMap>(filename);
}

string RealmzScenarioData::disassemble_party_map(size_t index) {
  const PartyMap& pm = this->party_maps.at(index);

  string ret = string_printf("===== %s MAP id=%zu level=%hd x=%hd y=%hd tile_size=%hd [MAP%zu]\n",
      (pm.is_dungeon ? "DUNGEON" : "LAND"), index, pm.level_num.load(), pm.x.load(), pm.y.load(), pm.tile_size.load(), index);
  if (pm.picture_id) {
    ret += string_printf("  picture id=%hd\n", pm.picture_id.load());
  }
  if (pm.text_id) {
    ret += string_printf("  text id=%hd\n", pm.text_id.load());
  }

  for (int x = 0; x < 10; x++) {
    if (!pm.annotations[x].icon_id && !pm.annotations[x].x && !pm.annotations[x].y) {
      continue;
    }
    ret += string_printf("  annotation icon_id=%d x=%d y=%d\n",
        pm.annotations[x].icon_id.load(), pm.annotations[x].x.load(), pm.annotations[x].y.load());
  }

  string description(pm.description, pm.description_valid_chars);
  ret += string_printf("  description=\"%s\"\n", description.c_str());
  return ret;
}

string RealmzScenarioData::disassemble_all_party_maps() {
  deque<string> blocks;
  for (size_t z = 0; z < this->party_maps.size(); z++) {
    blocks.emplace_back(this->disassemble_party_map(z));
  }
  return join(blocks, "");
}

Image RealmzScenarioData::render_party_map(size_t index) {
  const auto& pm = this->party_maps.at(index);

  if (!pm.tile_size) {
    throw runtime_error("tile size is zero");
  }
  if (pm.tile_size > (pm.is_dungeon ? 16 : 32)) {
    throw runtime_error("tile size is too large");
  }

  double whf = 320.0 / pm.tile_size;
  size_t wh = static_cast<size_t>(ceil(whf));

  Image ret;
  if (pm.is_dungeon) {
    ret = generate_dungeon_map(pm.level_num, pm.x, pm.y, wh, wh);
  } else {
    ret = generate_land_map(pm.level_num, pm.x, pm.y, wh, wh);
  }

  size_t rendered_tile_size = pm.is_dungeon ? 16 : 32;
  for (int x = 0; x < 10; x++) {
    const auto& a = pm.annotations[x];
    if (!a.icon_id) {
      continue;
    }
    Image cicn;
    try {
      cicn = this->scenario_rsf.decode_cicn(a.icon_id).image;
    } catch (const out_of_range&) {
    }
    try {
      cicn = this->global.global_rsf.decode_cicn(a.icon_id).image;
    } catch (const out_of_range&) {
    }
    if (cicn.get_width() == 0 || cicn.get_height() == 0) {
      fprintf(stderr, "warning: map refers to missing cicn %hd\n", a.icon_id.load());
    } else {
      // It appears that annotations should render centered on the tile on which
      // they are defined, so we may need to adjust dest x/y is the cicn size
      // isn't the same as the tile size.
      ssize_t px = a.x * rendered_tile_size - (cicn.get_width() - rendered_tile_size) / 2;
      ssize_t py = a.y * rendered_tile_size - (cicn.get_height() - rendered_tile_size) / 2;
      ret.blit(cicn, px, py, cicn.get_width(), cicn.get_height(), 0, 0);
    }
  }

  return ret;
}

////////////////////////////////////////////////////////////////////////////////
// LAYOUT

RealmzScenarioData::LevelNeighbors::LevelNeighbors()
    : x(-1),
      y(-1),
      left(-1),
      right(-1),
      top(-1),
      bottom(-1) {}

RealmzScenarioData::LandLayout::LandLayout() {
  for (size_t y = 0; y < 8; y++) {
    for (size_t x = 0; x < 16; x++) {
      this->layout[y][x] = -1;
    }
  }
}

RealmzScenarioData::LandLayout::LandLayout(const LandLayout& l) {
  for (size_t y = 0; y < 8; y++) {
    for (size_t x = 0; x < 16; x++) {
      this->layout[y][x] = l.layout[y][x];
    }
  }
}

RealmzScenarioData::LandLayout& RealmzScenarioData::LandLayout::operator=(
    const LandLayout& l) {
  for (size_t y = 0; y < 8; y++) {
    for (size_t x = 0; x < 16; x++) {
      this->layout[y][x] = l.layout[y][x];
    }
  }
  return *this;
}

size_t RealmzScenarioData::LandLayout::num_valid_levels() const {
  size_t count = 0;
  for (size_t y = 0; y < 8; y++) {
    for (size_t x = 0; x < 16; x++) {
      if (this->layout[y][x] >= 0) {
        count++;
      }
    }
  }
  return count;
}

RealmzScenarioData::LandLayout RealmzScenarioData::load_land_layout(
    const string& filename) {
  LandLayout l = load_object_file<LandLayout>(filename, true);
  for (size_t y = 0; y < 8; y++) {
    for (size_t x = 0; x < 16; x++) {
      if (l.layout[y][x] == -1) {
        l.layout[y][x] = 0;
      } else if (l.layout[y][x] == 0) {
        l.layout[y][x] = -1;
      }
    }
  }
  return l;
}

RealmzScenarioData::LevelNeighbors
RealmzScenarioData::LandLayout::get_level_neighbors(int16_t id) const {
  LevelNeighbors n;
  for (size_t y = 0; y < 8; y++) {
    for (size_t x = 0; x < 16; x++) {
      if (this->layout[y][x] == id) {
        if (n.x != -1 || n.y != -1) {
          throw runtime_error("multiple entries for level");
        }

        n.x = x;
        n.y = y;
        if (x != 0) {
          n.left = this->layout[y][x - 1];
        }
        if (x != 15) {
          n.right = this->layout[y][x + 1];
        }
        if (y != 0) {
          n.top = this->layout[y - 1][x];
        }
        if (y != 7) {
          n.bottom = this->layout[y + 1][x];
        }
      }
    }
  }
  return n;
}

vector<RealmzScenarioData::LandLayout>
RealmzScenarioData::LandLayout::get_connected_components() const {
  LandLayout remaining_components(*this);

  vector<LandLayout> ret;
  for (ssize_t y = 0; y < 8; y++) {
    for (ssize_t x = 0; x < 16; x++) {
      if (remaining_components.layout[y][x] == -1) {
        continue;
      }

      // This cell is the upper-left corner of a connected component; use
      // flood-fill to copy it to this_component
      LandLayout this_component;
      set<pair<ssize_t, ssize_t>> to_fill;
      to_fill.insert(make_pair(x, y));
      while (!to_fill.empty()) {
        auto pt = *to_fill.begin();
        to_fill.erase(pt);
        if (pt.first < 0 || pt.first >= 16 || pt.second < 0 || pt.second >= 8) {
          continue;
        }
        if (remaining_components.layout[pt.second][pt.first] == -1) {
          continue;
        }
        this_component.layout[pt.second][pt.first] = remaining_components.layout[pt.second][pt.first];
        remaining_components.layout[pt.second][pt.first] = -1;
        to_fill.insert(make_pair(pt.first - 1, pt.second));
        to_fill.insert(make_pair(pt.first + 1, pt.second));
        to_fill.insert(make_pair(pt.first, pt.second - 1));
        to_fill.insert(make_pair(pt.first, pt.second + 1));
      }

      ret.emplace_back(this_component);
    }
  }
  return ret;
}

Image RealmzScenarioData::generate_layout_map(const LandLayout& l) {

  ssize_t min_x = 16, min_y = 8, max_x = -1, max_y = -1;
  for (ssize_t y = 0; y < 8; y++) {
    for (ssize_t x = 0; x < 16; x++) {
      if (l.layout[y][x] < 0) {
        continue;
      }

      // If the level has no valid neighbors, ignore it
      if (x > 0 && l.layout[y][x - 1] < 0 &&
          x < 15 && l.layout[y][x + 1] < 0 &&
          y > 0 && l.layout[y - 1][x] < 0 &&
          y < 7 && l.layout[y + 1][x] < 0) {
        continue;
      }

      if (x < min_x) {
        min_x = x;
      }
      if (x > max_x) {
        max_x = x;
      }
      if (y < min_y) {
        min_y = y;
      }
      if (y > max_y) {
        max_y = y;
      }
    }
  }

  if (max_x < min_x || max_y < min_y) {
    throw runtime_error("layout has no valid levels");
  }

  max_x++;
  max_y++;

  Image overall_map(90 * 32 * (max_x - min_x), 90 * 32 * (max_y - min_y));
  for (ssize_t y = 0; y < (max_y - min_y); y++) {
    for (ssize_t x = 0; x < (max_x - min_x); x++) {
      int16_t level_id = l.layout[y + min_y][x + min_x];
      if (level_id < 0) {
        continue;
      }

      int xp = 90 * 32 * x;
      int yp = 90 * 32 * y;

      try {
        Image this_level_map = this->generate_land_map(level_id, 0, 0, 90, 90);

        // If get_level_neighbors fails, then we would not have written any
        // boundary information on the original map, so we can just ignore this
        int sx = 0, sy = 0;
        try {
          LevelNeighbors n = l.get_level_neighbors(level_id);
          sx = (n.left >= 0) ? 9 : 0;
          sy = (n.top >= 0) ? 9 : 0;
        } catch (const runtime_error&) {
        }

        overall_map.blit(this_level_map, xp, yp, 90 * 32, 90 * 32, sx, sy);
      } catch (const exception& e) {
        overall_map.fill_rect(xp, yp, 90 * 32, 90 * 32, 0xFFFFFFFF);
        overall_map.draw_text(xp + 10, yp + 10, 0xFF0000FF, 0x00000000,
            "can\'t read disassembled map %hd", level_id);
        overall_map.draw_text(xp + 10, yp + 20, 0x000000FF, 0x00000000,
            "%s", e.what());
      }
    }
  }

  return overall_map;
}

////////////////////////////////////////////////////////////////////////////////
// GLOBAL

RealmzScenarioData::GlobalMetadata RealmzScenarioData::load_global_metadata(
    const string& filename) {
  return load_object_file<GlobalMetadata>(filename, true);
}

string RealmzScenarioData::disassemble_globals() {
  return string_printf("===== GLOBAL METADATA [GMD]\n"
                       "  start_xap id=XAP%d\n"
                       "  death_xap id=XAP%d\n"
                       "  quit_xap id=XAP%d\n"
                       "  reserved1_xap id=XAP%d\n"
                       "  shop_xap id=XAP%d\n"
                       "  temple_xap id=XAP%d\n"
                       "  reserved2_xap id=XAP%d\n",
      this->global_metadata.start_xap.load(),
      this->global_metadata.death_xap.load(),
      this->global_metadata.quit_xap.load(),
      this->global_metadata.reserved1_xap.load(),
      this->global_metadata.shop_xap.load(),
      this->global_metadata.temple_xap.load(),
      this->global_metadata.reserved2_xap.load());
}

////////////////////////////////////////////////////////////////////////////////
// SCENARIO NAME

RealmzScenarioData::ScenarioMetadata RealmzScenarioData::load_scenario_metadata(
    const string& filename) {
  return load_object_file<ScenarioMetadata>(filename, true);
}

////////////////////////////////////////////////////////////////////////////////
// DATA EDCD

vector<RealmzScenarioData::ECodes> RealmzScenarioData::load_ecodes_index(
    const string& filename) {
  return load_vector_file<ECodes>(filename);
}

////////////////////////////////////////////////////////////////////////////////
// DATA TD

vector<RealmzScenarioData::Treasure> RealmzScenarioData::load_treasure_index(
    const string& filename) {
  return load_vector_file<Treasure>(filename);
}

string RealmzScenarioData::disassemble_treasure(size_t index) {
  const auto& t = this->treasures.at(index);

  string ret = string_printf("===== TREASURE id=%zu [TSR%zu]", index, index);

  if (t.victory_points < 0) {
    ret += string_printf(" victory_points=rand(1,%d)", -t.victory_points);
  } else if (t.victory_points > 0) {
    ret += string_printf(" victory_points=%d", t.victory_points.load());
  }

  if (t.gold < 0) {
    ret += string_printf(" gold=rand(1,%d)", -t.gold.load());
  } else if (t.gold > 0) {
    ret += string_printf(" gold=%d", t.gold.load());
  }

  if (t.gems < 0) {
    ret += string_printf(" gems=rand(1,%d)", -t.gems.load());
  } else if (t.gems > 0) {
    ret += string_printf(" gems=%d", t.gems.load());
  }

  if (t.jewelry < 0) {
    ret += string_printf(" jewelry=rand(1,%d)", -t.jewelry.load());
  } else if (t.jewelry > 0) {
    ret += string_printf(" jewelry=%d", t.jewelry.load());
  }

  ret += '\n';

  for (int x = 0; x < 20; x++) {
    if (t.item_ids[x]) {
      try {
        const auto& info = this->info_for_item(t.item_ids[x]);
        ret += string_printf("  %hd (%s)\n", t.item_ids[x].load(), info.name.c_str());
      } catch (const out_of_range&) {
        ret += string_printf("  %hd\n", t.item_ids[x].load());
      }
    }
  }

  return ret;
}

string RealmzScenarioData::disassemble_all_treasures() {
  deque<string> blocks;
  for (size_t z = 0; z < this->treasures.size(); z++) {
    blocks.emplace_back(this->disassemble_treasure(z));
  }
  return join(blocks, "");
}

////////////////////////////////////////////////////////////////////////////////
// DATA ED

vector<RealmzScenarioData::SimpleEncounter>
RealmzScenarioData::load_simple_encounter_index(const string& filename) {
  return load_vector_file<SimpleEncounter>(filename);
}

string RealmzScenarioData::disassemble_simple_encounter(size_t index) {
  const auto& e = this->simple_encounters.at(index);

  string prompt = render_string_reference(this->strings, e.prompt);
  string ret = string_printf("===== SIMPLE ENCOUNTER id=%zu can_backout=%hhd max_times=%hhd prompt=%s [SEC%zu]\n",
      index, e.can_backout, e.max_times, prompt.c_str(), index);

  vector<string> result_references[4];

  for (size_t x = 0; x < 4; x++) {
    string choice_text(e.choice_text[x].text, min(static_cast<size_t>(e.choice_text[x].valid_chars), static_cast<size_t>(sizeof(e.choice_text[x]) - 1)));
    strip_trailing_whitespace(choice_text);
    if (choice_text.empty()) {
      continue;
    }
    choice_text = escape_quotes(choice_text);
    ret += string_printf("  choice%zu: result=%d text=\"%s\"\n", x,
        e.choice_result_index[x], choice_text.c_str());
    if (e.choice_result_index[x] >= 1 && e.choice_result_index[x] <= 4) {
      result_references[e.choice_result_index[x] - 1].emplace_back(
          string_printf("ACTIVATE ON CHOICE %zu", x));
    }
  }

  for (size_t x = 0; x < 4; x++) {
    size_t y;
    for (y = 0; y < 8; y++) {
      if (e.choice_codes[x][y] || e.choice_args[x][y]) {
        break;
      }
    }
    if (y == 8 && result_references[x].empty()) {
      continue; // Option is blank; don't even print it
    }

    if (result_references[x].empty()) {
      ret += string_printf("  result%zu UNUSED\n", x + 1);
    } else {
      ret += string_printf("  result%zu\n", x + 1);
      for (const auto& ref : result_references[x]) {
        ret += string_printf("    %s\n", ref.c_str());
      }
    }

    for (size_t y = 0; y < 8; y++) {
      if (e.choice_codes[x][y] || e.choice_args[x][y]) {
        string dasm = disassemble_opcode(e.choice_codes[x][y], e.choice_args[x][y]);
        ret += string_printf("    %s\n", dasm.c_str());
      }
    }
  }

  return ret;
}

string RealmzScenarioData::disassemble_all_simple_encounters() {
  deque<string> blocks;
  for (size_t z = 0; z < this->simple_encounters.size(); z++) {
    blocks.emplace_back(this->disassemble_simple_encounter(z));
  }
  return join(blocks, "");
}

////////////////////////////////////////////////////////////////////////////////
// DATA ED2

vector<RealmzScenarioData::ComplexEncounter>
RealmzScenarioData::load_complex_encounter_index(const string& filename) {
  return load_vector_file<ComplexEncounter>(filename);
}

static const vector<const char*> rogue_encounter_action_names({
    "acrobatic_act",
    "detect_trap",
    "disable_trap",
    "action3",
    "force_lock",
    "action5",
    "pick_lock",
    "action7",
});

string RealmzScenarioData::disassemble_complex_encounter(size_t index) {
  const auto& e = this->complex_encounters.at(index);

  string prompt = render_string_reference(this->strings, e.prompt);
  string ret = string_printf("===== COMPLEX ENCOUNTER id=%zu can_backout=%hhd max_times=%hhd prompt=%s [CEC%zu]\n",
      index, e.can_backout, e.max_times, prompt.c_str(), index);

  vector<string> result_references[4];
  result_references[3].emplace_back("ACTIVATE DEFAULT");

  bool wrote_spell_header = false;
  for (size_t x = 0; x < 10; x++) {
    if (!e.spell_codes[x]) {
      continue;
    }
    if (!wrote_spell_header) {
      ret += "  spells\n";
      wrote_spell_header = true;
    }
    string spell_desc = this->desc_for_spell(e.spell_codes[x]);
    ret += string_printf("    result=%d, id=%s\n", e.spell_result_codes[x], spell_desc.c_str());
    if (e.spell_result_codes[x] >= 1 && e.spell_result_codes[x] <= 4) {
      result_references[e.spell_result_codes[x] - 1].emplace_back("ACTIVATE ON SPELL " + spell_desc);
    }
  }

  bool wrote_item_header = false;
  for (size_t x = 0; x < 5; x++) {
    if (!e.item_codes[x]) {
      continue;
    }
    if (!wrote_item_header) {
      ret += "  items\n";
      wrote_item_header = true;
    }
    auto item_desc = this->desc_for_item(e.item_codes[x]);
    ret += string_printf("    result=%d id=%s\n", e.item_result_codes[x], item_desc.c_str());
    if (e.item_result_codes[x] >= 1 && e.item_result_codes[x] <= 4) {
      result_references[e.item_result_codes[x] - 1].emplace_back("ACTIVATE ON ITEM " + item_desc);
    }
  }

  bool wrote_action_header = false;
  for (size_t x = 0; x < 5; x++) {
    string action_text(e.action_text[x].text, min((int)e.action_text[x].valid_chars, (int)sizeof(e.action_text[x]) - 1));
    strip_trailing_whitespace(action_text);
    if (action_text.empty()) {
      continue;
    }
    if (!wrote_action_header) {
      ret += string_printf("  actions result=%d\n", e.action_result);
      if (e.action_result >= 1 && e.action_result <= 4) {
        result_references[e.action_result - 1].emplace_back("ACTIVATE ON ACTION");
      }
      wrote_action_header = true;
    }
    action_text = escape_quotes(action_text);
    ret += string_printf("    selected=%d text=\"%s\"\n",
        e.actions_selected[x], action_text.c_str());
  }

  if (e.has_rogue_encounter) {
    try {
      const auto& re = this->rogue_encounters.at(e.rogue_encounter_id);
      ret += string_printf("  rogue_encounter id=%d reset=%d\n",
          e.rogue_encounter_id.load(), e.rogue_reset_flag);
      for (size_t z = 0; z < 8; z++) {
        if (!re.actions_available[z]) {
          continue;
        }
        if (re.success_result_codes[z] >= 1 && re.success_result_codes[z] <= 4) {
          result_references[re.success_result_codes[z] - 1].emplace_back(string_printf(
              "ACTIVATE ON ROGUE %s SUCCESS", rogue_encounter_action_names[z]));
        }
        if (re.failure_result_codes[z] >= 1 && re.failure_result_codes[z] <= 4) {
          result_references[re.failure_result_codes[z] - 1].emplace_back(string_printf(
              "ACTIVATE ON ROGUE %s FAILURE", rogue_encounter_action_names[z]));
        }
      }
    } catch (const out_of_range&) {
      ret += string_printf("  rogue encounter id=%d (MISSING) reset=%d\n",
          e.rogue_encounter_id.load(), e.rogue_reset_flag);
    }
  }

  string speak_text(e.speak_text.text, min((int)e.speak_text.valid_chars, (int)sizeof(e.speak_text) - 1));
  strip_trailing_whitespace(speak_text);
  if (!speak_text.empty()) {
    speak_text = escape_quotes(speak_text);
    ret += string_printf("  speak result=%d text=\"%s\"\n", e.speak_result,
        speak_text.c_str());
    if (e.speak_result >= 1 && e.speak_result <= 4) {
      result_references[e.speak_result - 1].emplace_back("ACTIVATE ON SPEAK");
    }
  }

  for (size_t x = 0; x < 4; x++) {
    size_t y;
    for (y = 0; y < 8; y++) {
      if (e.choice_codes[x][y] || e.choice_args[x][y]) {
        break;
      }
    }
    if (y == 8 && result_references[x].empty()) {
      continue; // Option is entirely blank; don't even print it
    }

    if (result_references[x].empty()) {
      ret += string_printf("  result%zu UNUSED\n", x + 1);
    } else {
      ret += string_printf("  result%zu\n", x + 1);
      for (const auto& ref : result_references[x]) {
        ret += string_printf("    %s\n", ref.c_str());
      }
    }

    for (size_t y = 0; y < 8; y++) {
      if (e.choice_codes[x][y] || e.choice_args[x][y]) {
        string dasm = disassemble_opcode(e.choice_codes[x][y], e.choice_args[x][y]);
        ret += string_printf("    %s\n", dasm.c_str());
      }
    }
  }

  return ret;
}

string RealmzScenarioData::disassemble_all_complex_encounters() {
  deque<string> blocks;
  for (size_t z = 0; z < this->complex_encounters.size(); z++) {
    blocks.emplace_back(this->disassemble_complex_encounter(z));
  }
  return join(blocks, "");
}

////////////////////////////////////////////////////////////////////////////////
// DATA TD2

vector<RealmzScenarioData::RogueEncounter>
RealmzScenarioData::load_rogue_encounter_index(const string& filename) {
  return load_vector_file<RogueEncounter>(filename);
}

string RealmzScenarioData::disassemble_rogue_encounter(size_t index) {
  const auto& e = this->rogue_encounters.at(index);

  string prompt = render_string_reference(strings, e.prompt_string);
  string ret = string_printf("===== ROGUE ENCOUNTER id=%zu sound=%hd prompt=%s "
                             "pct_per_level_to_open_lock=%hd pct_per_level_to_disable_trap=%hd "
                             "num_lock_tumblers=%hd [REC%zu]\n",
      index, e.prompt_sound.load(), prompt.c_str(), e.percent_per_level_to_open.load(),
      e.percent_per_level_to_disable.load(), e.num_lock_tumblers.load(), index);

  for (size_t x = 0; x < 8; x++) {
    if (!e.actions_available[x]) {
      continue;
    }
    string success_str = render_string_reference(strings, e.success_string_ids[x]);
    string failure_str = render_string_reference(strings, e.failure_string_ids[x]);

    ret += string_printf("  action_%s percent_modify=%d success_result=%d "
                         "failure_result=%d success_str=%s failure_str=%s success_sound=%d "
                         "failure_sound=%d\n",
        rogue_encounter_action_names[x],
        e.percent_modify[x], e.success_result_codes[x],
        e.failure_result_codes[x], success_str.c_str(), failure_str.c_str(),
        e.success_sound_ids[x].load(), e.failure_sound_ids[x].load());
  }

  if (e.is_trapped) {
    string spell_desc = this->desc_for_spell(e.trap_spell);
    ret += string_printf("  trap rogue_only=%d spell=%s spell_power=%d damage_range=[%d,%d] sound=%d\n",
        e.trap_affects_rogue_only, spell_desc.c_str(),
        e.trap_spell_power_level.load(), e.trap_damage_low.load(),
        e.trap_damage_high.load(), e.trap_sound.load());
  }

  return ret;
}

string RealmzScenarioData::disassemble_all_rogue_encounters() {
  deque<string> blocks;
  for (size_t z = 0; z < this->rogue_encounters.size(); z++) {
    blocks.emplace_back(this->disassemble_rogue_encounter(z));
  }
  return join(blocks, "");
}

////////////////////////////////////////////////////////////////////////////////
// DATA TD3

vector<RealmzScenarioData::TimeEncounter>
RealmzScenarioData::load_time_encounter_index(const string& filename) {
  return load_vector_file<TimeEncounter>(filename);
}

string RealmzScenarioData::disassemble_time_encounter(size_t index) {
  const auto& e = this->time_encounters.at(index);

  string ret = string_printf("===== TIME ENCOUNTER id=%zu", index);

  ret += string_printf(" day=%hd", e.day.load());
  ret += string_printf(" increment=%hd", e.increment.load());
  ret += string_printf(" percent_chance=%hd", e.percent_chance.load());
  ret += string_printf(" xap_id=XAP%hd", e.xap_id.load());
  if (e.required_level != -1) {
    ret += string_printf(" required_level: id=%hd(%s)", e.required_level.load(),
        e.land_or_dungeon == 1 ? "land" : "dungeon");
  }
  if (e.required_rect != -1) {
    ret += string_printf(" required_rect=%hd", e.required_rect.load());
  }
  if (e.required_x != -1 || e.required_y != -1) {
    ret += string_printf(" required_pos=(%hd,%hd)", e.required_x.load(), e.required_y.load());
  }
  if (e.required_item_id != -1) {
    ret += string_printf(" required_item_id=%hd", e.required_item_id.load());
    try {
      const auto& info = this->info_for_item(e.required_item_id);
      ret += string_printf("(%s)", info.name.c_str());
    } catch (const out_of_range&) {
    }
  }
  if (e.required_quest != -1) {
    ret += string_printf(" required_quest=%hd", e.required_quest.load());
  }

  ret += string_printf(" [TEC%zu]\n", index);
  return ret;
}

string RealmzScenarioData::disassemble_all_time_encounters() {
  deque<string> blocks;
  for (size_t z = 0; z < this->time_encounters.size(); z++) {
    blocks.emplace_back(this->disassemble_time_encounter(z));
  }
  return join(blocks, "");
}

////////////////////////////////////////////////////////////////////////////////
// DATA RD

static const unordered_map<uint8_t, string> land_type_to_string({
    {0, "outdoor"},
    {1, "reserved1"},
    {2, "reserved2"},
    {3, "cave"},
    {4, "indoor"},
    {5, "desert"},
    {6, "custom_1"},
    {7, "custom_2"},
    {8, "custom_3"},
    {9, "abyss"},
    {10, "snow"},
});

vector<RealmzScenarioData::MapMetadata>
RealmzScenarioData::load_map_metadata_index(const string& filename) {
  vector<MapMetadataFile> file_data = load_vector_file<MapMetadataFile>(filename);
  vector<MapMetadata> data(file_data.size());
  for (size_t x = 0; x < file_data.size(); x++) {
    try {
      data[x].land_type = land_type_to_string.at(file_data[x].land_type);
    } catch (const out_of_range& e) {
      data[x].land_type = "unknown";
    }
    for (size_t y = 0; y < 20; y++) {
      RandomRect r;
      r.top = file_data[x].coords[y].top;
      r.left = file_data[x].coords[y].left;
      r.bottom = file_data[x].coords[y].bottom;
      r.right = file_data[x].coords[y].right;
      r.times_in_10k = file_data[x].times_in_10k[y];
      r.battle_low = file_data[x].battle_range[y].low;
      r.battle_high = file_data[x].battle_range[y].high;
      r.xap_num[0] = file_data[x].xap_num[y][0];
      r.xap_num[1] = file_data[x].xap_num[y][1];
      r.xap_num[2] = file_data[x].xap_num[y][2];
      r.xap_chance[0] = file_data[x].xap_chance[y][0];
      r.xap_chance[1] = file_data[x].xap_chance[y][1];
      r.xap_chance[2] = file_data[x].xap_chance[y][2];
      r.percent_option = file_data[x].percent_option[y];
      r.sound = file_data[x].sound[y];
      r.text = file_data[x].text[y];
      data[x].random_rects.push_back(r);
    }
  }

  return data;
}

static void draw_random_rects(Image& map,
    const vector<RealmzScenarioData::RandomRect>& random_rects,
    size_t xpoff,
    size_t ypoff,
    bool is_dungeon,
    int16_t level_num,
    uint8_t x0, uint8_t y0, uint8_t w, uint8_t h) {

  size_t tile_size = is_dungeon ? 16 : 32;
  for (size_t z = 0; z < random_rects.size(); z++) {

    RealmzScenarioData::RandomRect rect = random_rects[z];
    // If the rect doesn't cover any tiles, skip it
    if (rect.left > rect.right || rect.top > rect.bottom) {
      continue;
    }

    // If the rect is completely outside of the drawing bounds, skip it
    if ((rect.right < x0) || (rect.bottom < y0) ||
        (rect.left > x0 + w) || (rect.top > y0 + h)) {
      continue;
    }

    // If the rect has no parameters set, skip it
    if (rect.top == 0 && rect.left == 0 && rect.bottom == 0 && rect.right == 0 &&
        rect.times_in_10k == 0 && rect.battle_low == 0 && rect.battle_high == 0 &&
        rect.xap_num[0] == 0 && rect.xap_num[1] == 0 && rect.xap_num[2] == 0 &&
        rect.xap_chance[0] == 0 && rect.xap_chance[1] == 0 && rect.xap_chance[2] == 0 &&
        rect.percent_option == 0 && rect.sound == 0 && rect.text == 0) {
      continue;
    }

    // If we get here, then the rect is nontrivial and is at least partially
    // within the render window, so we should draw it.

    // Clamp rect bounds to be within the render window
    if (rect.left < x0) {
      rect.left = x0;
    }
    if (rect.right > x0 + w - 1) {
      rect.right = x0 + w - 1;
    }
    if (rect.top < y0) {
      rect.top = y0;
    }
    if (rect.bottom > y0 + h - 1) {
      rect.bottom = y0 + h - 1;
    }

    ssize_t xp_left = (rect.left - x0) * tile_size + xpoff;
    ssize_t xp_right = (rect.right - x0) * tile_size + tile_size - 1 + xpoff;
    ssize_t yp_top = (rect.top - y0) * tile_size + ypoff;
    ssize_t yp_bottom = (rect.bottom - y0) * tile_size + tile_size - 1 + ypoff;

    ssize_t start_xx = (xp_left < 0) ? 0 : xp_left;
    ssize_t end_xx = (xp_right > static_cast<ssize_t>(map.get_width())) ? map.get_width() : xp_right;
    ssize_t start_yy = (yp_top < 0) ? 0 : yp_top;
    ssize_t end_yy = (yp_bottom > static_cast<ssize_t>(map.get_height())) ? map.get_height() : yp_bottom;
    for (ssize_t yy = start_yy; yy < end_yy; yy++) {
      for (ssize_t xx = start_xx; xx < end_xx; xx++) {

        uint64_t r = 0, g = 0, b = 0;
        map.read_pixel(xx, yy, &r, &g, &b);

        if (((xx + yy) / 8) & 1) {
          r = ((0xEF) * (uint32_t)r) / 0xFF;
          g = ((0xEF) * (uint32_t)g) / 0xFF;
          b = ((0xEF) * (uint32_t)b) / 0xFF;
        } else {
          r = (0xFF0 + 0xEF * (uint32_t)r) / 0xFF;
          g = (0xFF0 + 0xEF * (uint32_t)g) / 0xFF;
          b = (0xFF0 + 0xEF * (uint32_t)b) / 0xFF;
        }
        map.write_pixel(xx, yy, r, g, b);
      }
    }

    map.draw_horizontal_line(xp_left, xp_right, yp_top, 0, 0xFFFFFFFF);
    map.draw_horizontal_line(xp_left, xp_right, yp_bottom, 0, 0xFFFFFFFF);
    map.draw_vertical_line(xp_left, yp_top, yp_bottom, 0, 0xFFFFFFFF);
    map.draw_vertical_line(xp_right, yp_top, yp_bottom, 0, 0xFFFFFFFF);

    string rectinfo;
    if (rect.times_in_10k == -1) {
      rectinfo = string_printf("ENC XAP %d", rect.xap_num[0]);

    } else {
      rectinfo = string_printf("%d/10000", rect.times_in_10k);
      if (rect.battle_low || rect.battle_high) {
        rectinfo += string_printf(" b=[%d,%d]", rect.battle_low, rect.battle_high);
      }
      if (rect.percent_option) {
        rectinfo += string_printf(" o=%d%%", rect.percent_option);
      }
      if (rect.sound) {
        rectinfo += string_printf(" s=%d", rect.sound);
      }
      if (rect.text) {
        rectinfo += string_printf(" t=%d", rect.text);
      }
      for (size_t y = 0; y < 3; y++) {
        if (rect.xap_num[y] && rect.xap_chance[y]) {
          rectinfo += string_printf(
              " XAP%hd/%hd%%", rect.xap_num[y], rect.xap_chance[y]);
        }
      }
    }

    map.draw_text(
        xp_left + 2, yp_bottom - 8, NULL, NULL, 0xFFFFFFFF, 0x00000080,
        "%s", rectinfo.c_str());
    map.draw_text(
        xp_left + 2, yp_bottom - 16, NULL, NULL, 0xFFFFFFFF, 0x00000080,
        "%cRR%hd/%zu", is_dungeon ? 'D' : 'L', level_num, z);
  }
}

////////////////////////////////////////////////////////////////////////////////
// DATA DD

int8_t RealmzScenarioData::APInfo::get_x() const {
  if (this->location_code < 0) {
    return -1;
  }
  return this->location_code % 100;
}

int8_t RealmzScenarioData::APInfo::get_y() const {
  if (this->location_code < 0) {
    return -1;
  }
  return (this->location_code / 100) % 100;
}

int8_t RealmzScenarioData::APInfo::get_level_num() const {
  if (this->location_code < 0) {
    return -1;
  }
  return (this->location_code / 10000) % 100;
}

vector<vector<RealmzScenarioData::APInfo>> RealmzScenarioData::load_ap_index(
    const string& filename) {
  vector<APInfo> all_info = this->load_xap_index(filename);

  vector<vector<APInfo>> level_ap_info(all_info.size() / 100);
  for (size_t x = 0; x < all_info.size(); x++) {
    level_ap_info[x / 100].push_back(all_info[x]);
  }

  return level_ap_info;
}

vector<RealmzScenarioData::APInfo> RealmzScenarioData::load_xap_index(
    const string& filename) {
  return load_vector_file<APInfo>(filename);
}

enum class ReferenceType {
  NONE = 0,
  STRING,
  OPTION_STRING,
  XAP,
  ITEM,
  SPELL,
  SIMPLE_ENCOUNTER,
  COMPLEX_ENCOUNTER,
  TREASURE,
  BATTLE,
  SHOP,
};

struct OpcodeArgInfo {
  string arg_name;
  unordered_map<int16_t, string> value_names;
  string negative_modifier;
  ReferenceType ref_type;
};

struct OpcodeInfo {
  string name;
  string negative_name;
  bool always_use_ecodes;
  vector<OpcodeArgInfo> args;
};

// clang-format off
static const unordered_map<int16_t, string> race_names({
  {1, "human"}, {2, "shadow elf"}, {3, "elf"}, {4, "orc"}, {5, "furfoot"},
  {6, "gnome"}, {7, "dwarf"}, {8, "half elf"}, {9, "half orc"}, {10, "goblin"},
  {11, "hobgoblin"}, {12, "kobold"}, {13, "vampire"}, {14, "lizard man"},
  {15, "brownie"}, {16, "pixie"}, {17, "leprechaun"}, {18, "demon"},
  {19, "cathoon"}});

static const unordered_map<int16_t, string> party_condition_names({
  {0, "torch"}, {1, "waterworld"}, {2, "ogre_dragon_hide"},
  {3, "detect_secret"}, {4, "wizard_eye"}, {5, "search"},
  {6, "free_fall_levitate"}, {7, "sentry"}, {8, "charm_resist"}});

static const unordered_map<int16_t, string> char_condition_names({
  {0, "run_away"}, {1, "helpless"}, {2, "tangled"}, {3, "cursed"},
  {4, "magic_aura"}, {5, "stupid"}, {6, "slow"}, {7, "shield_from_hits"},
  {8, "shield_from_proj"}, {9, "poisoned"}, {10, "regenerating"},
  {11, "fire_protection"}, {12, "cold_protection"},
  {13, "electrical_protection"}, {14, "chemical_protection"},
  {15, "mental_protection"}, {16, "1st_level_protection"},
  {17, "2nd_level_protection"}, {18, "3rd_level_protection"},
  {19, "4th_level_protection"}, {20, "5th_level_protection"},
  {21, "strong"}, {22, "protection_from_evil"}, {23, "speedy"},
  {24, "invisible"}, {25, "animated"}, {26, "stoned"}, {27, "blind"},
  {28, "diseased"}, {29, "confused"}, {30, "reflecting_spells"},
  {31, "reflecting_attacks"}, {32, "attack_bonus"}, {33, "absorbing_energy"},
  {34, "energy_drain"}, {35, "absorbing_energy_from_attacks"},
  {36, "hindered_attack"}, {37, "hindered_defense"}, {38, "defense_bonus"},
  {39, "silenced"}});

static const unordered_map<int16_t, string> option_jump_target_value_names({
  {0, "back_up"}, {1, "xap"}, {2, "simple"}, {3, "complex"}, {4, "eliminate"}});

static const unordered_map<int16_t, string> jump_target_value_names({
  {0, "xap"}, {1, "simple"}, {2, "complex"}});

static const unordered_map<int16_t, string> jump_or_exit_actions({
  {1, "jump"}, {2, "exit_ap"}, {-2, "exit_ap_delete"}});

static const unordered_map<int16_t, string> land_dungeon_value_names({
  {0, "land"}, {1, "dungeon"}});

static const unordered_map<int16_t, OpcodeInfo> opcode_definitions({
  {  1, {"string", "", false, {
    {"", {}, "no_wait", ReferenceType::STRING},
  }}},

  {  2, {"battle", "", false, {
    {"low", {}, "surprise", ReferenceType::BATTLE},
    {"high", {}, "surprise", ReferenceType::BATTLE},
    {"sound_or_lose_xap", {}, "", ReferenceType::XAP},
    {"string", {}, "", ReferenceType::STRING},
    {"treasure_mode", {{0, "all"}, {5, "no_enemy"}, {10, "xap_on_lose"}}, "", ReferenceType::NONE},
  }}},

  {  3, {"option", "option_link", false, {
    {"continue_option", {{1, "yes"}, {2, "no"}}, "", ReferenceType::NONE},
    {"target_type", option_jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"left_prompt", {}, "", ReferenceType::OPTION_STRING},
    {"right_prompt", {}, "", ReferenceType::OPTION_STRING},
  }}},

  {  4, {"simple_enc", "", false, {
    {"", {}, "", ReferenceType::SIMPLE_ENCOUNTER},
  }}},

  {  5, {"complex_enc", "", false, {
    {"", {}, "", ReferenceType::COMPLEX_ENCOUNTER},
  }}},

  {  6, {"shop", "", false, {
    {"", {}, "auto_enter", ReferenceType::SHOP},
  }}},

  {  7, {"modify_ap", "", false, {
    {"level", {{-2, "simple"}, {-3, "complex"}}, "", ReferenceType::NONE},
    {"id", {}, "", ReferenceType::NONE},
    {"source_xap", {}, "", ReferenceType::XAP},
    {"level_type", {{0, "same"}, {1, "land"}, {2, "dungeon"}}, "", ReferenceType::NONE},
    {"result_code", {}, "", ReferenceType::NONE},
  }}},

  {  8, {"use_ap", "", false, {
    {"level", {}, "", ReferenceType::NONE},
    {"id", {}, "", ReferenceType::NONE},
  }}},

  {  9, {"sound", "", false, {
    {"", {}, "pause", ReferenceType::NONE},
  }}},

  { 10, {"treasure", "", false, {
    {"", {}, "", ReferenceType::TREASURE},
  }}},

  { 11, {"victory_points", "", false, {
    {"", {}, "", ReferenceType::NONE},
  }}},

  { 12, {"change_tile", "", false, {
    {"level", {}, "", ReferenceType::NONE},
    {"x", {}, "", ReferenceType::NONE},
    {"y", {}, "", ReferenceType::NONE},
    {"new_tile", {}, "", ReferenceType::NONE},
    {"level_type", {{0, "land"}, {1, "dungeon"}}, "", ReferenceType::NONE},
  }}},

  { 13, {"enable_ap", "", false, {
    {"level", {}, "", ReferenceType::NONE},
    {"id", {}, "", ReferenceType::NONE},
    {"percent_chance", {}, "", ReferenceType::NONE},
    {"low", {}, "dungeon", ReferenceType::NONE},
    {"high", {}, "dungeon", ReferenceType::NONE},
  }}},

  { 14, {"pick_chars", "", false, {
    {"", {}, "only_conscious", ReferenceType::NONE},
  }}},

  { 15, {"heal_picked", "", false, {
    {"mult", {}, "", ReferenceType::NONE},
    {"low_range", {}, "", ReferenceType::NONE},
    {"high_range", {}, "", ReferenceType::NONE},
    {"sound", {}, "", ReferenceType::NONE},
    {"string", {}, "", ReferenceType::STRING},
  }}},

  { 16, {"heal_party", "", false, {
    {"mult", {}, "", ReferenceType::NONE},
    {"low_range", {}, "", ReferenceType::NONE},
    {"high_range", {}, "", ReferenceType::NONE},
    {"sound", {}, "", ReferenceType::NONE},
    {"string", {}, "", ReferenceType::STRING},
  }}},

  { 17, {"spell_picked", "", false, {
    {"spell", {}, "", ReferenceType::SPELL},
    {"power", {}, "", ReferenceType::NONE},
    {"drv_modifier", {}, "", ReferenceType::NONE},
    {"can_drv", {{0, "yes"}, {1, "no"}}, "", ReferenceType::NONE},
  }}},

  { 18, {"spell_party", "", false, {
    {"spell", {}, "", ReferenceType::SPELL},
    {"power", {}, "", ReferenceType::NONE},
    {"drv_modifier", {}, "", ReferenceType::NONE},
    {"can_drv", {{0, "yes"}, {1, "no"}}, "", ReferenceType::NONE},
  }}},

  { 19, {"rand_string", "", false, {
    {"low", {}, "", ReferenceType::STRING},
    {"high", {}, "", ReferenceType::STRING},
  }}},

  { 20, {"tele_and_run", "", false, {
    {"level", {{-1, "same"}}, "", ReferenceType::NONE},
    {"x", {{-1, "same"}}, "", ReferenceType::NONE},
    {"y", {{-1, "same"}}, "", ReferenceType::NONE},
    {"sound", {}, "", ReferenceType::NONE},
    {"string", {}, "", ReferenceType::STRING},
  }}},

  { 21, {"jmp_if_item", "jmp_if_item_link", false, {
    {"item", {}, "", ReferenceType::ITEM},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"nonposs_action", {{0, "jump_other"}, {1, "continue"}, {2, "string_exit"}}, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"other_target", {}, "", ReferenceType::NONE},
  }}},

  { 22, {"change_item", "", false, {
    {"item", {}, "", ReferenceType::ITEM},
    {"num", {}, "", ReferenceType::NONE},
    {"action", {{1, "drop"}, {2, "charge"}, {3, "change_type"}}, "", ReferenceType::NONE},
    {"charges", {}, "", ReferenceType::NONE},
    {"new_item", {}, "", ReferenceType::ITEM},
  }}},

  { 23, {"change_rect", "change_rect_dungeon", false, {
    {"level", {}, "", ReferenceType::NONE},
    {"id", {}, "", ReferenceType::NONE},
    {"times_in_10k", {}, "", ReferenceType::NONE},
    {"new_battle_low", {{-1, "same"}}, "", ReferenceType::BATTLE},
    {"new_battle_high", {{-1, "same"}}, "", ReferenceType::BATTLE},
  }}},

  { 24, {"exit_ap", "", false, {}}},

  { 25, {"exit_ap_delete", "", false, {}}},

  { 26, {"mouse_click", "", false, {}}},

  { 27, {"picture", "", false, {
    {"", {}, "", ReferenceType::NONE},
  }}},

  { 28, {"redraw", "", false, {}}},

  { 29, {"give_map", "", false, {
    {"", {}, "auto_show", ReferenceType::NONE},
  }}},

  { 30, {"pick_ability", "", false, {
    {"ability", {}, "choose_failure", ReferenceType::NONE},
    {"success_mod", {}, "", ReferenceType::NONE},
    {"who", {{0, "picked"}, {1, "all"}, {2, "alive"}}, "", ReferenceType::NONE},
    {"what", {{0, "special"}, {1, "attribute"}}, "", ReferenceType::NONE},
  }}},

  { 31, {"jmp_ability", "jmp_ability_link", false, {
    {"ability", {}, "choose_failure", ReferenceType::NONE},
    {"success_mod", {}, "", ReferenceType::NONE},
    {"what", {{0, "special"}, {1, "attribute"}}, "", ReferenceType::NONE},
    {"success_xap", {}, "", ReferenceType::XAP},
    {"failure_xap", {}, "", ReferenceType::XAP},
  }}},

  { 32, {"temple", "", false, {
    {"inflation_percent", {}, "", ReferenceType::NONE},
  }}},

  { 33, {"take_money", "", false, {
    {"", {}, "gems", ReferenceType::NONE},
    {"action", {{0, "cont_if_poss"}, {1, "cont_if_not_poss"}, {2, "force"}, {-1, "jmp_back_if_not_poss"}}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"code_index", {}, "", ReferenceType::NONE},
  }}},

  { 34, {"break_enc", "", false, {}}},

  { 35, {"simple_enc_del", "", false, {
    {"", {}, "", ReferenceType::NONE},
  }}},

  { 36, {"stash_items", "", false, {
    {"", {{0, "restore"}, {1, "stash"}}, "", ReferenceType::NONE},
  }}},

  { 37, {"set_dungeon", "", false, {
    {"", {{0, "dungeon"}, {1, "land"}}, "", ReferenceType::NONE},
    {"level", {}, "", ReferenceType::NONE},
    {"x", {}, "", ReferenceType::NONE},
    {"y", {}, "", ReferenceType::NONE},
    {"dir", {{1, "north"}, {2, "east"}, {3, "south"}, {4, "west"}}, "", ReferenceType::NONE},
  }}},

  { 38, {"jmp_if_item_enc", "", false, {
    {"item", {}, "", ReferenceType::ITEM},
    {"continue", {{0, "if_poss"}, {1, "if_not_poss"}}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"code_index", {}, "", ReferenceType::NONE},
  }}},

  { 39, {"jmp_xap", "", false, {
    {"", {}, "", ReferenceType::XAP},
  }}},

  { 40, {"jmp_party_cond", "jmp_party_cond_link", false, {
    {"jmp_cond", {{1, "if_exists"}, {2, "if_not_exists"}}, "", ReferenceType::NONE},
    {"target_type", {{0, "none"}, {1, "xap"}, {1, "simple"}, {1, "complex"}}, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"condition", party_condition_names, "", ReferenceType::NONE},
  }}},

  { 41, {"simple_enc_del_any", "", false, {
    {"", {}, "", ReferenceType::SIMPLE_ENCOUNTER},
    {"choice", {}, "", ReferenceType::NONE},
  }}},

  { 42, {"jmp_random", "jmp_random_link", false, {
    {"percent_chance", {}, "", ReferenceType::NONE},
    {"action", jump_or_exit_actions, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"code_index", {}, "", ReferenceType::NONE},
  }}},

  { 43, {"give_cond", "", false, {
    {"who", {{0, "all"}, {1, "picked"}, {2, "alive"}}, "", ReferenceType::NONE},
    {"condition", char_condition_names, "", ReferenceType::NONE},
    {"duration", {}, "", ReferenceType::NONE},
    {"sound", {}, "", ReferenceType::NONE},
  }}},

  { 44, {"complex_enc_del", "", false, {
    {"", {}, "", ReferenceType::COMPLEX_ENCOUNTER},
  }}},

  { 45, {"tele", "", false, {
    {"level", {{-1, "same"}}, "", ReferenceType::NONE},
    {"x", {{-1, "same"}}, "", ReferenceType::NONE},
    {"y", {{-1, "same"}}, "", ReferenceType::NONE},
    {"sound", {}, "", ReferenceType::NONE},
  }}},

  { 46, {"jmp_quest", "jmp_quest_link", false, {
    {"", {}, "", ReferenceType::NONE},
    {"check", {{0, "set"}, {1, "not_set"}}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"code_index", {}, "", ReferenceType::STRING},
  }}},

  { 47, {"set_quest", "", false, {
    {"", {}, "clear", ReferenceType::NONE},
  }}},

  { 48, {"pick_battle", "", false, {
    {"low", {}, "", ReferenceType::BATTLE},
    {"high", {}, "", ReferenceType::BATTLE},
    {"sound", {}, "", ReferenceType::NONE},
    {"string", {}, "", ReferenceType::STRING},
    {"treasure", {}, "", ReferenceType::TREASURE},
  }}},

  { 49, {"bank", "", false, {}}},

  { 50, {"pick_attribute", "", false, {
    {"type", {{0, "race"}, {1, "gender"}, {2, "caste"}, {3, "rase_class"}, {4, "caste_class"}}, "", ReferenceType::NONE},
    {"gender", {{1, "male"}, {2, "female"}}, "", ReferenceType::NONE},
    {"race_caste", {}, "", ReferenceType::NONE},
    {"race_caste_class", {}, "", ReferenceType::NONE},
    {"who", {{0, "all"}, {1, "alive"}}, "", ReferenceType::NONE},
  }}},

  { 51, {"change_shop", "", false, {
    {"", {}, "", ReferenceType::NONE},
    {"inflation_percent_change", {}, "", ReferenceType::NONE},
    {"item_id", {}, "", ReferenceType::ITEM},
    {"item_count", {}, "", ReferenceType::NONE},
  }}},

  { 52, {"pick_misc", "", false, {
    {"type", {{0, "move"}, {1, "position"}, {2, "item_poss"}, {3, "pct_chance"}, {4, "save_vs_attr"}, {5, "save_vs_spell_type"}, {6, "currently_selected"}, {7, "item_equipped"}, {8, "party_position"}}, "", ReferenceType::NONE},
    // TODO: parameter should have ReferenceType::ITEM if type is 2 or 7
    {"parameter", {}, "", ReferenceType::NONE},
    {"who", {{0, "all"}, {1, "alive"}, {2, "picked"}}, "", ReferenceType::NONE},
  }}},

  { 53, {"pick_caste", "", false, {
    {"caste", {}, "", ReferenceType::NONE},
    {"caste_type", {{1, "fighter"}, {2, "magical"}, {3, "monk_rogue"}}, "", ReferenceType::NONE},
    {"who", {{0, "all"}, {1, "alive"}, {2, "picked"}}, "", ReferenceType::NONE},
  }}},

  { 54, {"change_time_enc", "", false, {
    {"", {}, "", ReferenceType::NONE},
    {"percent_chance", {{-1, "same"}}, "", ReferenceType::NONE},
    {"new_day_incr", {{-1, "same"}}, "", ReferenceType::NONE},
    {"reset_to_current", {{0, "no"}, {1, "yes"}}, "", ReferenceType::NONE},
    {"days_to_next_instance", {{-1, "same"}}, "", ReferenceType::NONE},
  }}},

  { 55, {"jmp_picked", "jmp_picked_link", false, {
    {"pc_id", {{0, "any"}}, "", ReferenceType::NONE},
    {"fail_action", {{0, "exit_ap"}, {1, "xap"}, {2, "string_exit"}}, "", ReferenceType::NONE},
    {"unused", {}, "", ReferenceType::NONE},
    {"success_xap", {}, "", ReferenceType::XAP},
    {"failure_parameter", {}, "", ReferenceType::NONE},
  }}},

  { 56, {"jmp_battle", "jmp_battle_link", false, {
    {"battle_low", {}, "", ReferenceType::BATTLE},
    {"battle_high", {}, "", ReferenceType::BATTLE},
    {"loss_xap", {{-1, "back_up"}}, "", ReferenceType::XAP},
    {"sound", {}, "", ReferenceType::NONE},
    {"string", {}, "", ReferenceType::STRING},
  }}},

  { 57, {"change_tileset", "", false, {
    {"new_tileset", {}, "", ReferenceType::NONE},
    {"dark", {{0, "no"}, {1, "yes"}}, "", ReferenceType::NONE},
    {"level", {}, "", ReferenceType::NONE},
  }}},

  { 58, {"jmp_difficulty", "jmp_difficulty_link", false, {
    {"difficulty", {{1, "novice"}, {2, "easy"}, {3, "normal"}, {4, "hard"}, {5, "veteran"}}, "", ReferenceType::NONE},
    {"action", jump_or_exit_actions, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"code_index", {}, "", ReferenceType::NONE},
  }}},

  { 59, {"jmp_tile", "jmp_tile_link", false, {
    {"tile", {}, "", ReferenceType::NONE},
    {"action", jump_or_exit_actions, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"code_index", {}, "", ReferenceType::NONE},
  }}},

  { 60, {"drop_all_money", "", false, {
    {"type", {{1, "gold"}, {2, "gems"}, {3, "jewelry"}}, "", ReferenceType::NONE},
    {"who", {{0, "all"}, {1, "picked"}}, "", ReferenceType::NONE},
  }}},

  { 61, {"incr_party_loc", "", false, {
    {"unused", {}, "", ReferenceType::NONE},
    {"x", {}, "", ReferenceType::NONE},
    {"y", {}, "", ReferenceType::NONE},
    {"move_type", {{0, "exact"}, {1, "random"}}, "", ReferenceType::NONE},
  }}},

  { 62, {"story", "", false, {
    {"", {}, "", ReferenceType::NONE},
  }}},

  { 63, {"change_time", "", false, {
    {"base", {{1, "absolute"}, {2, "relative"}}, "", ReferenceType::NONE},
    {"days", {{-1, "same"}}, "", ReferenceType::NONE},
    {"hours", {{-1, "same"}}, "", ReferenceType::NONE},
    {"minutes", {{-1, "same"}}, "", ReferenceType::NONE},
  }}},

  { 64, {"jmp_time", "jmp_time_link", false, {
    {"day", {{-1, "any"}}, "", ReferenceType::NONE},
    {"hour", {{-1, "any"}}, "", ReferenceType::NONE},
    {"unused", {}, "", ReferenceType::NONE},
    {"before_equal_xap", {}, "", ReferenceType::XAP},
    {"after_xap", {}, "", ReferenceType::XAP},
  }}},

  { 65, {"give_rand_item", "", false, {
    {"count", {}, "random", ReferenceType::NONE},
    {"item_low", {}, "", ReferenceType::ITEM},
    {"item_high", {}, "", ReferenceType::ITEM},
  }}},

  { 66, {"allow_camping", "", false, {
    {"", {{0, "enable"}, {1, "disable"}}, "", ReferenceType::NONE},
  }}},

  { 67, {"jmp_item_charge", "jmp_item_charge_link", false, {
    {"", {}, "", ReferenceType::ITEM},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"min_charges", {}, "", ReferenceType::NONE},
    {"target_if_enough", {{-1, "continue"}}, "", ReferenceType::NONE},
    {"target_if_not_enough", {{-1, "continue"}}, "", ReferenceType::NONE},
  }}},

  { 68, {"change_fatigue", "", false, {
    {"", {{1, "set_full"}, {2, "set_empty"}, {3, "modify"}}, "", ReferenceType::NONE},
    {"factor_percent", {}, "", ReferenceType::NONE},
  }}},

  { 69, {"change_casting_flags", "", false, {
    {"enable_char_casting", {{0, "yes"}, {1, "no"}}, "", ReferenceType::NONE},
    {"enable_npc_casting", {{0, "yes"}, {1, "no"}}, "", ReferenceType::NONE},
    {"enable_recharging", {{0, "yes"}, {1, "no"}}, "", ReferenceType::NONE},
    // Note: apparently e-code 4 isn't used and 5 must always be 1. We don't
    // enforce this for a disassembly though
  }}},

  { 70, {"save_restore_loc", "", true, {
    {"", {{1, "save"}, {2, "restore"}}, "", ReferenceType::NONE},
  }}},

  { 71, {"enable_coord_display", "", false, {
    {"", {{0, "enable"}, {1, "disable"}}, "", ReferenceType::NONE},
  }}},

  { 72, {"jmp_quest_range", "jmp_quest_range_link", false, {
    {"quest_low", {}, "", ReferenceType::NONE},
    {"quest_high", {}, "", ReferenceType::NONE},
    {"unused", {}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
  }}},

  { 73, {"shop_restrict", "", false, {
    {"", {}, "auto_enter", ReferenceType::SHOP},
    {"item_low1", {}, "", ReferenceType::ITEM},
    {"item_high1", {}, "", ReferenceType::ITEM},
    {"item_low2", {}, "", ReferenceType::ITEM},
    {"item_high2", {}, "", ReferenceType::ITEM},
  }}},

  { 74, {"give_spell_pts_picked", "", false, {
    {"mult", {}, "", ReferenceType::NONE},
    {"pts_low", {}, "", ReferenceType::NONE},
    {"pts_high", {}, "", ReferenceType::NONE},
  }}},

  { 75, {"jmp_spell_pts", "jmp_spell_pts_link", false, {
    {"who", {{1, "picked"}, {2, "alive"}}, "", ReferenceType::NONE},
    {"min_pts", {}, "", ReferenceType::NONE},
    {"fail_action", {{0, "continue"}, {1, "exit_ap"}}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
  }}},

  { 76, {"incr_quest_value", "", false, {
    {"", {}, "", ReferenceType::NONE},
    {"incr", {}, "", ReferenceType::NONE},
    {"target_type", {{0, "none"}, {1, "xap"}, {2, "simple"}, {3, "complex"}}, "", ReferenceType::NONE},
    {"jump_min_value", {}, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
  }}},

  { 77, {"jmp_quest_value", "jmp_quest_value_link", false, {
    {"", {}, "", ReferenceType::NONE},
    {"value", {}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target_less", {{0, "continue"}}, "", ReferenceType::NONE},
    {"target_equal_greater", {{0, "continue"}}, "", ReferenceType::NONE},
  }}},

  { 78, {"jmp_tile_params", "jmp_tile_params_link", false, {
    {"attr", {{1, "shoreline"}, {2, "is_needs_boat"}, {3, "path"}, {4, "blocks_los"}, {5, "need_fly_float"}, {6, "special"}, {7, "tile_id"}}, "", ReferenceType::NONE},
    {"tile_id", {}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target_false", {{0, "continue"}}, "", ReferenceType::NONE},
    {"target_true", {{0, "continue"}}, "", ReferenceType::NONE},
  }}},

  { 81, {"jmp_char_cond", "jmp_char_cond_link", false, {
    {"cond", {}, "", ReferenceType::NONE},
    {"who", {{-1, "picked"}, {0, "party"}}, "", ReferenceType::NONE},
    {"fail_string", {}, "", ReferenceType::STRING},
    {"success_xap", {}, "", ReferenceType::XAP},
    {"failure_xap", {}, "", ReferenceType::XAP},
  }}},

  { 82, {"enable_turning", "", false, {}}},

  { 83, {"disable_turning", "", false, {}}},

  { 84, {"check_scen_registered", "", false, {}}},

  { 85, {"jmp_random_xap", "jmp_random_xap_link", false, {
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target_low", {}, "", ReferenceType::XAP},
    {"target_high", {}, "", ReferenceType::XAP},
    {"sound", {}, "", ReferenceType::NONE},
    {"string", {}, "", ReferenceType::STRING},
  }}},

  { 86, {"jmp_misc", "jmp_misc_link", false, {
    {"", {{0, "caste_present"}, {1, "race_present"}, {2, "gender_present"}, {3, "in_boat"}, {4, "camping"}, {5, "caste_class_present"}, {6, "race_class_present"}, {7, "total_party_levels"}, {8, "picked_char_levels"}}, "", ReferenceType::NONE},
    {"value", {}, "picked_only", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "", ReferenceType::NONE},
    {"target_true", {{0, "continue"}}, "", ReferenceType::NONE},
    {"target_false", {{0, "continue"}}, "", ReferenceType::NONE},
  }}},

  { 87, {"jmp_npc", "jmp_npc_link", false, {
    {"", {}, "", ReferenceType::NONE},
    {"target_type", jump_target_value_names, "picked_only", ReferenceType::NONE},
    {"fail_action", {{0, "jmp_other"}, {1, "continue"}, {2, "string_exit"}}, "", ReferenceType::NONE},
    {"target", {}, "", ReferenceType::NONE},
    {"other_param", {}, "", ReferenceType::NONE},
  }}},

  { 88, {"drop_npc", "", false, {
    {"", {}, "", ReferenceType::NONE},
  }}},

  { 89, {"add_npc", "", false, {
    {"", {}, "", ReferenceType::NONE},
  }}},

  { 90, {"take_victory_pts", "", false, {
    {"", {}, "", ReferenceType::NONE},
    {"who", {{0, "each"}, {1, "picked"}, {2, "total"}}, "", ReferenceType::NONE},
  }}},

  { 91, {"drop_all_items", "", false, {}}},

  { 92, {"change_rect_size", "", false, {
    {"level", {}, "", ReferenceType::NONE},
    {"rect", {}, "", ReferenceType::NONE},
    {"level_type", {{0, "land"}, {1, "dungeon"}}, "", ReferenceType::NONE},
    {"times_in_10k_mult", {}, "", ReferenceType::NONE},
    {"action", {{-1, "none"}, {0, "set_coords"}, {1, "offset"}, {2, "resize"}, {3, "warp"}}, "", ReferenceType::NONE},
    {"left_h", {}, "", ReferenceType::NONE},
    {"right_v", {}, "", ReferenceType::NONE},
    {"top", {}, "", ReferenceType::NONE},
    {"bottom", {}, "", ReferenceType::NONE},
  }}},

  { 93, {"enable_compass", "", false, {}}},

  { 94, {"disable_compass", "", false, {}}},

  { 95, {"change_dir", "", false, {
    {"", {{-1, "random"}, {1, "north"}, {2, "east"}, {3, "south"}, {4, "west"}}, "", ReferenceType::NONE},
  }}},

  { 96, {"disable_dungeon_map", "", false, {}}},

  { 97, {"enable_dungeon_map", "", false, {}}},

  { 98, {"require_registration", "", false, {}}},

  { 99, {"get_registration", "", false, {}}},

  {100, {"end_battle", "", false, {}}},

  {101, {"back_up", "", false, {}}},

  {102, {"level_up_picked", "", false, {}}},

  {103, {"cont_boat_camping", "", false, {
    {"if_boat", {{1, "true"}, {2, "false"}}, "", ReferenceType::NONE},
    {"if_camping", {{1, "true"}, {2, "false"}}, "", ReferenceType::NONE},
    {"set_boat", {{1, "true"}, {2, "false"}}, "", ReferenceType::NONE},
  }}},

  {104, {"enable_random_battles", "", false, {
    {"", {{0, "false"}, {1, "true"}}, "", ReferenceType::NONE},
  }}},

  {105, {"enable_allies", "", false, {
    {"", {{1, "false"}, {2, "true"}}, "", ReferenceType::NONE},
  }}},

  {106, {"set_dark_los", "", false, {
    {"dark", {{1, "false"}, {2, "true"}}, "", ReferenceType::NONE},
    {"skip_if_dark_same", {{0, "false"}, {1, "true"}}, "", ReferenceType::NONE},
    {"los", {{1, "true"}, {2, "false"}}, "", ReferenceType::NONE},
    {"skip_if_los_same", {{0, "false"}, {1, "true"}}, "", ReferenceType::NONE},
  }}},

  {107, {"pick_battle_2", "", false, {
    {"battle_low", {}, "", ReferenceType::BATTLE},
    {"battle_high", {}, "", ReferenceType::BATTLE},
    {"sound", {}, "", ReferenceType::NONE},
    {"loss_xap", {}, "", ReferenceType::XAP},
  }}},

  {108, {"change_picked", "", false, {
    {"what", {{1, "attacks_round"}, {2, "spells_round"}, {3, "movement"}, {4, "damage"}, {5, "spell_pts"}, {6, "hand_to_hand"}, {7, "stamina"}, {8, "armor_rating"}, {9, "to_hit"}, {10, "missile_adjust"}, {11, "magic_resistance"}, {12, "prestige"}}, "", ReferenceType::NONE},
    {"count", {}, "", ReferenceType::NONE},
  }}},

  {111, {"ret", "", false, {}}},

  {112, {"pop", "", false, {}}},

  {119, {"revive_npc_after", "", false, {}}},

  {120, {"change_monster", "", false, {
    {"", {{1, "npc"}, {2, "monster"}}, "", ReferenceType::NONE},
    {"", {}, "", ReferenceType::NONE},
    {"count", {}, "", ReferenceType::NONE},
    {"new_icon", {}, "", ReferenceType::NONE},
    {"new_traitor", {{-1, "same"}}, "", ReferenceType::NONE},
  }}},

  {121, {"kill_lower_undead", "", false, {}}},

  {122, {"fumble_weapon", "", false, {
    {"string", {}, "", ReferenceType::STRING},
    {"sound", {}, "", ReferenceType::NONE},
  }}},

  {123, {"rout_monsters", "", false, {
    {"", {}, "", ReferenceType::NONE},
    {"", {}, "", ReferenceType::NONE},
    {"", {}, "", ReferenceType::NONE},
    {"", {}, "", ReferenceType::NONE},
    {"", {}, "", ReferenceType::NONE},
  }}},

  {124, {"summon_monsters", "", false, {
    {"type", {{0, "individual"}}, "", ReferenceType::NONE},
    {"", {}, "", ReferenceType::NONE},
    {"count", {}, "", ReferenceType::NONE},
    {"sound", {}, "", ReferenceType::NONE},
  }}},

  {125, {"destroy_related", "", false, {
    {"", {}, "", ReferenceType::NONE},
    {"count", {{0, "all"}}, "", ReferenceType::NONE},
    {"unused", {}, "", ReferenceType::NONE},
    {"unused", {}, "", ReferenceType::NONE},
    {"force", {{0, "false"}, {1, "true"}}, "", ReferenceType::NONE},
  }}},

  {126, {"macro_criteria", "", false, {
    {"when", {{0, "round_number"}, {1, "percent_chance"}, {2, "flee_fail"}}, "", ReferenceType::NONE},
    {"round_percent_chance", {}, "", ReferenceType::NONE},
    {"repeat", {{0, "none"}, {1, "each_round"}, {2, "jmp_random"}}, "", ReferenceType::NONE},
    {"xap_low", {}, "", ReferenceType::XAP},
    {"xap_high", {}, "", ReferenceType::XAP},
  }}},

  {127, {"cont_monster_present", "", false, {
    {"", {}, "", ReferenceType::NONE},
  }}},
});
// clang-format on

string RealmzScenarioData::disassemble_opcode(int16_t ap_code, int16_t arg_code) {
  int16_t opcode = abs(ap_code);
  if (opcode_definitions.count(opcode) == 0) {
    size_t ecodes_id = abs(arg_code);
    if (ecodes_id >= this->ecodes.size()) {
      return string_printf("[%hd %hd]", ap_code, arg_code);
    }
    return string_printf("[%hd %hd [%hd %hd %hd %hd %hd]]", ap_code, arg_code,
        this->ecodes[ecodes_id].data[0].load(),
        this->ecodes[ecodes_id].data[1].load(),
        this->ecodes[ecodes_id].data[2].load(),
        this->ecodes[ecodes_id].data[3].load(),
        this->ecodes[ecodes_id].data[4].load());
  }

  OpcodeInfo op = opcode_definitions.at(opcode);
  string op_name = (ap_code < 0 ? op.negative_name : op.name);
  if (op.args.size() == 0) {
    return op_name;
  }

  vector<int16_t> arguments;
  if (op.args.size() == 1 && !op.always_use_ecodes) {
    arguments.push_back(arg_code);

  } else {
    if (arg_code < 0) {
      op_name = op.negative_name;
      arg_code *= -1;
    }

    if ((size_t)arg_code >= ecodes.size()) {
      return string_printf("%-24s [bad ecode id %04X]", op_name.c_str(), arg_code);
    }
    if ((op.args.size() > 5) && ((size_t)arg_code >= ecodes.size() - 1)) {
      return string_printf("%-24s [bad 2-ecode id %04X]", op_name.c_str(), arg_code);
    }

    for (size_t x = 0; x < op.args.size(); x++) {
      arguments.push_back(ecodes[arg_code].data[x]); // Intentional overflow (x)
    }
  }

  string ret = string_printf("%-24s ", op_name.c_str());
  for (size_t x = 0; x < arguments.size(); x++) {
    if (x > 0) {
      ret += ", ";
    }

    if (!op.args[x].arg_name.empty()) {
      ret += (op.args[x].arg_name + "=");
    }

    int16_t value = arguments[x];
    bool use_negative_modifier = false;
    if (value < 0 && !op.args[x].negative_modifier.empty()) {
      use_negative_modifier = true;
      value *= -1;
    }

    switch (op.args[x].ref_type) {
      case ReferenceType::NONE:
        if (op.args[x].value_names.count(value)) {
          ret += string_printf("%hd(%s)", value, op.args[x].value_names.at(value).c_str());
        } else {
          ret += string_printf("%hd", value);
        }
        break;
      case ReferenceType::STRING:
        ret += render_string_reference(this->strings, value);
        break;
      case ReferenceType::OPTION_STRING:
        // Guess: if the scenario has any option strings at all, use them;
        // otherwise, use the normal string index?
        ret += render_string_reference(
            this->option_strings.empty() ? this->strings : this->option_strings, value);
        break;
      case ReferenceType::XAP:
        ret += string_printf("XAP%hd", value);
        break;
      case ReferenceType::ITEM:
        ret += this->desc_for_item(value);
        break;
      case ReferenceType::SPELL:
        ret += this->desc_for_spell(value);
        break;
      case ReferenceType::SIMPLE_ENCOUNTER:
        ret += string_printf("SEC%hd", value);
        break;
      case ReferenceType::COMPLEX_ENCOUNTER:
        ret += string_printf("CEC%hd", value);
        break;
      case ReferenceType::TREASURE:
        ret += string_printf("TSR%hd", value);
        break;
      case ReferenceType::SHOP:
        ret += string_printf("SHP%hd", value);
        break;
      case ReferenceType::BATTLE:
        ret += string_printf("BTL%hd", value);
        break;
      default:
        throw logic_error("invalid reference type");
    }

    if (use_negative_modifier) {
      ret += (", " + op.args[x].negative_modifier);
    }
  }

  return ret;
}

string RealmzScenarioData::disassemble_xap(int16_t ap_num) {
  const auto& ap = this->xaps.at(ap_num);

  string data = string_printf("===== XAP id=%d [XAP%d]\n", ap_num, ap_num);

  // TODO: eliminate code duplication here
  for (size_t x = 0; x < land_metadata.size(); x++) {
    for (size_t y = 0; y < land_metadata[x].random_rects.size(); y++) {
      const auto& r = land_metadata[x].random_rects[y];
      if (r.xap_num[0] == ap_num ||
          r.xap_num[1] == ap_num ||
          r.xap_num[2] == ap_num) {
        data += string_printf("RANDOM RECTANGLE REFERENCE land_level=%zu rect_num=%zu start_coord=%d,%d end_coord=%d,%d [LRR%zu/%zu]\n",
            x, y, r.left, r.top, r.right, r.bottom, x, y);
      }
    }
  }
  for (size_t x = 0; x < dungeon_metadata.size(); x++) {
    for (size_t y = 0; y < dungeon_metadata[x].random_rects.size(); y++) {
      const auto& r = dungeon_metadata[x].random_rects[y];
      if (r.xap_num[0] == ap_num ||
          r.xap_num[1] == ap_num ||
          r.xap_num[2] == ap_num) {
        data += string_printf("RANDOM RECTANGLE REFERENCE dungeon_level=%zu rect_num=%zu start_coord=%d,%d end_coord=%d,%d [DRR%zu/%zu]\n",
            x, y, r.left, r.top, r.right, r.bottom, x, y);
      }
    }
  }

  for (size_t x = 0; x < 8; x++) {
    if (ap.command_codes[x] || ap.argument_codes[x]) {
      string dasm = disassemble_opcode(ap.command_codes[x], ap.argument_codes[x]);
      data += string_printf("  %s\n", dasm.c_str());
    }
  }

  return data;
}

string RealmzScenarioData::disassemble_all_xaps() {
  deque<string> blocks;
  for (size_t x = 0; x < this->xaps.size(); x++) {
    blocks.emplace_back(this->disassemble_xap(x));
  }
  return join(blocks, "");
}

string RealmzScenarioData::disassemble_level_ap(
    int16_t level_num, int16_t ap_num, bool dungeon) {
  const auto& ap = (dungeon ? this->dungeon_aps : this->land_aps).at(level_num).at(ap_num);

  if (ap.get_x() < 0 || ap.get_y() < 0) {
    return "";
  }

  string extra;
  if (ap.to_level != level_num || ap.to_x != ap.get_x() || ap.to_y != ap.get_y()) {
    extra = string_printf(" to_level=%d to_x=%d to_y=%d", ap.to_level, ap.to_x,
        ap.to_y);
  }
  if (ap.percent_chance != 100) {
    extra += string_printf(" prob=%d", ap.percent_chance);
  }
  string data = string_printf("===== %s AP level=%d id=%d x=%d y=%d%s [%cAP%d/%d]\n",
      (dungeon ? "DUNGEON" : "LAND"), level_num, ap_num, ap.get_x(), ap.get_y(),
      extra.c_str(), (dungeon ? 'D' : 'L'), level_num, ap_num);

  for (size_t x = 0; x < 8; x++) {
    if (ap.command_codes[x] || ap.argument_codes[x]) {
      string dasm = this->disassemble_opcode(ap.command_codes[x], ap.argument_codes[x]);
      data += string_printf("  %s\n", dasm.c_str());
    }
  }

  return data;
}

string RealmzScenarioData::disassemble_level_aps(int16_t level_num, bool dungeon) {
  string ret;
  size_t count = (dungeon ? this->dungeon_aps : this->land_aps).at(level_num).size();
  for (size_t x = 0; x < count; x++) {
    ret += this->disassemble_level_ap(level_num, x, dungeon);
  }
  return ret;
}

string RealmzScenarioData::disassemble_all_level_aps(bool dungeon) {
  deque<string> blocks;
  size_t count = (dungeon ? this->dungeon_aps : this->land_aps).size();
  for (size_t x = 0; x < count; x++) {
    blocks.emplace_back(this->disassemble_level_aps(x, dungeon));
  }
  return join(blocks, "");
}

////////////////////////////////////////////////////////////////////////////////
// DATA DL

static uint16_t location_sig(uint8_t x, uint8_t y) {
  return ((uint16_t)x << 8) | y;
}

void RealmzScenarioData::MapData::transpose() {
  for (size_t y = 0; y < 90; y++) {
    for (size_t x = y + 1; x < 90; x++) {
      int16_t t = this->data[y][x];
      this->data[y][x] = this->data[x][y];
      this->data[x][y] = t;
    }
  }
}

vector<RealmzScenarioData::MapData> RealmzScenarioData::load_dungeon_map_index(
    const string& filename) {
  return load_vector_file<MapData>(filename);
}

string RealmzScenarioData::generate_dungeon_map_text(int16_t level_num) {
  const auto& mdata = this->dungeon_maps.at(level_num);
  deque<string> lines;
  for (ssize_t y = 0; y < 90; y++) {
    deque<string> line_tokens;
    for (ssize_t x = 0; x < 90; x++) {
      line_tokens.emplace_back(string_printf("%4hd", mdata.data[y][x].load()));
    }
    lines.emplace_back(join(line_tokens, ", "));
  }
  return join(lines, "\n");
}

Image RealmzScenarioData::generate_dungeon_map(int16_t level_num, uint8_t x0,
    uint8_t y0, uint8_t w, uint8_t h) {
  const auto& mdata = this->dungeon_maps.at(level_num);
  const auto& metadata = this->dungeon_metadata.at(level_num);
  const auto& aps = this->dungeon_aps.at(level_num);

  constexpr uint16_t wall_tile_flag = 0x0001;
  constexpr uint16_t vert_door_tile_flag = 0x0002;
  constexpr uint16_t horiz_door_tile_flag = 0x0004;
  constexpr uint16_t stairs_tile_flag = 0x0008;
  constexpr uint16_t columns_tile_flag = 0x0010;
  // constexpr uint16_t unmapped_tile_flag = 0x0080;
  constexpr uint16_t secret_up_tile_flag = 0x0100;
  constexpr uint16_t secret_right_tile_flag = 0x0200;
  constexpr uint16_t secret_down_tile_flag = 0x0400;
  constexpr uint16_t secret_left_tile_flag = 0x0800;
  constexpr uint16_t has_ap_tile_flag = 0x1000;
  constexpr uint16_t battle_blank_tile_flag = 0x2000;

  if ((x0 >= 90) || (y0 >= 90) || ((x0 + w) > 90) || ((y0 + h) > 90)) {
    throw runtime_error("map bounds out of range");
  }

  Image map(w * 16, h * 16);
  size_t pattern_x = 576, pattern_y = 320;

  unordered_map<uint16_t, vector<int>> loc_to_ap_nums;
  for (size_t x = 0; x < aps.size(); x++) {
    loc_to_ap_nums[location_sig(aps[x].get_x(), aps[x].get_y())].push_back(x);
  }

  Image dungeon_pattern = this->global.global_rsf.decode_PICT(302).image;

  for (ssize_t y = y0 + h - 1; y >= y0; y--) {
    for (ssize_t x = x0 + w - 1; x >= x0; x--) {
      int16_t data = mdata.data[y][x];

      size_t xp = (x - x0) * 16;
      size_t yp = (y - y0) * 16;
      map.fill_rect(xp, yp, 16, 16, 0x000000FF);
      if (data & wall_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 0,
            pattern_y + 0, 0xFFFFFFFF);
      }
      if (data & vert_door_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 16,
            pattern_y + 0, 0xFFFFFFFF);
      }
      if (data & horiz_door_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 32,
            pattern_y + 0, 0xFFFFFFFF);
      }
      if (data & stairs_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 48,
            pattern_y + 0, 0xFFFFFFFF);
      }
      if (data & columns_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 0,
            pattern_y + 16, 0xFFFFFFFF);
      }
      if (data & secret_up_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 0,
            pattern_y + 32, 0xFFFFFFFF);
      }
      if (data & secret_right_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 16,
            pattern_y + 32, 0xFFFFFFFF);
      }
      if (data & secret_down_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 32,
            pattern_y + 32, 0xFFFFFFFF);
      }
      if (data & secret_left_tile_flag) {
        map.mask_blit(dungeon_pattern, xp, yp, 16, 16, pattern_x + 48,
            pattern_y + 32, 0xFFFFFFFF);
      }

      if (data & has_ap_tile_flag) {
        map.draw_horizontal_line(xp, xp + 15, yp, 0, 0xFF0000FF);
        map.draw_horizontal_line(xp, xp + 15, yp + 15, 0, 0xFF0000FF);
        map.draw_vertical_line(xp, yp, yp + 15, 0, 0xFF0000FF);
        map.draw_vertical_line(xp + 15, yp, yp + 15, 0, 0xFF0000FF);
      }
      if (data & battle_blank_tile_flag) {
        map.draw_horizontal_line(xp, xp + 15, yp + 7, 0, 0x00FFFFFF);
        map.draw_horizontal_line(xp, xp + 15, yp + 8, 0, 0x00FFFFFF);
        map.draw_vertical_line(xp + 7, yp, yp + 15, 0, 0x00FFFFFF);
        map.draw_vertical_line(xp + 8, yp, yp + 15, 0, 0x00FFFFFF);
      }

      size_t text_xp = xp + 1;
      size_t text_yp = yp + 1;

      // Draw the coords if both are multiples of 10
      if (y % 10 == 0 && x % 10 == 0) {
        map.draw_text(text_xp, text_yp, 0xFF00FFFF, 0x00000080, "%zu,%zu", x, y);
        text_yp += 8;
      }

      // TODO: we intentionally don't include the DAP%d token here because
      // dungeon tiles are only 16x16, which really only leaves room for two
      // digits. We could fix this by scaling up the tileset to 32x32, but I'm
      // lazy.
      for (const auto& ap_num : loc_to_ap_nums[location_sig(x, y)]) {
        if (aps[ap_num].percent_chance < 100) {
          map.draw_text(text_xp, text_yp, 0xFFFFFFFF, 0x00000080, "%hd/%d-%hhu%%",
              level_num, ap_num, aps[ap_num].percent_chance);
        } else {
          map.draw_text(text_xp, text_yp, 0xFFFFFFFF, 0x00000080, "%hd/%d",
              level_num, ap_num);
        }
        text_yp += 8;
      }
    }
  }

  // Finally, draw random rects
  draw_random_rects(map, metadata.random_rects, 0, 0, true, level_num,
      x0, y0, w, h);

  return map;
}

////////////////////////////////////////////////////////////////////////////////
// DATA LD

vector<RealmzScenarioData::MapData> RealmzScenarioData::load_land_map_index(
    const string& filename) {
  // Format is the same as for dungeons, except it's in column-major order
  vector<MapData> data = load_dungeon_map_index(filename);
  for (auto& m : data) {
    m.transpose();
  }

  return data;
}

unordered_set<string> RealmzScenarioData::all_land_types() {
  unordered_set<string> all;
  for (const auto& it : this->land_type_to_tileset_definition) {
    all.emplace(it.first);
  }
  for (const auto& it : this->global.land_type_to_tileset_definition) {
    all.emplace(it.first);
  }
  return all;
}

string RealmzScenarioData::generate_land_map_text(int16_t level_num) {
  const auto& mdata = this->land_maps.at(level_num);
  deque<string> lines;
  for (ssize_t y = 0; y < 90; y++) {
    deque<string> line_tokens;
    for (ssize_t x = 0; x < 90; x++) {
      line_tokens.emplace_back(string_printf("%4hd", mdata.data[y][x].load()));
    }
    lines.emplace_back(join(line_tokens, ", "));
  }
  return join(lines, "\n");
}

Image RealmzScenarioData::generate_land_map(
    int16_t level_num,
    uint8_t x0,
    uint8_t y0,
    uint8_t w,
    uint8_t h,
    unordered_set<int16_t>* used_negative_tiles,
    unordered_map<string, unordered_set<uint8_t>>* used_positive_tiles) {
  const auto& mdata = this->land_maps.at(level_num);
  const auto& metadata = this->land_metadata.at(level_num);
  const auto& aps = this->land_aps.at(level_num);

  unordered_set<uint8_t>* used_positive_tiles_for_land_type = nullptr;
  if (used_positive_tiles) {
    used_positive_tiles_for_land_type = &(*used_positive_tiles)[metadata.land_type];
  }

  LevelNeighbors n;
  if (x0 == 0 && y0 == 0 && w == 90 && h == 90) {
    try {
      n = this->layout.get_level_neighbors(level_num);
    } catch (const runtime_error& e) {
      fprintf(stderr, "warning: can\'t get neighbors for level (%s)\n", e.what());
    }
  }

  int16_t start_x = -1, start_y = -1;
  if (level_num == this->scenario_metadata.start_level) {
    start_x = this->scenario_metadata.start_x;
    start_y = this->scenario_metadata.start_y;
  }

  if ((x0 >= 90) || (y0 >= 90) || ((x0 + w) > 90) || ((y0 + h) > 90)) {
    throw runtime_error("map bounds out of range");
  }

  unordered_map<uint16_t, vector<int>> loc_to_ap_nums;
  for (size_t x = 0; x < aps.size(); x++) {
    loc_to_ap_nums[location_sig(aps[x].get_x(), aps[x].get_y())].push_back(x);
  }

  size_t horizontal_neighbors = (n.left != -1 ? 1 : 0) + (n.right != -1 ? 1 : 0);
  size_t vertical_neighbors = (n.top != -1 ? 1 : 0) + (n.bottom != -1 ? 1 : 0);

  const TileSetDefinition* tileset;
  try {
    tileset = &this->land_type_to_tileset_definition.at(metadata.land_type);
  } catch (const out_of_range&) {
    tileset = &this->global.land_type_to_tileset_definition.at(metadata.land_type);
  }

  Image map(w * 32 + horizontal_neighbors * 9, h * 32 + vertical_neighbors * 9);

  // Write neighbor directory
  if (n.left != -1) {
    string text = string_printf("TO LEVEL %d", n.left);
    for (size_t y = (n.top != -1 ? 10 : 1); y < h * 32; y += 10 * 32) {
      for (size_t yy = 0; yy < text.size(); yy++) {
        map.draw_text(2, y + 9 * yy, 0xFFFFFFFF, 0x000000FF, "%c", text[yy]);
      }
    }
  }
  if (n.right != -1) {
    string text = string_printf("TO LEVEL %d", n.right);
    size_t x = 32 * 90 + (n.left != -1 ? 11 : 2);
    for (size_t y = (n.top != -1 ? 10 : 1); y < h * 32; y += 10 * 32) {
      for (size_t yy = 0; yy < text.size(); yy++) {
        map.draw_text(x, y + 9 * yy, 0xFFFFFFFF, 0x000000FF, "%c", text[yy]);
      }
    }
  }
  if (n.top != -1) {
    string text = string_printf("TO LEVEL %d", n.top);
    for (size_t x = (n.left != -1 ? 10 : 1); x < w * 32; x += 10 * 32) {
      map.draw_text(x, 1, 0xFFFFFFFF, 0x000000FF, "%s", text.c_str());
    }
  }
  if (n.bottom != -1) {
    string text = string_printf("TO LEVEL %d", n.bottom);
    size_t y = 32 * 90 + (n.top != -1 ? 10 : 1);
    for (size_t x = (n.left != -1 ? 10 : 1); x < w * 32; x += 10 * 32) {
      map.draw_text(x, y, 0xFFFFFFFF, 0x000000FF, "%s", text.c_str());
    }
  }

  // Load the positive pattern
  int16_t resource_id = resource_id_for_land_type(metadata.land_type);
  Image positive_pattern =
      this->scenario_rsf.resource_exists(RESOURCE_TYPE_PICT, resource_id)
      ? this->scenario_rsf.decode_PICT(resource_id).image
      : this->global.global_rsf.decode_PICT(resource_id).image;

  for (size_t y = y0; y < y0 + h; y++) {
    for (size_t x = x0; x < x0 + w; x++) {
      int16_t data = mdata.data[y][x];
      while (data <= -1000) {
        data += 1000;
      }
      while (data > 1000) {
        data -= 1000;
      }

      size_t xp = (x - x0) * 32 + (n.left != -1 ? 9 : 0);
      size_t yp = (y - y0) * 32 + (n.top != -1 ? 9 : 0);

      // Draw the tile itself
      if ((data < 0) || (data > 200)) { // Masked tile
        if (used_negative_tiles) {
          used_negative_tiles->emplace(data);
        }

        Image cicn;
        if (this->scenario_rsf.resource_exists(RESOURCE_TYPE_cicn, data)) {
          cicn = this->scenario_rsf.decode_cicn(data).image;
        } else if (this->global.global_rsf.resource_exists(RESOURCE_TYPE_cicn, data)) {
          cicn = this->global.global_rsf.decode_cicn(data).image;
        }

        // If neither cicn was valid, draw an error tile
        if (cicn.get_width() == 0 || cicn.get_height() == 0) {
          map.fill_rect(xp, yp, 32, 32, 0x000000FF);
          map.draw_text(xp + 2, yp + 30 - 9, 0xFFFFFFFF, 0x000000FF, "%04hX", data);

        } else {
          if (tileset->base_tile_id) {
            size_t source_id = tileset->base_tile_id - 1;
            size_t sxp = (source_id % 20) * 32;
            size_t syp = (source_id / 20) * 32;
            map.blit(positive_pattern, xp, yp, 32, 32, sxp, syp);
          } else {
            map.fill_rect(xp, yp, 32, 32, 0x000000FF);
          }

          // Negative tile images may be >32px in either dimension, and are
          // anchored at the lower-right corner, so we have to adjust the
          // destination x/y appropriately
          map.blit(
              cicn,
              xp - (cicn.get_width() - 32),
              yp - (cicn.get_height() - 32),
              cicn.get_width(),
              cicn.get_height(),
              0,
              0);
        }

      } else if (data <= 200) { // Standard tile
        if (used_positive_tiles_for_land_type) {
          used_positive_tiles_for_land_type->emplace(data);
        }

        size_t source_id = data - 1;
        size_t sxp = (source_id % 20) * 32;
        size_t syp = (source_id / 20) * 32;
        map.blit(positive_pattern, xp, yp, 32, 32, sxp, syp);

        // If it's a path, shade it red
        if (tileset->tiles[data].is_path) {
          map.fill_rect(xp, yp, 32, 32, 0xFF000040);
        }
      }
    }
  }

  // This is a separate loop so we can draw APs that are hidden by large
  // negative tile overlays
  for (size_t y = y0; y < y0 + h; y++) {
    for (size_t x = x0; x < x0 + w; x++) {

      size_t xp = (x - x0) * 32 + (n.left != -1 ? 9 : 0);
      size_t yp = (y - y0) * 32 + (n.top != -1 ? 9 : 0);

      int16_t data = mdata.data[y][x];
      bool has_ap = ((data <= -1000) || (data > 1000));
      bool ap_is_secret = ((data <= -3000) || (data > 3000));
      size_t text_xp = xp + 2;
      size_t text_yp = yp + 2;

      // Draw a red border if it has an AP, and make it dashed if the AP is
      // secret
      if (has_ap && ap_is_secret) {
        map.draw_horizontal_line(xp, xp + 31, yp, 4, 0xFF0000FF);
        map.draw_horizontal_line(xp, xp + 31, yp + 31, 4, 0xFF0000FF);
        map.draw_vertical_line(xp, yp, yp + 31, 4, 0xFF0000FF);
        map.draw_vertical_line(xp + 31, yp, yp + 31, 4, 0xFF0000FF);
      } else if (has_ap) {
        map.draw_horizontal_line(xp, xp + 31, yp, 0, 0xFF0000FF);
        map.draw_horizontal_line(xp, xp + 31, yp + 31, 0, 0xFF0000FF);
        map.draw_vertical_line(xp, yp, yp + 31, 0, 0xFF0000FF);
        map.draw_vertical_line(xp + 31, yp, yp + 31, 0, 0xFF0000FF);
      }

      // Draw the coords if both are multiples of 10
      if (y % 10 == 0 && x % 10 == 0) {
        map.draw_text(text_xp, text_yp, 0xFF00FFFF, 0x00000080, "%zu,%zu", x, y);
        text_yp += 8;
      }

      // Draw "START" if this is the start loc
      if (x == static_cast<size_t>(start_x) && y == static_cast<size_t>(start_y)) {
        map.draw_text(text_xp, text_yp, 0x00FFFFFF, 0x00000080, "START");
        text_yp += 8;
      }

      // Draw APs if present
      for (const auto& ap_num : loc_to_ap_nums[location_sig(x, y)]) {
        if (aps[ap_num].percent_chance < 100) {
          map.draw_text(text_xp, text_yp, 0xFFFFFFFF, 0x00000080, "%hd/%d-%d%%",
              level_num, ap_num, aps[ap_num].percent_chance);
        } else {
          map.draw_text(text_xp, text_yp, 0xFFFFFFFF, 0x00000080, "%hd/%d",
              level_num, ap_num);
        }
        text_yp += 8;
      }
    }
  }

  // Finally, draw random rects
  draw_random_rects(map, metadata.random_rects, (n.left != -1 ? 9 : 0),
      (n.top != -1 ? 9 : 0), false, level_num, x0, y0, w, h);

  return map;
}

////////////////////////////////////////////////////////////////////////////////
// DATA SD2

template <size_t FieldSize>
vector<string> load_fixed_size_string_index(const string& filename) {
  try {
    string data = load_file(filename);
    StringReader r(data);

    vector<string> ret;
    while (!r.eof()) {
      size_t size = min<size_t>(r.get_u8(), FieldSize);
      ret.emplace_back(r.read(size));
      if (!r.eof()) {
        r.skip(min<size_t>(r.remaining(), FieldSize - size));
      }
    }
    return ret;
  } catch (const cannot_open_file&) {
    return {};
  }
}

vector<string> RealmzScenarioData::load_string_index(const string& filename) {
  return load_fixed_size_string_index<0xFF>(filename);
}

////////////////////////////////////////////////////////////////////////////////
// DATA OD

vector<string> RealmzScenarioData::load_option_string_index(const string& filename) {
  return load_fixed_size_string_index<0x18>(filename);
}

////////////////////////////////////////////////////////////////////////////////
// DATA MD

vector<RealmzScenarioData::MonsterDefinition> RealmzScenarioData::load_monster_index(const string& filename) {
  return load_vector_file<MonsterDefinition>(filename);
}

string RealmzScenarioData::disassemble_monster(size_t index) {
  const auto& m = this->monsters.at(index);

  BlockStringWriter w;
  w.write_printf("===== MONSTER id=%zu [MST%zu]", index, index);
  w.write_printf("  stamina=%hhu bonus=%hhu", m.stamina, m.bonus_stamina);
  w.write_printf("  agility=%hhu", m.agility);
  w.write_printf("  description_index=%hhu", m.description_index);
  w.write_printf("  movement=%hhu", m.movement);
  w.write_printf("  armor_rating=%hhu", m.armor_rating);
  w.write_printf("  magic_resistance=%hhu", m.magic_resistance);
  if (m.required_weapon_id == -1) {
    w.write_printf("  required_weapon=BLUNT");
  } else if (m.required_weapon_id == -2) {
    w.write_printf("  required_weapon=SHARP");
  } else if (m.required_weapon_id == 0) {
    w.write_printf("  required_weapon=(any)");
  } else {
    w.write_printf("  required_weapon=%hhd", m.required_weapon_id);
  }
  w.write_printf("  traitor=%hhu", m.traitor);
  w.write_printf("  size=%hhu", m.size);
  w.write_printf("  magic_using=%hhu", m.magic_using);
  w.write_printf("  undead=%hhu", m.undead);
  w.write_printf("  demon_devil=%hhu", m.demon_devil);
  w.write_printf("  reptilian=%hhu", m.reptilian);
  w.write_printf("  very_evil=%hhu", m.very_evil);
  w.write_printf("  intelligent=%hhu", m.intelligent);
  w.write_printf("  giant_size=%hhu", m.giant_size);
  w.write_printf("  non_humanoid=%hhu", m.non_humanoid);
  w.write_printf("  num_physical_attacks=%hhu", m.num_physical_attacks);
  w.write_printf("  num_magic_attacks=%hhu", m.num_magic_attacks);
  for (size_t z = 0; z < 5; z++) {
    static const array<const char*, 0x0B> forms = {
        /* 20 */ "(nothing)",
        /* 21 */ "pummel",
        /* 22 */ "claw",
        /* 23 */ "bite",
        /* 24 */ "(unused-24)",
        /* 25 */ "(unused-25)",
        /* 26 */ "(unused-26)",
        /* 27 */ "punch/kick",
        /* 28 */ "club",
        /* 29 */ "slime",
        /* 2A */ "sting",
    };
    static const array<const char*, 0x14> special_conditions = {
        /* 00 */ "(nothing)",
        /* 01 */ "cause fear",
        /* 02 */ "paralyze",
        /* 03 */ "curse",
        /* 04 */ "stupefy",
        /* 05 */ "entangle",
        /* 06 */ "poison",
        /* 07 */ "confuse",
        /* 08 */ "drain spell points",
        /* 09 */ "drain experience",
        /* 0A */ "charm",
        /* 0B */ "fire damage",
        /* 0C */ "cold damage",
        /* 0D */ "electric damage",
        /* 0E */ "chemical damage",
        /* 0F */ "mental damage",
        /* 10 */ "cause disease",
        /* 11 */ "cause age",
        /* 12 */ "cause blindness",
        /* 13 */ "turn to stone",
    };
    const auto& att = m.attacks[z];
    w.write_printf("  (attack %zu) damage_range=[%hhu, %hhu]", z, att.min_damage, att.max_damage);
    try {
      w.write_printf("  (attack %zu) form=%s", z, forms.at(att.form - 0x20));
    } catch (const out_of_range&) {
      w.write_printf("  (attack %zu) form=(unknown-%02hhX)", z, att.form);
    }
    try {
      w.write_printf("  (attack %zu) special_condition=%s", z, special_conditions.at(att.special_condition));
    } catch (const out_of_range&) {
      w.write_printf("  (attack %zu) special_conditions=(unknown-%02hhX)", z, att.special_condition);
    }
  }
  w.write_printf("  damage_plus=%hhu", m.damage_plus);
  w.write_printf("  cast_spell_percent=%hhu", m.cast_spell_percent);
  w.write_printf("  run_away_percent=%hhu", m.run_away_percent);
  w.write_printf("  surrender_percent=%hhu", m.surrender_percent);
  w.write_printf("  use_missile_percent=%hhu", m.use_missile_percent);
  if (m.summon_flag == 0) {
    w.write_printf("  summon_flag=no");
  } else if (m.summon_flag == 1) {
    w.write_printf("  summon_flag=yes");
  } else if (m.summon_flag == -1) {
    w.write_printf("  summon_flag=is_npc");
  } else {
    w.write_printf("  summon_flag=%02hhX", m.summon_flag);
  }
  w.write_printf("  drv_adjust_heat=%hhd", m.drv_adjust_heat);
  w.write_printf("  drv_adjust_cold=%hhd", m.drv_adjust_cold);
  w.write_printf("  drv_adjust_electric=%hhd", m.drv_adjust_electric);
  w.write_printf("  drv_adjust_chemical=%hhd", m.drv_adjust_chemical);
  w.write_printf("  drv_adjust_mental=%hhd", m.drv_adjust_mental);
  w.write_printf("  drv_adjust_magic=%hhd", m.drv_adjust_magic);
  w.write_printf("  immune_to_charm=%hhu", m.immune_to_charm);
  w.write_printf("  immune_to_heat=%hhu", m.immune_to_heat);
  w.write_printf("  immune_to_cold=%hhu", m.immune_to_cold);
  w.write_printf("  immune_to_electric=%hhu", m.immune_to_electric);
  w.write_printf("  immune_to_chemical=%hhu", m.immune_to_chemical);
  w.write_printf("  immune_to_mental=%hhu", m.immune_to_mental);
  for (size_t z = 0; z < 3; z++) {
    uint16_t item_id = m.treasure_items[z];
    if (item_id) {
      try {
        const auto& item_info = this->info_for_item(item_id);
        w.write_printf("  treasure[%zu]=%hu (%s)", z, item_id, item_info.name.c_str());
      } catch (const out_of_range&) {
        w.write_printf("  treasure[%zu]=%hu", z, item_id);
      }
    }
  }
  for (size_t z = 0; z < 6; z++) {
    uint16_t item_id = m.held_items[z];
    if (item_id) {
      try {
        const auto& item_info = this->info_for_item(item_id);
        w.write_printf("  held_items[%zu]=%hu (%s)", z, item_id, item_info.name.c_str());
      } catch (const out_of_range&) {
        w.write_printf("  held_items[%zu]=%hu", z, item_id);
      }
    }
  }
  if (m.weapon) {
    try {
      const auto& item_info = this->info_for_item(m.weapon);
      w.write_printf("  weapon=%hu (%s)", m.weapon.load(), item_info.name.c_str());
    } catch (const out_of_range&) {
      w.write_printf("  weapon=%hu", m.weapon.load());
    }
  } else {
    w.write_printf("  weapon=(none)");
  }
  for (size_t z = 0; z < 10; z++) {
    uint16_t spell_id = m.spells[z];
    if (spell_id) {
      try {
        const string& name = this->name_for_spell(spell_id);
        w.write_printf("  spells[%zu]=%hu (%s)", z, spell_id, name.c_str());
      } catch (const out_of_range&) {
        w.write_printf("  spells[%zu]=%hu", z, spell_id);
      }
    }
  }
  w.write_printf("  spell_points=%hu", m.spell_points.load());
  w.write_printf("  icon=%hu", m.icon.load());
  string a1_str = format_data_string(m.unknown_a1, sizeof(m.unknown_a1));
  w.write_printf("  a1=%s", a1_str.c_str());
  string a2_str = format_data_string(m.unknown_a2, sizeof(m.unknown_a2));
  w.write_printf("  a2=%s", a2_str.c_str());
  w.write_printf("  hide_in_bestiary_menu=%hhu", m.hide_in_bestiary_menu);
  w.write_printf("  magic_plus_required_to_hit=%hhu", m.magic_plus_required_to_hit);
  string a3_str = format_data_string(m.unknown_a3, sizeof(m.unknown_a3));
  w.write_printf("  a3=%s", a3_str.c_str());
  string a4_str = format_data_string(m.unknown_a4, sizeof(m.unknown_a4));
  w.write_printf("  a4=%s", a4_str.c_str());
  for (size_t z = 0; z < sizeof(m.conditions); z++) {
    if (m.conditions[z]) {
      w.write_printf("  condition[%zu(%s)]=%hhd%s", z, char_condition_names.at(z).c_str(), m.conditions[z], m.conditions[z] < 0 ? " (permanent)" : "");
    }
  }
  w.write_printf("  macro_number=%hu", m.macro_number.load());
  string name(m.name, sizeof(m.name));
  strip_trailing_zeroes(name);
  w.write_printf("  name=\"%s\"", name.c_str());
  w.write("", 0);
  return w.close("\n");
}

string RealmzScenarioData::disassemble_all_monsters() {
  deque<string> blocks;
  for (size_t z = 0; z < this->monsters.size(); z++) {
    blocks.emplace_back(this->disassemble_monster(z));
  }
  return join(blocks, "");
}

//////////////////////////////////////////////////////////////////////////////
// DATA BD

vector<RealmzScenarioData::BattleDefinition> RealmzScenarioData::load_battle_index(const string& filename) {
  return load_vector_file<BattleDefinition>(filename);
}

string RealmzScenarioData::disassemble_battle(size_t index) {
  const auto& b = this->battles.at(index);

  BlockStringWriter w;
  set<int16_t> monster_ids;
  w.write_printf("===== BATTLE id=%zu [BTL%zu]", index, index);
  for (size_t y = 0; y < 13; y++) {
    string line = string_printf("  field[%zx]:", y);
    for (size_t x = 0; x < 13; x++) {
      int16_t monster_id = b.monster_ids[x][y];
      if (monster_id) {
        monster_ids.emplace(monster_id);
        line += string_printf(" %6hd", monster_id);
      } else {
        line += " ------";
      }
    }
    w.write(std::move(line));
  }
  for (int16_t monster_id : monster_ids) {
    uint16_t effective_monster_id = (monster_id < 0) ? -monster_id : monster_id;
    const char* friendly_str = (monster_id < 0) ? "(friendly) " : "";
    try {
      string name(this->monsters.at(effective_monster_id).name, sizeof(MonsterDefinition::name));
      strip_trailing_zeroes(name);
      w.write_printf("  (reference) %hd=%s%s", monster_id, friendly_str, name.c_str());
    } catch (const out_of_range&) {
      w.write_printf("  (reference) %hd=%s(missing)", monster_id, friendly_str);
    }
  }
  // TODO: Add monster names here for the monsters referenced in the above lines
  w.write_printf("  bonus_distance=%hhu", b.bonus_distance);
  w.write_printf("  a1=%02hhX", b.unknown_a1);
  string before = render_string_reference(this->strings, b.before_string);
  w.write_printf("  before_string=%s", before.c_str());
  string after = render_string_reference(this->strings, b.after_string);
  w.write_printf("  after_string=%s", after.c_str());
  w.write_printf("  macro_number=%hd", b.macro_number.load());
  w.write("", 0);
  return w.close("\n");
}

string RealmzScenarioData::disassemble_all_battles() {
  deque<string> blocks;
  for (size_t z = 0; z < this->battles.size(); z++) {
    blocks.emplace_back(this->disassemble_battle(z));
  }
  return join(blocks, "");
}

//////////////////////////////////////////////////////////////////////////////
// DATA NI

vector<RealmzScenarioData::ItemDefinition> RealmzScenarioData::load_custom_item_index(const string& filename) {
  return load_vector_file<ItemDefinition>(filename);
}

string RealmzScenarioData::disassemble_custom_item(size_t index) {
  const auto& i = this->custom_items.at(index);

  static const array<const char*, 26> wear_class_names = {
      /* 0 */ "ring",
      /* 1 */ "(unused-1)",
      /* 2 */ "melee weapon",
      /* 3 */ "shield",
      /* 4 */ "armor/robe",
      /* 5 */ "gauntlet/gloves",
      /* 6 */ "cloak/cape",
      /* 7 */ "helmet/cap",
      /* 8 */ "ion stone",
      /* 9 */ "boots",
      /* 10 */ "quiver",
      /* 11 */ "waist/belt",
      /* 12 */ "neck",
      /* 13 */ "scroll case",
      /* 14 */ "misc",
      /* 15 */ "missile weapon",
      /* 16 */ "brooch",
      /* 17 */ "face/mask",
      /* 18 */ "scabbard",
      /* 19 */ "belt loop",
      /* 20 */ "scroll",
      /* 21 */ "magic item",
      /* 22 */ "supply item",
      /* 23 */ "AP item", // TODO: special 5 = AP ID
      /* 24 */ "identified item",
      /* 25 */ "scenario item",
  };
  static const array<const char*, 64> category_flag_names = {
      /* 8000000000000000 */ "small blunt weapon",
      /* 4000000000000000 */ "medium blunt weapon",
      /* 2000000000000000 */ "large blunt weapon",
      /* 1000000000000000 */ "very small bladed weapon",
      /* 0800000000000000 */ "small bladed weapon",
      /* 0400000000000000 */ "medium bladed weapon",
      /* 0200000000000000 */ "large bladed weapon",
      /* 0100000000000000 */ "very large bladed weapon",
      /* 0080000000000000 */ "staff",
      /* 0040000000000000 */ "spear",
      /* 0020000000000000 */ "pole arm",
      /* 0010000000000000 */ "ninja style weapon",
      /* 0008000000000000 */ "normal bow",
      /* 0004000000000000 */ "crossbow",
      /* 0002000000000000 */ "dart",
      /* 0001000000000000 */ "flask of oil",
      /* 0000800000000000 */ "throwing knife",
      /* 0000400000000000 */ "whip",
      /* 0000200000000000 */ "quiver",
      /* 0000100000000000 */ "belt",
      /* 0000080000000000 */ "necklace",
      /* 0000040000000000 */ "cap",
      /* 0000020000000000 */ "soft helm",
      /* 0000010000000000 */ "small helm",
      /* 0000008000000000 */ "large helm",
      /* 0000004000000000 */ "small shield",
      /* 0000002000000000 */ "medium shield",
      /* 0000001000000000 */ "large shield",
      /* 0000000800000000 */ "bracer",
      /* 0000000400000000 */ "cloth gloves",
      /* 0000000200000000 */ "leather gloves",
      /* 0000000100000000 */ "metal gloves",
      /* 0000000080000000 */ "cloak/cape",
      /* 0000000040000000 */ "robe",
      /* 0000000020000000 */ "padded armor",
      /* 0000000010000000 */ "leather armor",
      /* 0000000008000000 */ "chain armor",
      /* 0000000004000000 */ "banded armor",
      /* 0000000002000000 */ "plate armor",
      /* 0000000001000000 */ "soft boots",
      /* 0000000000800000 */ "hard boots",
      /* 0000000000400000 */ "throwing hammer",
      /* 0000000000200000 */ "throwing stars",
      /* 0000000000100000 */ "misc blunt weapon",
      /* 0000000000080000 */ "misc bladed weapon",
      /* 0000000000040000 */ "misc large weapon",
      /* 0000000000020000 */ "misc missile weapon",
      /* 0000000000010000 */ "misc item",
      /* 0000000000008000 */ "scroll case",
      /* 0000000000004000 */ "brooch/pin",
      /* 0000000000002000 */ "ring",
      /* 0000000000001000 */ "potion",
      /* 0000000000000800 */ "misc magic item",
      /* 0000000000000400 */ "special object",
      /* 0000000000000200 */ "ion stone",
      /* 0000000000000100 */ "book",
      /* 0000000000000080 */ "scroll",
      /* 0000000000000040 */ "unused 40",
      /* 0000000000000020 */ "unused 20",
      /* 0000000000000010 */ "unused 10",
      /* 0000000000000008 */ "unused 08",
      /* 0000000000000004 */ "unused 04",
      /* 0000000000000002 */ "unused 02",
      /* 0000000000000001 */ "unused 01",
  };
  static const array<const char*, 9> race_flag_names = {
      /* 8000 */ "short",
      /* 4000 */ "elvish",
      /* 2000 */ "half",
      /* 1000 */ "goblinoid",
      /* 0800 */ "reptilian",
      /* 0400 */ "nether worldly",
      /* 0200 */ "goodly",
      /* 0100 */ "neutral",
      /* 0080 */ "evil",
  };
  static const array<const char*, 7> caste_flag_names = {
      /* 8000 */ "warrior",
      /* 4000 */ "thief",
      /* 2000 */ "archer",
      /* 1000 */ "sorcerer",
      /* 0800 */ "priest",
      /* 0400 */ "enchanter",
      /* 0200 */ "warrior wizard",
  };

  const char* item_name = nullptr;
  try {
    item_name = this->info_for_item(index + 800).name.c_str();
  } catch (const out_of_range&) {
  }

  BlockStringWriter w;
  if (item_name) {
    w.write_printf("===== ITEM id=%zu name=\"%s\" [ITM%zu]", index + 800, item_name, index + 800);
  } else {
    w.write_printf("===== ITEM id=%zu [ITM%zu]", index + 800, index + 800);
  }
  w.write_printf("  strength_bonus=%hd", i.strength_bonus.load());
  w.write_printf("  item_id=%hu", i.item_id.load());
  w.write_printf("  icon_id=%hd", i.icon_id.load());
  w.write_printf("  weapon_type=%hu", i.weapon_type.load());
  w.write_printf("  blade_type=%hd", i.blade_type.load());
  w.write_printf("  charge_count=%hd", i.charge_count.load());
  w.write_printf("  luck_bonus=%hd", i.luck_bonus.load());
  w.write_printf("  movement=%hd", i.movement.load());
  w.write_printf("  armor_rating=%hd", i.armor_rating.load());
  w.write_printf("  magic_resist=%hd", i.magic_resist.load());
  w.write_printf("  magic_plus=%hd", i.magic_plus.load());
  w.write_printf("  spell_points=%hd", i.spell_points.load());
  w.write_printf("  sound_id=%hd", i.sound_id.load());
  w.write_printf("  weight=%hd", i.weight.load());
  w.write_printf("  cost=%hd", i.cost.load());
  w.write_printf("  required_hands=%hu", i.required_hands.load());
  w.write_printf("  disguise_item_id=%hu", i.disguise_item_id.load());
  try {
    w.write_printf("  wear_class=%hu (%s)", i.wear_class.load(), wear_class_names.at(i.wear_class));
  } catch (const out_of_range&) {
    w.write_printf("  wear_class=%hu", i.wear_class.load());
  }
  w.write_printf("  category_flags=%016" PRIX64, i.category_flags.load());
  uint64_t category_flags_remaining = i.category_flags;
  for (ssize_t z = 63; (z >= 0) && category_flags_remaining; z--) {
    if (category_flags_remaining & 1) {
      w.write_printf("    %s", category_flag_names.at(z));
    }
    category_flags_remaining >>= 1;
  }
  w.write_printf("  not_usable_by_race_flags=%04hX", i.not_usable_by_race_flags.load());
  uint16_t race_flags_remaining = i.not_usable_by_race_flags;
  for (size_t z = 0; (z < 16) && race_flags_remaining; z++) {
    if ((z < race_flag_names.size()) && (race_flags_remaining & 0x8000)) {
      w.write_printf("    %s", race_flag_names.at(z));
    }
    race_flags_remaining >>= 1;
  }
  w.write_printf("  usable_by_races=%04hX", i.usable_by_race_flags.load());
  race_flags_remaining = i.usable_by_race_flags;
  for (size_t z = 0; (z < 16) && race_flags_remaining; z++) {
    if ((z < race_flag_names.size()) && (race_flags_remaining & 0x8000)) {
      w.write_printf("    %s", race_flag_names.at(z));
    }
    race_flags_remaining >>= 1;
  }
  w.write_printf("  not_usable_by_caste_flags=%04hX", i.not_usable_by_caste_flags.load());
  uint16_t caste_flags_remaining = i.not_usable_by_caste_flags;
  for (size_t z = 0; (z < 16) && caste_flags_remaining; z++) {
    if ((z < caste_flag_names.size()) && (caste_flags_remaining & 0x8000)) {
      w.write_printf("    %s", caste_flag_names.at(z));
    }
    caste_flags_remaining >>= 1;
  }
  w.write_printf("  usable_by_castes=%04hX", i.usable_by_caste_flags.load());
  caste_flags_remaining = i.usable_by_caste_flags;
  for (size_t z = 0; (z < 16) && caste_flags_remaining; z++) {
    if ((z < caste_flag_names.size()) && (caste_flags_remaining & 0x8000)) {
      w.write_printf("    %s", caste_flag_names.at(z));
    }
    caste_flags_remaining >>= 1;
  }
  w.write_printf("  specific_race=%hu", i.specific_race.load());
  w.write_printf("  specific_caste=%hu", i.specific_caste.load());
  string a2_str = format_data_string(i.unknown_a2, sizeof(i.unknown_a2));
  w.write_printf("  a2=%s", a2_str.c_str());
  w.write_printf("  damage=%hd", i.damage.load());
  string a3_str = format_data_string(i.unknown_a3, sizeof(i.unknown_a3));
  w.write_printf("  a3=%s", a3_str.c_str());
  w.write_printf("  heat_bonus_damage=%hd", i.heat_bonus_damage.load());
  w.write_printf("  cold_bonus_damage=%hd", i.cold_bonus_damage.load());
  w.write_printf("  electric_bonus_damage=%hd", i.electric_bonus_damage.load());
  w.write_printf("  undead_bonus_damage=%hd", i.undead_bonus_damage.load());
  w.write_printf("  demon_bonus_damage=%hd", i.demon_bonus_damage.load());
  w.write_printf("  evil_bonus_damage=%hd", i.evil_bonus_damage.load());
  bool special1_is_spell = false;
  bool special1_is_condition = false;
  if (i.specials[0] <= -1 && i.specials[0] >= -7) {
    w.write_printf("  specials[0]=power level %hd", static_cast<int16_t>(-i.specials[0]));
  } else if (i.specials[0] == 8) {
    w.write_printf("  specials[0]=random power level");
  } else if (i.specials[0] >= 20 && i.specials[0] < 60) {
    w.write_printf("  specials[0]=add condition %hd (%s)", static_cast<int16_t>(i.specials[0] - 20), char_condition_names.at(i.specials[0] - 20).c_str());
  } else if (i.specials[0] >= 60 && i.specials[0] < 100) {
    w.write_printf("  specials[0]=remove condition %hd (%s)", static_cast<int16_t>(i.specials[0] - 60), char_condition_names.at(i.specials[0] - 60).c_str());
  } else if (i.specials[0] == 120) {
    w.write_printf("  specials[0]=auto hit");
  } else if (i.specials[0] == 121) {
    w.write_printf("  specials[0]=double to-hit bonus");
  } else if (i.specials[0] == 122) {
    w.write_printf("  specials[0]=bonus attack");
  } else {
    w.write_printf("  specials[0]=%hd (unknown)", i.specials[0].load());
  }
  if (special1_is_spell) {
    try {
      const auto& name = this->name_for_spell(i.specials[1]);
      w.write_printf("  specials[1]=%hd (%s)", i.specials[1].load(), name.c_str());
    } catch (const out_of_range&) {
      w.write_printf("  specials[1]=%hd (unknown spell)", i.specials[1].load());
    }
  } else if (special1_is_condition) {
    w.write_printf("  specials[1]=%hd rounds%s", i.specials[1].load(), i.specials[1] < 0 ? " (permanent)" : "");
  } else {
    w.write_printf("  specials[1]=%hd", i.specials[1].load());
  }
  // TODO: These two fields are described as:
  //   - = Special Attributes
  //   + = Special Ability
  //   30 to 40 Party Condition
  // Assign names to values appropriately here.
  if (i.specials[2] < 0) {
    w.write_printf("  specials[2]=%hd (attribute)", i.specials[2].load());
  } else {
    w.write_printf("  specials[2]=%hd (ability)", i.specials[2].load());
  }
  if (i.specials[3] < 0) {
    w.write_printf("  specials[3]=%hd (attribute)", i.specials[3].load());
  } else {
    w.write_printf("  specials[3]=%hd (ability)", i.specials[3].load());
  }
  if (i.wear_class == 23) {
    w.write_printf("  specials[4]=%hd (AP number)", i.specials[4].load());
  } else {
    w.write_printf("  specials[4]=%hd (attr/ability amount)", i.specials[4].load());
  }
  w.write_printf("  weight_per_charge=%hu", i.weight_per_charge.load());
  w.write_printf("  drop_on_empty=%hu", i.drop_on_empty.load());
  w.write("", 0);
  return w.close("\n");
}

string RealmzScenarioData::disassemble_all_custom_items() {
  deque<string> blocks;
  for (size_t z = 0; z < this->custom_items.size(); z++) {
    blocks.emplace_back(this->disassemble_custom_item(z));
  }
  return join(blocks, "");
}

//////////////////////////////////////////////////////////////////////////////
// DATA SD

vector<RealmzScenarioData::Shop> RealmzScenarioData::load_shop_index(const string& filename) {
  return load_vector_file<Shop>(filename);
}

string RealmzScenarioData::disassemble_shop(size_t index) {
  const auto& s = this->shops.at(index);

  static const array<const char*, 5> category_names = {
      "weapons", "armor1", "armor2", "magic", "items"};

  BlockStringWriter w;
  w.write_printf("===== SHOP id=%zu [SHP%zu]", index, index);
  w.write_printf("  inflation_percent=%hu", s.inflation_percent.load());
  for (size_t z = 0; z < 1000; z++) {
    if (s.item_ids[z] || s.item_counts[z]) {
      const char* item_name = "unknown";
      try {
        item_name = this->info_for_item(s.item_ids[z]).name.c_str();
      } catch (const out_of_range&) {
      }
      w.write_printf("  %s[%zu]=%hu (%s) x%hhu", category_names[z / 200], z % 200, s.item_ids[z].load(), item_name, s.item_counts[z]);
    }
  }
  w.write("", 0);
  return w.close("\n");
}

string RealmzScenarioData::disassemble_all_shops() {
  deque<string> blocks;
  for (size_t z = 0; z < this->shops.size(); z++) {
    blocks.emplace_back(this->disassemble_shop(z));
  }
  return join(blocks, "");
}
