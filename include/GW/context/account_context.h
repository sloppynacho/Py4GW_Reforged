#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct AccountUnlockedCount {
    uint32_t id;
    uint32_t unk1;
    uint32_t unk2;
};
static_assert(sizeof(AccountUnlockedCount) == 0xC, "AccountUnlockedCount size mismatch");

struct AccountUnlockedItemInfo {
    uint32_t name_id;
    uint32_t mod_struct_index;
    uint32_t mod_struct_size;
};
static_assert(sizeof(AccountUnlockedItemInfo) == 0xC, "AccountUnlockedItemInfo size mismatch");

struct AccountContext {
    gw::GwArray<AccountUnlockedCount> account_unlocked_counts;
    uint8_t h0010[0xA4];
    gw::GwArray<uint32_t> unlocked_pvp_heros;
    gw::GwArray<uint32_t> h00c4;
    gw::GwArray<void*> h00d4;
    gw::GwArray<AccountUnlockedItemInfo> unlocked_pvp_item_info;
    gw::GwArray<uint32_t> unlocked_pvp_items;
    uint8_t h0104[0x20];
    gw::GwArray<uint32_t> unlocked_account_skills;
    uint32_t account_flags;
};

static_assert(offsetof(AccountContext, unlocked_pvp_heros) == 0xB4, "AccountContext::unlocked_pvp_heros offset mismatch");
static_assert(offsetof(AccountContext, h00d4) == 0xD4, "AccountContext::h00d4 offset mismatch");
static_assert(offsetof(AccountContext, unlocked_pvp_item_info) == 0xE4, "AccountContext::unlocked_pvp_item_info offset mismatch");
static_assert(offsetof(AccountContext, unlocked_account_skills) == 0x124, "AccountContext::unlocked_account_skills offset mismatch");
static_assert(sizeof(AccountContext) == 0x138, "AccountContext size mismatch");

}  // namespace gw::context
