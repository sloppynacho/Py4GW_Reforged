#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/maps.h"
#include "GW/common/constants/quest_ids.h"
#include "GW/common/game_pos.h"
#include "GW/common/gw_array.h"

#include <cstdint>

namespace gw::context {

struct Quest {
    gw::constants::QuestID quest_id;
    uint32_t log_state;
    wchar_t* location;
    wchar_t* name;
    wchar_t* npc;
    gw::constants::MapID map_from;
    GamePos marker;
    uint32_t h0024;
    gw::constants::MapID map_to;
    wchar_t* description;
    wchar_t* objectives;

    bool IsCompleted() const { return (log_state & 0x2U) != 0; }
    bool IsCurrentMissionQuest() const { return (log_state & 0x10U) != 0; }
    bool IsAreaPrimary() const { return (log_state & 0x40U) != 0; }
    bool IsPrimary() const { return (log_state & 0x20U) != 0; }
};
static_assert(sizeof(Quest) == 0x34, "Quest size mismatch");

struct MissionObjective {
    uint32_t objective_id;
    wchar_t* enc_str;
    uint32_t type;
};
static_assert(sizeof(MissionObjective) == 0xC, "MissionObjective size mismatch");

using QuestLog = gw::GwArray<Quest>;

}  // namespace gw::context
