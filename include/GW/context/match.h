#pragma once

#include "base/error_handling.h"

#include <cstdint>

namespace gw::context {

struct ObserverMatch {
    uint32_t match_id;
    uint32_t match_id_dup;
    uint32_t map_id;
    uint32_t age;
    struct {
        uint32_t type;
        uint32_t reserved;
        uint32_t version;
        uint32_t state;
        uint32_t level;
        uint32_t config1;
        uint32_t config2;
        uint32_t score1;
        uint32_t score2;
        uint32_t score3;
        uint32_t stat1;
        uint32_t stat2;
        uint32_t data1;
        uint32_t data2;
    } flags;
    wchar_t* team_names;
    uint32_t unknown1[0xA];
    wchar_t* team_names2;
};
static_assert(sizeof(ObserverMatch) == 0x78, "ObserverMatch size mismatch");

}  // namespace gw::context
