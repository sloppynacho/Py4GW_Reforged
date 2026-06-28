#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct GadgetInfo {
    uint32_t h0000;
    uint32_t h0004;
    uint32_t h0008;
    wchar_t* name_enc;
};
static_assert(sizeof(GadgetInfo) == 0x10, "GadgetInfo size mismatch");

struct GadgetContext {
    gw::GwArray<GadgetInfo> gadget_info;
};

static_assert(sizeof(GadgetContext) == 0x10, "GadgetContext size mismatch");

}  // namespace gw::context
