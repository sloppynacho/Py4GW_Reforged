#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/gw_array.h"
#include "GW/common/gw_list.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct PlayerPartyMember {
    uint32_t login_number;
    uint32_t called_target_id;
    uint32_t state;

    bool connected() const { return (state & 1U) != 0; }
    bool ticked() const { return (state & 2U) != 0; }
};
static_assert(sizeof(PlayerPartyMember) == 0xC, "PlayerPartyMember size mismatch");

struct HeroPartyMember {
    uint32_t agent_id;
    uint32_t owner_player_id;
    uint32_t hero_id;
    uint32_t h000c;
    uint32_t h0010;
    uint32_t level;
};
static_assert(sizeof(HeroPartyMember) == 0x18, "HeroPartyMember size mismatch");

struct HenchmanPartyMember {
    uint32_t agent_id;
    uint32_t h0004[10];
    gw::constants::Profession profession;
    uint32_t level;
};
static_assert(sizeof(HenchmanPartyMember) == 0x34, "HenchmanPartyMember size mismatch");

using HeroPartyMemberArray = gw::GwArray<HeroPartyMember>;
using PlayerPartyMemberArray = gw::GwArray<PlayerPartyMember>;
using HenchmanPartyMemberArray = gw::GwArray<HenchmanPartyMember>;

struct PartyInfo {
    uint32_t party_id;
    PlayerPartyMemberArray players;
    HenchmanPartyMemberArray henchmen;
    HeroPartyMemberArray heroes;
    gw::GwArray<uint32_t> others;
    uint32_t h0044[14];
    gw::GwLink<PartyInfo> invite_link;

    size_t GetPartySize() const;
};
static_assert(sizeof(PartyInfo) == 0x84, "PartyInfo size mismatch");

enum PartySearchType {
    PartySearchType_Hunting = 0,
    PartySearchType_Mission = 1,
    PartySearchType_Quest = 2,
    PartySearchType_Trade = 3,
    PartySearchType_Guild = 4,
};

struct PartySearch {
    uint32_t party_search_id;
    uint32_t party_search_type;
    uint32_t hardmode;
    uint32_t district;
    uint32_t language;
    uint32_t party_size;
    uint32_t hero_count;
    wchar_t message[32];
    wchar_t party_leader[20];
    gw::constants::Profession primary;
    gw::constants::Profession secondary;
    uint32_t level;
    uint32_t timestamp;
};
static_assert(sizeof(PartySearch) == 0x94, "PartySearch size mismatch");

}  // namespace gw::context
