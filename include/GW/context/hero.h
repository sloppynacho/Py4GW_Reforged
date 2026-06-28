#pragma once

#include "base/error_handling.h"

#include "GW/common/game_pos.h"
#include "GW/common/gw_array.h"

#include <cstdint>

namespace gw::context {

using AgentID = uint32_t;

enum class HeroBehavior : uint32_t {
    Fight,
    Guard,
    AvoidCombat
};

struct HeroFlag {
    uint32_t hero_id;
    AgentID agent_id;
    uint32_t level;
    HeroBehavior hero_behavior;
    Vec2f flag;
    uint32_t h0018;
    AgentID locked_target_id;
    uint32_t h0020;
};
static_assert(sizeof(HeroFlag) == 0x24, "HeroFlag size mismatch");

struct HeroInfo {
    uint32_t hero_id;
    uint32_t agent_id;
    uint32_t level;
    uint32_t primary;
    uint32_t secondary;
    uint32_t hero_file_id;
    uint32_t model_file_id;
    uint8_t h001C[52];
    wchar_t name[20];
};
static_assert(sizeof(HeroInfo) == 0x78, "HeroInfo size mismatch");

using HeroFlagArray = gw::GwArray<HeroFlag>;
using HeroInfoArray = gw::GwArray<HeroInfo>;

}  // namespace gw::context
