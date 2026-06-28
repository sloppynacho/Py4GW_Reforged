#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"
#include "GW/common/gw_list.h"
#include "GW/context/pathing.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct PropsContext {
    uint32_t h0000[0x1B];
    gw::GwArray<gw::GwList<PropByType>> props_by_type;
    uint32_t h007c[0xA];
    gw::GwArray<PropModelInfo> prop_models;
    uint32_t h00b4[0x38];
    gw::GwArray<MapProp*> prop_array;
};
static_assert(sizeof(PropsContext) == 0x1A4, "PropsContext size mismatch");

struct MapContextSub2 {
    uint32_t h0000[6];
    PathingMapArray pmaps;
};

struct MapContextSub1 {
    MapContextSub2* sub2;
    gw::GwArray<uint32_t> pathing_map_block;
    uint32_t total_trapezoid_count;
    uint32_t h001c[0x12];
    gw::GwArray<gw::GwList<void*>> something_else_for_props;
};

struct MapContext {
    float map_boundaries[5];
    uint32_t h0014[6];
    gw::GwArray<void*> spawns1;
    gw::GwArray<void*> spawns2;
    gw::GwArray<void*> spawns3;
    float h005c[6];
    MapContextSub1* sub1;
    uint8_t h0078[4];
    PropsContext* props;
    uint32_t h0080;
    void* terrain;
    uint32_t h0088[42];
    void* zones;
};

static_assert(offsetof(MapContext, sub1) == 0x74, "MapContext::sub1 offset mismatch");
static_assert(offsetof(MapContext, props) == 0x7C, "MapContext::props offset mismatch");
static_assert(offsetof(MapContext, terrain) == 0x84, "MapContext::terrain offset mismatch");
static_assert(offsetof(MapContext, zones) == 0x130, "MapContext::zones offset mismatch");

}  // namespace gw::context
