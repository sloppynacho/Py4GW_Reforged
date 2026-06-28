#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/game_pos.h"
#include "GW/common/gw_array.h"
#include "GW/common/gw_list.h"
#include "GW/context/item.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct VisibleEffect {
    uint32_t unk;
    gw::constants::EffectID id;
    uint32_t has_ended;
};
static_assert(sizeof(VisibleEffect) == 0xC, "VisibleEffect size mismatch");

using VisibleEffectList = gw::GwList<VisibleEffect>;

struct EquipmentVTable {
    void(__fastcall* Destroy)(void* this_ptr);
    void(__fastcall* GetItemClassFlags)(void* this_ptr, uint32_t edx, uint32_t slot);
    void(__fastcall* EquipItem)(void* this_ptr, uint32_t edx, uint32_t slot);
    void(__fastcall* LoadModelMaybe)(void* this_ptr, uint32_t edx, uint32_t slot);
    void(__fastcall* RemoveItem)(void* this_ptr, uint32_t edx, uint32_t slot);
    void(__fastcall* RefreshModelMaybe)(void* this_ptr);
    bool(__fastcall* ModelRelatedBooleanCheck)(void* this_ptr);
    uint32_t(__fastcall* GetType)(void* this_ptr);
};

struct Equipment {
    void* vtable;
    uint32_t h0004;
    uint32_t h0008;
    uint32_t h000c;
    ItemData* left_hand_ptr;
    ItemData* right_hand_ptr;
    uint32_t h0018;
    ItemData* shield_ptr;
    uint8_t left_hand_map;
    uint8_t right_hand_map;
    uint8_t head_map;
    uint8_t shield_map;
    union {
        ItemData items[9];
        struct {
            ItemData weapon;
            ItemData offhand;
            ItemData chest;
            ItemData legs;
            ItemData head;
            ItemData feet;
            ItemData hands;
            ItemData costume_body;
            ItemData costume_head;
        };
    };
    union {
        uint32_t item_ids[9];
        struct {
            uint32_t item_id_weapon;
            uint32_t item_id_offhand;
            uint32_t item_id_chest;
            uint32_t item_id_legs;
            uint32_t item_id_head;
            uint32_t item_id_feet;
            uint32_t item_id_hands;
            uint32_t item_id_costume_body;
            uint32_t item_id_costume_head;
        };
    };
};
static_assert(sizeof(Equipment) == 0xD8, "Equipment size mismatch");

struct TagInfo {
    uint16_t guild_id;
    uint8_t primary;
    uint8_t secondary;
    uint16_t level;
};

struct AgentItem;
struct AgentGadget;
struct AgentLiving;

struct Agent {
    uint32_t* vtable;
    uint32_t h0004;
    uint32_t h0008;
    uint32_t h000c[2];
    uint32_t timer;
    uint32_t timer2;
    gw::GwLink<Agent> link;
    gw::GwLink<Agent> link2;
    uint32_t agent_id;
    float z;
    float width1;
    float height1;
    float width2;
    float height2;
    float width3;
    float height3;
    float rotation_angle;
    float rotation_cos;
    float rotation_sin;
    uint32_t name_properties;
    uint32_t ground;
    uint32_t h0060;
    gw::Vec3f terrain_normal;
    uint8_t h0070[4];
    union {
        struct {
            float x;
            float y;
            uint32_t plane;
        };
        gw::GamePos pos;
    };
    uint8_t h0080[4];
    float name_tag_x;
    float name_tag_y;
    float name_tag_z;
    uint16_t visual_effects;
    uint16_t h0092;
    uint32_t h0094[2];
    uint32_t type;
    union {
        struct {
            float move_x;
            float move_y;
        };
        gw::Vec2f velocity;
    };
    uint32_t h00a8;
    float rotation_cos2;
    float rotation_sin2;
    uint32_t h00b4[4];

    bool GetIsItemType() const { return (type & 0x400U) != 0; }
    bool GetIsGadgetType() const { return (type & 0x200U) != 0; }
    bool GetIsLivingType() const { return (type & 0xDBU) != 0; }

    AgentItem* GetAsAgentItem();
    AgentGadget* GetAsAgentGadget();
    AgentLiving* GetAsAgentLiving();

    const AgentItem* GetAsAgentItem() const;
    const AgentGadget* GetAsAgentGadget() const;
    const AgentLiving* GetAsAgentLiving() const;
};
static_assert(sizeof(Agent) == 0xC4, "Agent size mismatch");

struct AgentItem : Agent {
    uint32_t owner;
    uint32_t item_id;
    uint32_t h00cc;
    uint32_t extra_type;
};
static_assert(sizeof(AgentItem) == 0xD4, "AgentItem size mismatch");
static_assert(offsetof(AgentItem, owner) == 0xC4, "AgentItem offset mismatch");

struct AgentGadget : Agent {
    uint32_t h00c4;
    uint32_t h00c8;
    uint32_t extra_type;
    uint32_t gadget_id;
    uint32_t h00d4[4];
};
static_assert(sizeof(AgentGadget) == 0xE4, "AgentGadget size mismatch");
static_assert(offsetof(AgentGadget, h00c4) == 0xC4, "AgentGadget offset mismatch");

struct AgentLiving : Agent {
    uint32_t owner;
    uint32_t h00c8;
    uint32_t h00cc;
    uint32_t h00d0;
    uint32_t h00d4[3];
    float animation_type;
    uint32_t h00e4[2];
    float weapon_attack_speed;
    float attack_speed_modifier;
    uint16_t player_number;
    uint16_t agent_model_type;
    uint32_t transmog_npc_id;
    Equipment** equip;
    uint32_t h0100;
    uint32_t h0104;
    TagInfo* tags;
    uint16_t h010c;
    uint8_t primary;
    uint8_t secondary;
    uint8_t level;
    uint8_t team_id;
    uint8_t h0112[2];
    uint32_t h0114;
    float energy_regen;
    uint32_t h011c;
    float energy;
    uint32_t max_energy;
    uint32_t h0128;
    float hp_pips;
    uint32_t h0130;
    float hp;
    uint32_t max_hp;
    uint32_t effects;
    uint32_t h0140;
    uint8_t hex;
    uint8_t h0145[19];
    uint32_t model_state;
    uint32_t type_map;
    uint32_t h0160[4];
    uint32_t in_spirit_range;
    VisibleEffectList visible_effects;
    uint32_t h0180;
    uint32_t login_number;
    float animation_speed;
    uint32_t animation_code;
    uint32_t animation_id;
    uint8_t h0194[32];
    uint8_t dagger_status;
    gw::constants::Allegiance allegiance;
    uint16_t weapon_type;
    uint16_t skill;
    uint16_t h01ba;
    uint8_t weapon_item_type;
    uint8_t offhand_item_type;
    uint16_t weapon_item_id;
    uint16_t offhand_item_id;

    bool GetIsBleeding() const { return (effects & 0x0001U) != 0; }
    bool GetIsConditioned() const { return (effects & 0x0002U) != 0; }
    bool GetIsCrippled() const { return (effects & 0x000AU) == 0xAU; }
    bool GetIsDead() const { return (effects & 0x0010U) != 0; }
    bool GetIsDeepWounded() const { return (effects & 0x0020U) != 0; }
    bool GetIsPoisoned() const { return (effects & 0x0040U) != 0; }
    bool GetIsEnchanted() const { return (effects & 0x0080U) != 0; }
    bool GetIsDegenHexed() const { return (effects & 0x0400U) != 0; }
    bool GetIsHexed() const { return (effects & 0x0800U) != 0; }
    bool GetIsWeaponSpelled() const { return (effects & 0x8000U) != 0; }

    bool GetInCombatStance() const { return (type_map & 0x000001U) != 0; }
    bool GetHasQuest() const { return (type_map & 0x000002U) != 0; }
    bool GetIsDeadByTypeMap() const { return (type_map & 0x000008U) != 0; }
    bool GetIsFemale() const { return (type_map & 0x000200U) != 0; }
    bool GetHasBossGlow() const { return (type_map & 0x000400U) != 0; }
    bool GetIsHidingCape() const { return (type_map & 0x001000U) != 0; }
    bool GetCanBeViewedInPartyWindow() const { return (type_map & 0x20000U) != 0; }
    bool GetIsSpawned() const { return (type_map & 0x040000U) != 0; }
    bool GetIsBeingObserved() const { return (type_map & 0x400000U) != 0; }

    bool GetIsKnockedDown() const { return model_state == 1104U; }
    bool GetIsMoving() const { return model_state == 12U || model_state == 76U || model_state == 204U; }
    bool GetIsAttacking() const { return model_state == 96U || model_state == 1088U || model_state == 1120U; }
    bool GetIsCasting() const { return model_state == 65U || model_state == 581U; }
    bool GetIsIdle() const { return model_state == 68U || model_state == 64U || model_state == 100U; }

    bool GetIsAlive() const { return !GetIsDead() && hp > 0.0f; }
    bool IsPlayer() const { return login_number != 0; }
    bool IsNPC() const { return login_number == 0; }
};
static_assert(sizeof(AgentLiving) == 0x1C4, "AgentLiving size mismatch");
static_assert(offsetof(AgentLiving, owner) == 0xC4, "AgentLiving offset mismatch");

struct MapAgent {
    float cur_energy;
    float max_energy;
    float energy_regen;
    uint32_t skill_timestamp;
    float h0010;
    float max_energy2;
    float h0018;
    uint32_t h001c;
    float cur_health;
    float max_health;
    float health_regen;
    uint32_t h002c;
    uint32_t effects;

    bool GetIsBleeding() const { return (effects & 0x0001U) != 0; }
    bool GetIsConditioned() const { return (effects & 0x0002U) != 0; }
    bool GetIsCrippled() const { return (effects & 0x000AU) == 0xAU; }
    bool GetIsDead() const { return (effects & 0x0010U) != 0; }
    bool GetIsDeepWounded() const { return (effects & 0x0020U) != 0; }
    bool GetIsPoisoned() const { return (effects & 0x0040U) != 0; }
    bool GetIsEnchanted() const { return (effects & 0x0080U) != 0; }
    bool GetIsDegenHexed() const { return (effects & 0x0400U) != 0; }
    bool GetIsHexed() const { return (effects & 0x0800U) != 0; }
    bool GetIsWeaponSpelled() const { return (effects & 0x8000U) != 0; }
};
static_assert(sizeof(MapAgent) == 0x34, "MapAgent size mismatch");

struct AgentMovement {
    uint32_t h0000[3];
    uint32_t agent_id;
    uint32_t h0010[3];
    uint32_t agent_def;
    uint32_t h0020[6];
    uint32_t moving1;
    uint32_t h003c[2];
    uint32_t moving2;
    uint32_t h0048[7];
    gw::Vec3f h0064;
    uint32_t h0070;
    gw::Vec3f h0074;
};
static_assert(sizeof(AgentMovement) == 0x80, "AgentMovement size mismatch");

struct AgentInfo {
    uint32_t h0000[13];
    wchar_t* name_enc;
};
static_assert(sizeof(AgentInfo) == 0x38, "AgentInfo size mismatch");

using AgentList = gw::GwList<Agent>;
using AgentArray = gw::GwArray<Agent*>;
using MapAgentArray = gw::GwArray<MapAgent>;
using AgentInfoArray = gw::GwArray<AgentInfo>;
using AgentMovementArray = gw::GwArray<AgentMovement*>;

}  // namespace gw::context
