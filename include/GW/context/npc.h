#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"

#include <cstdint>

namespace gw::context {

struct NPC {
    uint32_t model_file_id;
    uint32_t h0004;
    uint32_t scale;
    uint32_t sex;
    uint32_t npc_flags;
    uint32_t primary;
    uint32_t h0018;
    uint8_t default_level;
    uint8_t pad001D;
    uint16_t pad001E;
    wchar_t* name_enc;
    uint32_t* model_files;
    uint32_t files_count;
    uint32_t files_capacity;

    bool IsHenchman() const { return (npc_flags & 0x10U) != 0; }
    bool IsHero() const { return (npc_flags & 0x20U) != 0; }
    bool IsSpirit() const { return (npc_flags & 0x4000U) != 0; }
    bool IsMinion() const { return (npc_flags & 0x100U) != 0; }
    bool IsPet() const { return npc_flags == 0xDU; }
};
static_assert(sizeof(NPC) == 0x30, "NPC size mismatch");

using NPCArray = gw::GwArray<NPC>;

}  // namespace gw::context
