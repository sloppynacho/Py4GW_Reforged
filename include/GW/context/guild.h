#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/maps.h"
#include "GW/common/gw_array.h"

#include <algorithm>
#include <cstdint>

namespace gw::context {

struct GHKey {
    uint32_t k[4]{};

    explicit operator bool() const {
        return std::any_of(std::begin(k), std::end(k), [](uint32_t value) { return value != 0; });
    }
};

struct GuildPlayer {
    void* vtable;
    wchar_t* name_ptr;
    wchar_t invited_name[20];
    wchar_t current_name[20];
    wchar_t inviter_name[20];
    uint32_t invite_time;
    wchar_t promoter_name[20];
    uint32_t h00AC[12];
    uint32_t offline;
    uint32_t member_type;
    uint32_t status;
    uint32_t h00E8[35];
};
static_assert(sizeof(GuildPlayer) == 0x174, "GuildPlayer size mismatch");

using GuildRoster = gw::GwArray<GuildPlayer*>;

struct GuildHistoryEvent {
    uint32_t time1;
    uint32_t time2;
    wchar_t name[256];
};
static_assert(sizeof(GuildHistoryEvent) == 0x208, "GuildHistoryEvent size mismatch");

using GuildHistory = gw::GwArray<GuildHistoryEvent*>;

struct CapeDesign {
    uint32_t cape_bg_color;
    uint32_t cape_detail_color;
    uint32_t cape_emblem_color;
    uint32_t cape_shape;
    uint32_t cape_detail;
    uint32_t cape_emblem;
    uint32_t cape_trim;
};
static_assert(sizeof(CapeDesign) == 0x1C, "CapeDesign size mismatch");

struct Guild {
    GHKey key;
    uint32_t h0010[5];
    uint32_t index;
    uint32_t rank;
    uint32_t features;
    wchar_t name[32];
    uint32_t rating;
    uint32_t faction;
    uint32_t faction_point;
    uint32_t qualifier_point;
    wchar_t tag[8];
    CapeDesign cape;
};
static_assert(sizeof(Guild) == 0xAC, "Guild size mismatch");

using GuildArray = gw::GwArray<Guild*>;

struct TownAlliance {
    uint32_t rank;
    uint32_t allegiance;
    uint32_t faction;
    wchar_t name[32];
    wchar_t tag[5];
    uint8_t padding[2];
    CapeDesign cape;
    gw::constants::MapID map_id;
};
static_assert(sizeof(TownAlliance) == 0x78, "TownAlliance size mismatch");

}  // namespace gw::context
