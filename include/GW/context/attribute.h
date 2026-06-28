#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/gw_array.h"

#include <cstdint>

namespace gw::context {

struct Attribute {
    gw::constants::Attribute id;
    uint32_t level_base;
    uint32_t level;
    uint32_t decrement_points;
    uint32_t increment_points;
};
static_assert(sizeof(Attribute) == 0x14, "Attribute size mismatch");

struct AttributeInfo {
    gw::constants::Profession profession_id;
    gw::constants::Attribute attribute_id;
    uint32_t name_id;
    uint32_t desc_id;
    uint32_t is_pve;
};
static_assert(sizeof(AttributeInfo) == 0x14, "AttributeInfo size mismatch");

struct PartyAttribute {
    uint32_t agent_id;
    Attribute attribute[54];
};
static_assert(sizeof(PartyAttribute) == 0x43C, "PartyAttribute size mismatch");

using PartyAttributeArray = gw::GwArray<PartyAttribute>;

}  // namespace gw::context
