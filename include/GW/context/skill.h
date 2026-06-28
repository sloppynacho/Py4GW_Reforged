#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/gw_array.h"
#include "GW/common/constants/skills.h"

#include <cstddef>
#include <cstdint>
#include <windows.h>

namespace gw::context {

struct Skill {
    gw::constants::SkillID skill_id;
    uint32_t h0004;
    gw::constants::Campaign campaign;
    gw::constants::SkillType type;
    uint32_t special;
    uint32_t combo_req;
    uint32_t effect1;
    uint32_t condition;
    uint32_t effect2;
    uint32_t weapon_req;
    gw::constants::ProfessionByte profession;
    gw::constants::AttributeByte attribute;
    uint16_t title;
    gw::constants::SkillID skill_id_pvp;
    uint8_t combo;
    uint8_t target;
    uint8_t h0032;
    uint8_t skill_equip_type;
    uint8_t overcast;
    uint8_t energy_cost;
    uint8_t health_cost;
    uint8_t h0037;
    uint32_t adrenaline;
    float activation;
    float aftercast;
    uint32_t duration0;
    uint32_t duration15;
    uint32_t recharge;
    uint16_t h0050[4];
    uint32_t skill_arguments;
    uint32_t scale0;
    uint32_t scale15;
    uint32_t bonus_scale0;
    uint32_t bonus_scale15;
    float aoe_range;
    float const_effect;
    uint32_t caster_overhead_animation_id;
    uint32_t caster_body_animation_id;
    uint32_t target_body_animation_id;
    uint32_t target_overhead_animation_id;
    uint32_t projectile_animation_1_id;
    uint32_t projectile_animation_2_id;
    uint32_t icon_file_id;
    uint32_t icon_file_id_2;
    uint32_t icon_file_id_hi_res;
    uint32_t name;
    uint32_t concise;
    uint32_t description;

    uint8_t GetEnergyCost() const {
        switch (energy_cost) {
        case 11:
            return 15;
        case 12:
            return 25;
        default:
            return energy_cost;
        }
    }

    [[nodiscard]] bool IsTouchRange() const { return (special & 0x2U) != 0; }
    [[nodiscard]] bool IsElite() const { return (special & 0x4U) != 0; }
    [[nodiscard]] bool IsHalfRange() const { return (special & 0x8U) != 0; }
    [[nodiscard]] bool IsPvP() const { return (special & 0x400000U) != 0; }
    [[nodiscard]] bool IsPvE() const { return (special & 0x80000U) != 0; }
    [[nodiscard]] bool IsPlayable() const { return (special & 0x2000000U) == 0; }
    [[nodiscard]] bool IsStacking() const { return (special & 0x10000U) != 0; }
    [[nodiscard]] bool IsNonStacking() const { return (special & 0x20000U) != 0; }

    [[nodiscard]] bool IsUnused() const;
};
static_assert(sizeof(Skill) == 0xA4, "Skill size mismatch");

using SkillArray = gw::GwArray<Skill>;

struct SkillbarSkill {
    uint32_t adrenaline_a;
    uint32_t adrenaline_b;
    uint32_t recharge;
    gw::constants::SkillID skill_id;
    uint32_t event;

    uint32_t GetRecharge() const;
};

struct SkillbarCast {
    uint16_t h0000;
    gw::constants::SkillID skill_id;
    uint32_t h0004;
};

using SkillbarCastArray = gw::GwArray<SkillbarCast>;

struct Skillbar {
    uint32_t agent_id;
    SkillbarSkill skills[8];
    uint32_t disabled;
    uint32_t h00a8[2];
    uint32_t casting;
    uint32_t h00b4[2];

    bool IsValid() const {
        return agent_id > 0;
    }

    SkillbarSkill* GetSkillById(gw::constants::SkillID skill_id, size_t* slot_out = nullptr);
};

using SkillbarArray = gw::GwArray<Skillbar>;

struct Effect {
    gw::constants::SkillID skill_id;
    uint32_t attribute_level;
    uint32_t effect_id;
    uint32_t agent_id;
    float duration;
    DWORD timestamp;

    DWORD GetTimeElapsed() const;
    DWORD GetTimeRemaining() const;
};

struct Buff {
    gw::constants::SkillID skill_id;
    uint32_t h0004;
    uint32_t buff_id;
    uint32_t target_agent_id;
};

using BuffArray = gw::GwArray<Buff>;
using EffectArray = gw::GwArray<Effect>;

struct AgentEffects {
    uint32_t agent_id;
    BuffArray buffs;
    EffectArray effects;
};

using AgentEffectsArray = gw::GwArray<AgentEffects>;

static_assert(sizeof(SkillbarSkill) == 0x14, "SkillbarSkill size mismatch");
static_assert(sizeof(Skillbar) == 0xBC, "Skillbar size mismatch");
static_assert(sizeof(Effect) == 0x18, "Effect size mismatch");
static_assert(sizeof(Buff) == 0x10, "Buff size mismatch");
static_assert(sizeof(AgentEffects) == 0x24, "AgentEffects size mismatch");

}  // namespace gw::context
