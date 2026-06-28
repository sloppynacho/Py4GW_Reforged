#pragma once

#include "base/error_handling.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct GameplayContext {
    uint32_t h0000[0x13];
    float mission_map_zoom;
    uint32_t h0050[10];
};

static_assert(offsetof(GameplayContext, mission_map_zoom) == 0x4C, "GameplayContext::mission_map_zoom offset mismatch");
static_assert(sizeof(GameplayContext) == 0x78, "GameplayContext size mismatch");

}  // namespace gw::context
