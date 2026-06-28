#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"
#include "GW/context/guild.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct GuildContext {
    uint32_t h0000;
    uint32_t h0004;
    uint32_t h0008;
    uint32_t h000c;
    uint32_t h0010;
    uint32_t h0014;
    uint32_t h0018;
    uint32_t h001c;
    gw::GwArray<void*> h0020;
    uint32_t h0030;
    wchar_t player_name[20];
    uint32_t h005c;
    uint32_t player_guild_index;
    GHKey player_gh_key;
    uint32_t h0074;
    wchar_t announcement[256];
    wchar_t announcement_author[20];
    uint32_t player_guild_rank;
    uint32_t h02a4;
    gw::GwArray<TownAlliance> factions_outpost_guilds;
    uint32_t kurzick_town_count;
    uint32_t luxon_town_count;
    uint32_t h02c0;
    uint32_t h02c4;
    uint32_t h02c8;
    GuildHistory player_guild_history;
    uint32_t h02dc[7];
    GuildArray guilds;
    uint32_t h0308[4];
    gw::GwArray<void*> h0318;
    uint32_t h0328;
    gw::GwArray<void*> h032c;
    uint32_t h033c[7];
    GuildRoster player_roster;
};

static_assert(offsetof(GuildContext, player_name) == 0x34, "GuildContext::player_name offset mismatch");
static_assert(offsetof(GuildContext, player_gh_key) == 0x64, "GuildContext::player_gh_key offset mismatch");
static_assert(offsetof(GuildContext, announcement) == 0x78, "GuildContext::announcement offset mismatch");
static_assert(offsetof(GuildContext, factions_outpost_guilds) == 0x2A8, "GuildContext::factions_outpost_guilds offset mismatch");
static_assert(offsetof(GuildContext, guilds) == 0x2F8, "GuildContext::guilds offset mismatch");
static_assert(offsetof(GuildContext, player_roster) == 0x358, "GuildContext::player_roster offset mismatch");

}  // namespace gw::context
