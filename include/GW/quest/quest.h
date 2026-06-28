#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/context/quest.h"

#include <atomic>
#include <cstdint>
#include <string>

namespace gw::quest {

bool Initialize();
void Shutdown();

using RequestQuestInfoFn = void(__cdecl*)(uint32_t identifier);
using RequestQuestDataFn = void(__cdecl*)(uint32_t identifier, bool update_markers);

gw::constants::QuestID GetActiveQuestId();

// bool SetActiveQuestId(gw::constants::QuestID quest_id);
// bool SetActiveQuest(context::Quest* quest);
// bool AbandonQuest(context::Quest* quest);
// bool AbandonQuestId(gw::constants::QuestID quest_id);
// Deferred until UIMgr exists. Legacy behavior routes through UI message plumbing.

context::Quest* GetActiveQuest();
context::QuestLog* GetQuestLog();
context::Quest* GetQuest(gw::constants::QuestID quest_id);

bool GetQuestEntryGroupName(gw::constants::QuestID quest_id, wchar_t* out, size_t out_len);

bool RequestQuestInfo(const context::Quest* quest, bool update_markers = false);
bool RequestQuestInfoId(gw::constants::QuestID quest_id, bool update_markers = false);

void AsyncGetQuestName(const context::Quest* quest, std::wstring& res);
void AsyncGetQuestDescription(const context::Quest* quest, std::wstring& res);
void AsyncGetQuestObjectives(const context::Quest* quest, std::wstring& res);
void AsyncGetQuestLocation(const context::Quest* quest, std::wstring& res);
void AsyncGetQuestNPC(const context::Quest* quest, std::wstring& res);
void AsyncDecodeAnyEncStr(const wchar_t* str, std::wstring& res);

extern RequestQuestInfoFn g_request_quest_info_func;
extern RequestQuestDataFn g_request_quest_data_func;
extern std::atomic<bool> g_initialized;

}  // namespace gw::quest
