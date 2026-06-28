#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/constants/maps.h"
#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct ObserverMatch;

struct CharProgressBar {
    int pips;
    uint8_t color[4];
    uint8_t background[4];
    int h000c[7];
    float progress;
};
static_assert(sizeof(CharProgressBar) == 0x2C, "CharProgressBar size mismatch");

struct CharContext {
    gw::GwArray<void*> h0000;
    uint32_t h0010;
    gw::GwArray<void*> h0014;
    uint32_t h0024[4];
    gw::GwArray<void*> h0034;
    gw::GwArray<void*> h0044;
    uint32_t h0054[4];
    uint32_t player_uuid[4];
    wchar_t player_name[0x14];
    uint32_t h009c[20];
    gw::GwArray<void*> h00ec;
    uint32_t h00fc[37];
    uint32_t world_flags;
    uint32_t token1;
    gw::constants::MapID map_id;
    uint32_t is_explorable;
    uint8_t host[0x18];
    uint32_t token2;
    uint32_t h01bc[27];
    int32_t district_number;
    gw::constants::Language language;
    gw::constants::MapID observe_map_id;
    gw::constants::MapID current_map_id;
    gw::constants::InstanceType observe_map_type;
    gw::constants::InstanceType current_map_type;
    uint32_t h0240[5];
    gw::GwArray<ObserverMatch*> observer_matches;
    uint32_t h0264[17];
    uint32_t player_flags;
    uint32_t player_number;
    uint32_t h02b0[40];
    CharProgressBar* progress_bar;
    uint32_t h0354[27];
    wchar_t player_email[0x40];
};

static_assert(offsetof(CharContext, player_uuid) == 0x64, "CharContext::player_uuid offset mismatch");
static_assert(offsetof(CharContext, player_name) == 0x74, "CharContext::player_name offset mismatch");
static_assert(offsetof(CharContext, map_id) == 0x198, "CharContext::map_id offset mismatch");
static_assert(offsetof(CharContext, observer_matches) == 0x254, "CharContext::observer_matches offset mismatch");
static_assert(offsetof(CharContext, progress_bar) == 0x350, "CharContext::progress_bar offset mismatch");
static_assert(sizeof(CharContext) == 0x440, "CharContext size mismatch");

}  // namespace gw::context
