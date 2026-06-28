#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"

#include <cstdint>

namespace gw::context {

using PlayerID = uint32_t;

struct Player {
    uint32_t agent_id;
    uint32_t h0004[3];
    uint32_t appearance_bitmap;
    uint32_t flags;
    uint32_t primary;
    uint32_t secondary;
    uint32_t h0020;
    wchar_t* name_enc;
    wchar_t* name;
    uint32_t party_leader_player_number;
    uint32_t active_title_tier;
    uint32_t reforged_or_dhuums_flags;
    uint32_t player_number;
    uint32_t party_size;
    gw::GwArray<void*> h0040;

    bool IsPvP() const { return (flags & 0x800U) != 0; }
};
static_assert(sizeof(Player) == 0x50, "Player size mismatch");

using PlayerArray = gw::GwArray<Player>;

}  // namespace gw::context
