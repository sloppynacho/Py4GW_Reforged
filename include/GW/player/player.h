#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/context/player.h"
#include "GW/context/title.h"

#include <atomic>
#include <cstdint>
#include <vector>

namespace gw::player {

using PlayerNumber = uint32_t;

bool Initialize();
void Shutdown();

using RemoveActiveTitleFn = void(__cdecl*)();
using SetActiveTitleFn = void(__cdecl*)(uint32_t identifier);
using DepositFactionFn = void(__cdecl*)(uint32_t always_0, uint32_t allegiance, uint32_t amount);

bool SetActiveTitle(gw::constants::TitleID title_id);
bool RemoveActiveTitle();

uint32_t GetPlayerAgentId(uint32_t player_id);
uint32_t GetAmountOfPlayersInInstance();

context::PlayerArray* GetPlayerArray();
PlayerNumber GetPlayerNumber();

context::Player* GetPlayerByID(uint32_t player_id = 0);
wchar_t* GetPlayerName(uint32_t player_id = 0);
wchar_t* SetPlayerName(uint32_t player_id, const wchar_t* replace_name);

// bool ChangeSecondProfession(gw::constants::Profession profession, uint32_t hero_index = 0);
// Deferred until SkillbarMgr is migrated. Legacy body:
// return SkillbarMgr::ChangeSecondProfession(profession, hero_index);

context::Player* GetPlayerByName(const wchar_t* name);

context::Title* GetTitleTrack(gw::constants::TitleID title_id);
gw::constants::TitleID GetActiveTitleId();
context::Title* GetActiveTitle();
std::vector<int> GetTitleIDs();
context::TitleClientData* GetTitleData(gw::constants::TitleID title_id);

bool DepositFaction(uint32_t allegiance);

extern RemoveActiveTitleFn g_remove_active_title_func;
extern SetActiveTitleFn g_set_active_title_func;
extern DepositFactionFn g_deposit_faction_func;
extern context::TitleClientData* g_title_data;
extern std::atomic<bool> g_initialized;

}  // namespace gw::player
