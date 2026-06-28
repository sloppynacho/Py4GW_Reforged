#include "base/error_handling.h"

#include "GW/map/map.h"

#include "base/CrashHandler.h"
#include "base/logger.h"
#include "base/memory_patcher.h"
#include "base/patterns.h"
#include "base/scanner.h"

namespace {

struct MapDimensions {
    uint32_t unk;
    uint32_t start_x;
    uint32_t start_y;
    uint32_t end_x;
    uint32_t end_y;
    uint32_t unk1;
};

struct InstanceInfo {
    MapDimensions* terrain_info1;
    gw::constants::InstanceType instance_type;
    gw::context::AreaInfo* current_map_info;
    uint32_t terrain_count;
    MapDimensions* terrain_info2;
};

InstanceInfo* g_instance_info = nullptr;
py4gw::MemoryPatcher g_bypass_tolerance_patch;

bool ResolveSkipCinematic() {
    CrashContextScope context("startup", "map", "resolve_skip_cinematic");
    const auto* pattern = py4gw::Patterns::Get("map.skip_cinematic");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.skip_cinematic", "map");
        return false;
    }

    const uintptr_t address = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    gw::map::g_skip_cinematic_func = reinterpret_cast<gw::map::VoidFn>(address);
    return Logger::AssertAddress("SkipCinematic_Func", reinterpret_cast<uintptr_t>(gw::map::g_skip_cinematic_func), "map");
}

bool ResolveRegionId() {
    CrashContextScope context("startup", "map", "resolve_region_id");
    const auto* pattern = py4gw::Patterns::Get("map.region_id_ref");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.region_id_ref", "map");
        return false;
    }

    const uintptr_t address = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("RegionId_Ref", address, "map")) {
        return false;
    }
    const uintptr_t candidate = *reinterpret_cast<const uintptr_t*>(address);
    if (!Logger::AssertAddress("RegionId_Ptr", candidate, "map")) {
        return false;
    }
    gw::map::g_region_id_addr = reinterpret_cast<gw::constants::ServerRegion*>(candidate);
    return true;
}

bool ResolveAreaInfo() {
    CrashContextScope context("startup", "map", "resolve_area_info");
    const auto* pattern = py4gw::Patterns::Get("map.area_info_ref");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.area_info_ref", "map");
        return false;
    }

    const uintptr_t address = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("AreaInfo_Ref", address, "map")) {
        return false;
    }

    const uintptr_t candidate = *reinterpret_cast<const uintptr_t*>(address);
    if (!Logger::AssertAddress("AreaInfo_Ptr", candidate, "map")) {
        return false;
    }
    if (!py4gw::Scanner::IsValidPtr(candidate, py4gw::ScannerSection::RData)) {
        Logger::Instance().LogError("Area info pointer is outside the expected rdata section.", "map");
        return false;
    }

    gw::map::g_area_info_addr = reinterpret_cast<gw::context::AreaInfo*>(candidate);
    return true;
}

bool ResolveInstanceInfo() {
    CrashContextScope context("startup", "map", "resolve_instance_info");
    const auto* pattern = py4gw::Patterns::Get("map.instance_info_ref");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.instance_info_ref", "map");
        return false;
    }

    const uintptr_t address = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("InstanceInfo_Ref", address, "map")) {
        return false;
    }

    const uintptr_t candidate = *reinterpret_cast<const uintptr_t*>(address);
    if (!Logger::AssertAddress("InstanceInfo_Ptr", candidate, "map")) {
        return false;
    }

    g_instance_info = reinterpret_cast<InstanceInfo*>(candidate);
    gw::map::g_instance_info_ptr = address;
    return true;
}

bool ResolveQueryAltitude() {
    CrashContextScope context("startup", "map", "resolve_query_altitude");
    const auto* pattern = py4gw::Patterns::Get("map.query_altitude_callsite");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.query_altitude_callsite", "map");
        return false;
    }

    const uintptr_t callsite = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("QueryAltitude_Callsite", callsite, "map")) {
        return false;
    }

    gw::map::g_query_altitude_func = reinterpret_cast<gw::map::QueryAltitudeFn>(
        py4gw::Scanner::FunctionFromNearCall(callsite));
    return Logger::AssertAddress("QueryAltitude_Func", reinterpret_cast<uintptr_t>(gw::map::g_query_altitude_func), "map");
}

bool ResolveBypassTolerancePatch() {
    CrashContextScope context("startup", "map", "resolve_bypass_tolerance_patch");
    const auto* pattern = py4gw::Patterns::Get("map.bypass_tolerance_patch");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.bypass_tolerance_patch", "map");
        return false;
    }

    const uintptr_t address = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("BypassTolerancePatch_Addr", address, "map")) {
        return false;
    }

    static constexpr char patch[] = "\xEB";
    g_bypass_tolerance_patch.SetPatch(address, patch, 1);
    return g_bypass_tolerance_patch.IsValid();
}

bool ResolveEnterChallengeFunctions() {
    CrashContextScope context("startup", "map", "resolve_enter_challenge_functions");
    const auto* pattern = py4gw::Patterns::Get("map.enter_challenge_anchor");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.enter_challenge_anchor", "map");
        return false;
    }

    const uintptr_t address = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("EnterChallenge_Anchor", address, "map")) {
        return false;
    }

    gw::map::g_cancel_enter_challenge_mission_func = reinterpret_cast<gw::map::VoidFn>(
        py4gw::Scanner::FunctionFromNearCall(address + 0x1B));
    return Logger::AssertAddress(
        "CancelEnterChallengeMission_Func",
        reinterpret_cast<uintptr_t>(gw::map::g_cancel_enter_challenge_mission_func),
        "map");
}

bool ResolveMapTypeInstanceInfos() {
    CrashContextScope context("startup", "map", "resolve_map_type_instance_infos");
    const auto* pattern = py4gw::Patterns::Get("map.map_type_instance_infos_ref");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: map.map_type_instance_infos_ref", "map");
        return false;
    }

    const uintptr_t address = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("MapTypeInstanceInfos_Anchor", address, "map")) {
        return false;
    }

    gw::map::g_map_type_instance_infos_size = (*reinterpret_cast<const uint32_t*>(address + 5)) / sizeof(gw::map::MapTypeInstanceInfo);
    const uintptr_t candidate = *reinterpret_cast<const uintptr_t*>(address + 0x1A);
    if (!Logger::AssertAddress("MapTypeInstanceInfos_Ptr", candidate, "map")) {
        return false;
    }

    gw::map::g_map_type_instance_infos = reinterpret_cast<gw::map::MapTypeInstanceInfo*>(candidate);
    return true;
}

bool Init() {
    CrashContextScope context("startup", "map", "init");
    return ResolveSkipCinematic() &&
        ResolveRegionId() &&
        ResolveAreaInfo() &&
        ResolveInstanceInfo() &&
        ResolveQueryAltitude() &&
        ResolveBypassTolerancePatch() &&
        ResolveEnterChallengeFunctions() &&
        ResolveMapTypeInstanceInfos();
}

void Exit() {
    CrashContextScope context("shutdown", "map", "exit");
    g_bypass_tolerance_patch.TogglePatch(false);
    g_bypass_tolerance_patch.Reset();
    g_instance_info = nullptr;
    gw::map::g_skip_cinematic_func = nullptr;
    gw::map::g_cancel_enter_challenge_mission_func = nullptr;
    gw::map::g_query_altitude_func = nullptr;
    gw::map::g_region_id_addr = nullptr;
    gw::map::g_area_info_addr = nullptr;
    gw::map::g_map_type_instance_infos = nullptr;
    gw::map::g_map_type_instance_infos_size = 0;
    gw::map::g_instance_info_ptr = 0;
}

}  // namespace

namespace gw::map {

QueryAltitudeFn g_query_altitude_func = nullptr;
VoidFn g_skip_cinematic_func = nullptr;
VoidFn g_cancel_enter_challenge_mission_func = nullptr;
gw::constants::ServerRegion* g_region_id_addr = nullptr;
context::AreaInfo* g_area_info_addr = nullptr;
MapTypeInstanceInfo* g_map_type_instance_infos = nullptr;
uint32_t g_map_type_instance_infos_size = 0;
uintptr_t g_instance_info_ptr = 0;
std::atomic<bool> g_initialized = false;

bool Initialize() {
    CrashContextScope context("startup", "map", "initialize");
    if (g_initialized) {
        return true;
    }

    PY4GW_ASSERT(py4gw::Scanner::Initialize());
    PY4GW_ASSERT(py4gw::Patterns::Initialize());

    if (!Init()) {
        Exit();
        return false;
    }

    if (g_bypass_tolerance_patch.IsValid()) {
        g_bypass_tolerance_patch.TogglePatch(true);
    }
    g_initialized = true;
    return true;
}

void Shutdown() {
    CrashContextScope context("shutdown", "map", "shutdown");
    if (!g_initialized) {
        return;
    }

    Exit();
    g_initialized = false;
}

}  // namespace gw::map
