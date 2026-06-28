#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/gw_array.h"
#include "GW/common/game_pos.h"
#include "GW/context/agent.h"
#include "GW/context/attribute.h"
#include "GW/context/hero.h"
#include "GW/context/item.h"
#include "GW/context/map.h"
#include "GW/context/npc.h"
#include "GW/context/player.h"
#include "GW/context/quest.h"
#include "GW/context/skill.h"
#include "GW/context/title.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct PartyAlly {
    uint32_t agent_id;
    uint32_t h0004;
    uint32_t composite_id;
};
static_assert(sizeof(PartyAlly) == 0xC, "PartyAlly size mismatch");

struct ControlledMinions {
    uint32_t agent_id;
    uint32_t minion_count;
};
static_assert(sizeof(ControlledMinions) == 0x8, "ControlledMinions size mismatch");

struct DupeSkill {
    uint32_t skill_id;
    uint32_t count;
};
static_assert(sizeof(DupeSkill) == 0x8, "DupeSkill size mismatch");

struct ProfessionState {
    uint32_t agent_id;
    gw::constants::Profession primary;
    gw::constants::Profession secondary;
    uint32_t unlocked_professions;
    uint32_t h0010;

    bool IsProfessionUnlocked(gw::constants::Profession profession) const {
        return (unlocked_professions & (1U << static_cast<uint32_t>(profession))) != 0;
    }
};
static_assert(sizeof(ProfessionState) == 0x14, "ProfessionState size mismatch");

struct AccountInfo {
    wchar_t* account_name;
    uint32_t wins;
    uint32_t losses;
    uint32_t rating;
    uint32_t qualifier_points;
    uint32_t rank;
    uint32_t tournament_reward_points;
};
static_assert(sizeof(AccountInfo) == 0x1C, "AccountInfo size mismatch");

struct PartyMemberMoraleInfo {
    uint32_t agent_id;
    uint32_t agent_id_dup;
    uint32_t h0008[4];
    uint32_t morale;
};

struct PartyMoraleLink {
    uint32_t h0000;
    uint32_t h0004;
    PartyMemberMoraleInfo* party_member_info;
};
static_assert(sizeof(PartyMoraleLink) == 0xC, "PartyMoraleLink size mismatch");

struct PetInfo {
    uint32_t agent_id;
    uint32_t owner_agent_id;
    wchar_t* pet_name;
    uint32_t model_file_id1;
    uint32_t model_file_id2;
    HeroBehavior behavior;
    uint32_t locked_target_id;
};
static_assert(sizeof(PetInfo) == 0x1C, "PetInfo size mismatch");

struct PlayerControlledCharacter {
    uint32_t h0000[5];
    uint32_t agent_id;
    uint32_t composite_id;
    uint32_t h001c;
    uint32_t h0020;
    uint32_t h0024;
    uint32_t h0028;
    uint32_t h002c;
    uint32_t h0030;
    uint32_t h0034;
    uint32_t h0038;
    uint32_t h003c;
    uint32_t h0040;
    uint32_t h0044;
    uint32_t h0048;
    uint32_t h004c;
    uint32_t h0050;
    uint32_t h0054;
    uint32_t h0058;
    uint32_t h005c;
    uint32_t h0060;
    uint32_t more_flags;
    uint32_t h0068;
    uint32_t h006c;
    uint32_t h0070;
    uint32_t h0074;
    uint32_t h0078;
    uint32_t h007c;
    uint32_t h0080;
    uint32_t h0084;
    uint32_t h0088;
    uint32_t h008c;
    uint32_t h0090;
    uint32_t h0094;
    uint32_t h0098;
    uint32_t h009c;
    uint32_t h00a0;
    uint32_t h00a4;
    uint32_t h00a8;
    uint32_t h00ac;
    uint32_t h00b0;
    uint32_t h00b4;
    uint32_t h00b8;
    uint32_t h00bc;
    uint32_t h00c0;
    uint32_t h00c4;
    uint32_t h00c8;
    uint32_t h00cc;
    uint32_t h00d0;
    uint32_t h00d4;
    uint32_t h00d8;
    uint32_t h00dc;
    uint32_t h00e0;
    uint32_t h00e4;
    uint32_t h00e8;
    uint32_t h00ec;
    uint32_t h00f0;
    uint32_t h00f4;
    uint32_t h00f8;
    uint32_t h00fc;
    uint32_t h0100;
    uint32_t h0104;
    uint32_t h0108;
    uint32_t flags;
    uint32_t h0110;
    uint32_t h0114;
    uint32_t h0118;
    uint32_t h011c;
    uint32_t h0120;
    uint32_t h0124;
    uint32_t h0128;
    uint32_t h012c;
    uint32_t h0130;
};
static_assert(sizeof(PlayerControlledCharacter) == 0x134, "PlayerControlledCharacter size mismatch");

struct WorldContext {
    AccountInfo* account_info;
    gw::GwArray<wchar_t> message_buffer;
    gw::GwArray<wchar_t> dialog_buffer;
    MerchItemArray merch_items;
    MerchItemArray merch_items2;
    uint32_t accum_map_init_unk0;
    uint32_t accum_map_init_unk1;
    uint32_t accum_map_init_offset;
    uint32_t accum_map_init_length;
    uint32_t h0054;
    uint32_t accum_map_init_unk2;
    uint32_t h005c[8];
    MapAgentArray map_agents;
    gw::GwArray<PartyAlly> party_allies;
    gw::Vec3f all_flag;
    uint32_t h00a8;
    PartyAttributeArray attributes;
    uint32_t h00bc[255];
    gw::GwArray<void*> h04b8;
    gw::GwArray<void*> h04c8;
    uint32_t h04d8;
    gw::GwArray<void*> h04dc;
    uint32_t h04ec[7];
    AgentEffectsArray party_effects;
    gw::GwArray<void*> h0518;
    gw::constants::QuestID active_quest_id;
    QuestLog quest_log;
    uint32_t h053c[10];
    gw::GwArray<MissionObjective> mission_objectives;
    gw::GwArray<uint32_t> henchmen_agent_ids;
    HeroFlagArray hero_flags;
    HeroInfoArray hero_info;
    gw::GwArray<void*> cartographed_areas;
    uint32_t h05b4[2];
    gw::GwArray<ControlledMinions> controlled_minion_count;
    gw::GwArray<uint32_t> missions_completed;
    gw::GwArray<uint32_t> missions_bonus;
    gw::GwArray<uint32_t> missions_completed_hm;
    gw::GwArray<uint32_t> missions_bonus_hm;
    gw::GwArray<uint32_t> unlocked_map;
    uint32_t h061c[2];
    PartyMemberMoraleInfo* player_morale_info;
    uint32_t h0628;
    gw::GwArray<PartyMoraleLink> party_morale_related;
    uint32_t h063c[16];
    uint32_t player_number;
    PlayerControlledCharacter* player_controlled_character;
    uint32_t is_hard_mode_unlocked;
    uint32_t h0688[2];
    uint32_t salvage_session_id;
    uint32_t h0694[5];
    uint32_t player_team_token;
    gw::GwArray<PetInfo> pets;
    gw::GwArray<ProfessionState> party_profession_states;
    gw::GwArray<void*> h06cc;
    uint32_t h06dc;
    gw::GwArray<void*> h06e0;
    SkillbarArray skillbar;
    gw::GwArray<uint32_t> learnable_character_skills;
    gw::GwArray<uint32_t> unlocked_character_skills;
    gw::GwArray<DupeSkill> duplicated_character_skills;
    gw::GwArray<void*> h0730;
    uint32_t experience;
    uint32_t experience_dupe;
    uint32_t current_kurzick;
    uint32_t current_kurzick_dupe;
    uint32_t total_earned_kurzick;
    uint32_t total_earned_kurzick_dupe;
    uint32_t current_luxon;
    uint32_t current_luxon_dupe;
    uint32_t total_earned_luxon;
    uint32_t total_earned_luxon_dupe;
    uint32_t current_imperial;
    uint32_t current_imperial_dupe;
    uint32_t total_earned_imperial;
    uint32_t total_earned_imperial_dupe;
    uint32_t unk_faction4;
    uint32_t unk_faction4_dupe;
    uint32_t unk_faction5;
    uint32_t unk_faction5_dupe;
    uint32_t level;
    uint32_t level_dupe;
    uint32_t morale;
    uint32_t morale_dupe;
    uint32_t current_balth;
    uint32_t current_balth_dupe;
    uint32_t total_earned_balth;
    uint32_t total_earned_balth_dupe;
    uint32_t current_skill_points;
    uint32_t current_skill_points_dupe;
    uint32_t total_earned_skill_points;
    uint32_t total_earned_skill_points_dupe;
    uint32_t max_kurzick;
    uint32_t max_luxon;
    uint32_t max_balth;
    uint32_t max_imperial;
    uint32_t equipment_status;
    AgentInfoArray agent_infos;
    gw::GwArray<void*> h07dc;
    MissionMapIconArray mission_map_icons;
    NPCArray npcs;
    PlayerArray players;
    TitleArray titles;
    gw::GwArray<TitleTier> title_tiers;
    gw::GwArray<uint32_t> vanquished_areas;
    uint32_t foes_killed;
    uint32_t foes_to_kill;
};

static_assert(offsetof(WorldContext, merch_items) == 0x24, "WorldContext::merch_items offset mismatch");
static_assert(offsetof(WorldContext, map_agents) == 0x7C, "WorldContext::map_agents offset mismatch");
static_assert(offsetof(WorldContext, attributes) == 0xAC, "WorldContext::attributes offset mismatch");
static_assert(offsetof(WorldContext, party_effects) == 0x508, "WorldContext::party_effects offset mismatch");
static_assert(offsetof(WorldContext, active_quest_id) == 0x528, "WorldContext::active_quest_id offset mismatch");
static_assert(offsetof(WorldContext, hero_flags) == 0x584, "WorldContext::hero_flags offset mismatch");
static_assert(offsetof(WorldContext, player_number) == 0x67C, "WorldContext::player_number offset mismatch");
static_assert(offsetof(WorldContext, player_controlled_character) == 0x680, "WorldContext::player_controlled_character offset mismatch");
static_assert(offsetof(WorldContext, pets) == 0x6AC, "WorldContext::pets offset mismatch");
static_assert(offsetof(WorldContext, skillbar) == 0x6F0, "WorldContext::skillbar offset mismatch");
static_assert(offsetof(WorldContext, agent_infos) == 0x7CC, "WorldContext::agent_infos offset mismatch");
static_assert(offsetof(WorldContext, players) == 0x80C, "WorldContext::players offset mismatch");
static_assert(offsetof(WorldContext, title_tiers) == 0x82C, "WorldContext::title_tiers offset mismatch");
static_assert(sizeof(WorldContext) == 0x854, "WorldContext size mismatch");

}  // namespace gw::context
