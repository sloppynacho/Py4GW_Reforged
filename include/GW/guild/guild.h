#pragma once

#include "base/error_handling.h"

#include "GW/context/guild.h"

#include <atomic>
#include <cstdint>

namespace gw::guild {

bool Initialize();
void Shutdown();

context::GuildArray* GetGuildArray();
context::Guild* GetPlayerGuild();
context::Guild* GetCurrentGH();
context::Guild* GetGuildInfo(uint32_t guild_id);
uint32_t GetPlayerGuildIndex();
wchar_t* GetPlayerGuildAnnouncement();
wchar_t* GetPlayerGuildAnnouncer();

bool TravelGH();
bool TravelGH(context::GHKey key);
bool LeaveGH();

extern std::atomic<bool> g_initialized;

}  // namespace gw::guild
