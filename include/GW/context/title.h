#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/gw_array.h"

#include <cstdint>

namespace gw::context {

struct Title {
    uint32_t props;
    uint32_t current_points;
    uint32_t current_title_tier_index;
    uint32_t points_needed_current_rank;
    uint32_t next_title_tier_index;
    uint32_t points_needed_next_rank;
    uint32_t max_title_rank;
    uint32_t max_title_tier_index;
    uint32_t h0020;
    wchar_t* points_desc;
    wchar_t* h0028;

    bool IsPercentageBased() const { return (props & 1U) != 0; }
    bool HasTiers() const { return (props & 3U) == 2U; }
};
static_assert(sizeof(Title) == 0x2C, "Title size mismatch");

struct TitleTier {
    uint32_t props;
    uint32_t tier_number;
    wchar_t* tier_name_enc;

    bool IsPercentageBased() const { return (props & 1U) != 0; }
};
static_assert(sizeof(TitleTier) == 0xC, "TitleTier size mismatch");

struct TitleClientData {
    gw::constants::TitleID title_id;
    uint32_t name_id;
};

using TitleArray = gw::GwArray<Title>;

}  // namespace gw::context
