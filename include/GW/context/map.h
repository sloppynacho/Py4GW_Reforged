#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/gw_array.h"

#include <cstdint>

namespace gw::context {

struct MissionMapIcon {
    uint32_t index;
    float X;
    float Y;
    uint32_t h000C;
    uint32_t h0010;
    uint32_t option;
    uint32_t h0018;
    uint32_t model_id;
    uint32_t h0020;
    uint32_t h0024;
};
static_assert(sizeof(MissionMapIcon) == 0x28, "MissionMapIcon size mismatch");

using MissionMapIconArray = gw::GwArray<MissionMapIcon>;

enum class Continent : uint32_t {
    Kryta,
    DevContinent,
    Cantha,
    BattleIsles,
    Elona,
    RealmOfTorment
};

enum class RegionType : uint32_t {
    AllianceBattle,
    Arena,
    ExplorableZone,
    GuildBattleArea,
    GuildHall,
    MissionOutpost,
    CooperativeMission,
    CompetitiveMission,
    EliteMission,
    Challenge,
    Outpost,
    ZaishenBattle,
    HeroesAscent,
    City,
    MissionArea,
    HeroBattleOutpost,
    HeroBattleArea,
    EotnMission,
    Dungeon,
    Marketplace,
    Unknown,
    DevRegion
};

enum Region : uint32_t {
    Region_Kryta,
    Region_Maguuma,
    Region_Ascalon,
    Region_NorthernShiverpeaks,
    Region_HeroesAscent,
    Region_CrystalDesert,
    Region_FissureOfWoe,
    Region_Presearing,
    Region_Kaineng,
    Region_Kurzick,
    Region_Luxon,
    Region_ShingJea,
    Region_Kourna,
    Region_Vaabi,
    Region_Desolation,
    Region_Istan,
    Region_DomainOfAnguish,
    Region_TarnishedCoast,
    Region_DepthsOfTyria,
    Region_FarShiverpeaks,
    Region_CharrHomelands,
    Region_BattleIslands,
    Region_TheBattleOfJahai,
    Region_TheFlightNorth,
    Region_TheTenguAccords,
    Region_TheRiseOfTheWhiteMantle,
    Region_Swat,
    Region_DevRegion
};

struct AreaInfo {
    gw::constants::Campaign campaign;
    Continent continent;
    Region region;
    RegionType type;
    uint32_t flags;
    uint32_t thumbnail_id;
    uint32_t min_party_size;
    uint32_t max_party_size;
    uint32_t min_player_size;
    uint32_t max_player_size;
    uint32_t controlled_outpost_id;
    uint32_t fraction_mission;
    uint32_t min_level;
    uint32_t max_level;
    uint32_t needed_pq;
    uint32_t mission_maps_to;
    uint32_t x;
    uint32_t y;
    uint32_t icon_start_x;
    uint32_t icon_start_y;
    uint32_t icon_end_x;
    uint32_t icon_end_y;
    uint32_t icon_start_x_dupe;
    uint32_t icon_start_y_dupe;
    uint32_t icon_end_x_dupe;
    uint32_t icon_end_y_dupe;
    uint32_t file_id;
    uint32_t mission_chronology;
    uint32_t ha_map_chronology;
    uint32_t name_id;
    uint32_t description_id;

    uint32_t FileId1() const { return ((file_id - 1U) % 0xFF00U) + 0x100U; }
    uint32_t FileId2() const { return ((file_id - 1U) / 0xFF00U) + 0x100U; }
    bool GetHasEnterButton() const { return (flags & 0x100U) != 0 || (flags & 0x40000U) != 0; }
    bool GetIsOnWorldMap() const { return (flags & 0x20U) == 0; }
    bool GetIsPvP() const { return (flags & 0x40001U) != 0; }
    bool GetIsGuildHall() const { return (flags & 0x800000U) != 0; }
    bool GetIsVanquishableArea() const { return (flags & 0x10000000U) != 0; }
    bool GetIsUnlockable() const { return (flags & 0x10000U) != 0; }
    bool GetHasMissionMapsTo() const { return (flags & 0x8000000U) != 0; }
};
static_assert(sizeof(AreaInfo) == 0x7C, "AreaInfo size mismatch");

}  // namespace gw::context
