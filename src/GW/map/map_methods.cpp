#include "base/error_handling.h"

#include "GW/map/map.h"

#include "GW/context/agent_context.h"
#include "GW/context/cinematic.h"
#include "GW/context/char_context.h"
#include "GW/context/context.h"
#include "GW/context/game_context.h"
#include "GW/context/map_context.h"
#include "GW/context/world_context.h"

namespace {
struct InstanceInfo {
    void* terrain_info1;
    gw::constants::InstanceType instance_type;
    gw::context::AreaInfo* current_map_info;
    uint32_t terrain_count;
    void* terrain_info2;
};
}

namespace gw::map {

int QueryAltitude(const GamePos& pos, float radius, float& altitude, Vec3f* terrain_normal) {
    if (!g_query_altitude_func) {
        return 0;
    }
    return g_query_altitude_func(&pos, radius, &altitude, terrain_normal);
}

bool GetIsMapLoaded() {
    auto* game = context::GetGameContext();
    return game && game->map != nullptr;
}

gw::constants::MapID GetMapID() {
    auto* character = context::GetCharContext();
    return character ? character->current_map_id : gw::constants::MapID::Longeyes_Ledge_outpost;
}

bool GetIsMapUnlocked(gw::constants::MapID map_id) {
    auto* world = context::GetWorldContext();
    auto* unlocked_map = world && world->unlocked_map.valid() ? &world->unlocked_map : nullptr;
    if (!unlocked_map) {
        return false;
    }

    const uint32_t real_index = static_cast<uint32_t>(map_id) / 32U;
    if (real_index >= unlocked_map->size()) {
        return false;
    }
    const uint32_t shift = static_cast<uint32_t>(map_id) % 32U;
    const uint32_t flag = 1U << shift;
    return (unlocked_map->at(real_index) & flag) != 0;
}

gw::constants::ServerRegion GetRegion() {
    return g_region_id_addr ? *g_region_id_addr : gw::constants::ServerRegion::Unknown;
}

uintptr_t GetServerRegionPtr() {
    return reinterpret_cast<uintptr_t>(g_region_id_addr);
}

MapTypeInstanceInfo* GetMapTypeInstanceInfo(context::RegionType map_type) {
    const bool is_outpost = !(map_type == context::RegionType::ExplorableZone ||
        map_type == context::RegionType::MissionArea ||
        map_type == context::RegionType::Dungeon);
    for (size_t i = 0; i < g_map_type_instance_infos_size; ++i) {
        if (g_map_type_instance_infos[i].map_region_type == map_type &&
            g_map_type_instance_infos[i].is_outpost == is_outpost) {
            return &g_map_type_instance_infos[i];
        }
    }
    return nullptr;
}

gw::constants::Language GetLanguage() {
    auto* character = context::GetCharContext();
    return character ? character->language : gw::constants::Language::English;
}

bool GetIsObserving() {
    auto* character = context::GetCharContext();
    return character ? character->current_map_id != character->observe_map_id : false;
}

int GetDistrict() {
    auto* character = context::GetCharContext();
    return character ? character->district_number : 0;
}

uint32_t GetInstanceTime() {
    auto* agent = context::GetAgentContext();
    return agent ? agent->instance_timer : 0;
}

gw::constants::InstanceType GetInstanceType() {
    auto* info = g_instance_info_ptr
        ? *reinterpret_cast<InstanceInfo**>(g_instance_info_ptr)
        : nullptr;
    return info ? info->instance_type : gw::constants::InstanceType::Loading;
}

gw::constants::ServerRegion RegionFromDistrict(gw::constants::District district) {
    switch (district) {
    case gw::constants::District::International:
        return gw::constants::ServerRegion::International;
    case gw::constants::District::American:
        return gw::constants::ServerRegion::America;
    case gw::constants::District::EuropeEnglish:
    case gw::constants::District::EuropeFrench:
    case gw::constants::District::EuropeGerman:
    case gw::constants::District::EuropeItalian:
    case gw::constants::District::EuropeSpanish:
    case gw::constants::District::EuropePolish:
    case gw::constants::District::EuropeRussian:
        return gw::constants::ServerRegion::Europe;
    case gw::constants::District::AsiaKorean:
        return gw::constants::ServerRegion::Korea;
    case gw::constants::District::AsiaChinese:
        return gw::constants::ServerRegion::China;
    case gw::constants::District::AsiaJapanese:
        return gw::constants::ServerRegion::Japan;
    default:
        break;
    }
    return GetRegion();
}

gw::constants::Language LanguageFromDistrict(gw::constants::District district) {
    switch (district) {
    case gw::constants::District::EuropeFrench:
        return gw::constants::Language::French;
    case gw::constants::District::EuropeGerman:
        return gw::constants::Language::German;
    case gw::constants::District::EuropeItalian:
        return gw::constants::Language::Italian;
    case gw::constants::District::EuropeSpanish:
        return gw::constants::Language::Spanish;
    case gw::constants::District::EuropePolish:
        return gw::constants::Language::Polish;
    case gw::constants::District::EuropeRussian:
        return gw::constants::Language::Russian;
    case gw::constants::District::EuropeEnglish:
    case gw::constants::District::AsiaKorean:
    case gw::constants::District::AsiaChinese:
    case gw::constants::District::AsiaJapanese:
    case gw::constants::District::International:
    case gw::constants::District::American:
        return gw::constants::Language::English;
    default:
        break;
    }
    return GetLanguage();
}

context::MissionMapIconArray* GetMissionMapIconArray() {
    auto* world = context::GetWorldContext();
    return world && world->mission_map_icons.valid() ? &world->mission_map_icons : nullptr;
}

context::PathingMapArray* GetPathingMap() {
    auto* map_context = context::GetMapContext();
    if (!(map_context && map_context->sub1 && map_context->sub1->sub2)) {
        return nullptr;
    }
    return &map_context->sub1->sub2->pmaps;
}

uint32_t GetFoesKilled() {
    auto* world = context::GetWorldContext();
    return world ? world->foes_killed : 0;
}

uint32_t GetFoesToKill() {
    auto* world = context::GetWorldContext();
    return world ? world->foes_to_kill : 0;
}

context::AreaInfo* GetMapInfo(gw::constants::MapID map_id) {
    if (map_id == gw::constants::MapID::None) {
        map_id = GetMapID();
    }
    return g_area_info_addr &&
        map_id > gw::constants::MapID::None &&
        map_id < gw::constants::MapID::Count
        ? &g_area_info_addr[static_cast<uint32_t>(map_id)]
        : nullptr;
}

uintptr_t GetInstanceInfoPtr() {
    return g_instance_info_ptr;
}

bool GetIsInCinematic() {
    auto* game = context::GetGameContext();
    return game && game->cinematic ? game->cinematic->h0004 != 0 : false;
}

bool SkipCinematic() {
    if (!g_skip_cinematic_func) {
        return false;
    }
    g_skip_cinematic_func();
    return true;
}

bool CancelEnterChallenge() {
    if (!g_cancel_enter_challenge_mission_func) {
        return false;
    }
    g_cancel_enter_challenge_mission_func();
    return true;
}

}  // namespace gw::map
