#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"
#include "GW/common/gw_list.h"
#include "GW/context/party.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct PartyContext {
    uint32_t h0000;
    gw::GwArray<void*> h0004;
    uint32_t flag;
    uint32_t h0018;
    gw::GwList<PartyInfo> requests;
    uint32_t requests_count;
    gw::GwList<PartyInfo> sending;
    uint32_t sending_count;
    uint32_t h003c;
    gw::GwArray<PartyInfo*> parties;
    uint32_t h0050;
    PartyInfo* player_party;
    uint8_t h0058[104];
    gw::GwArray<PartySearch*> party_search;

    bool InHardMode() const { return (flag & 0x10U) != 0; }
    bool IsDefeated() const { return (flag & 0x20U) != 0; }
    bool IsPartyLeader() const { return ((flag >> 0x7) & 1U) != 0; }
};

static_assert(offsetof(PartyContext, requests) == 0x1C, "PartyContext::requests offset mismatch");
static_assert(offsetof(PartyContext, parties) == 0x40, "PartyContext::parties offset mismatch");
static_assert(offsetof(PartyContext, player_party) == 0x54, "PartyContext::player_party offset mismatch");
static_assert(offsetof(PartyContext, party_search) == 0xC0, "PartyContext::party_search offset mismatch");
static_assert(sizeof(PartyContext) == 0xD0, "PartyContext size mismatch");

}  // namespace gw::context
