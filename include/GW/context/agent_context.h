#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct AgentMovement;

struct AgentSummaryInfoSub {
    uint32_t h0000;
    uint32_t h0004;
    uint32_t gadget_id;
    uint32_t h000c;
    wchar_t* gadget_name_enc;
    uint32_t h0014;
    uint32_t composite_agent_id;
};
static_assert(sizeof(AgentSummaryInfoSub) == 0x1C, "AgentSummaryInfoSub size mismatch");

struct AgentSummaryInfo {
    uint32_t h0000;
    uint32_t h0004;
    AgentSummaryInfoSub* extra_info_sub;
};
static_assert(sizeof(AgentSummaryInfo) == 0xC, "AgentSummaryInfo size mismatch");

struct AgentContext {
    gw::GwArray<void*> h0000;
    uint32_t h0010[5];
    uint32_t h0024;
    uint32_t h0028[2];
    uint32_t h0030;
    uint32_t h0034[2];
    uint32_t h003c;
    uint32_t h0040[2];
    uint32_t h0048;
    uint32_t h004c[2];
    uint32_t h0054;
    uint32_t h0058[11];
    gw::GwArray<void*> h0084;
    uint32_t h0094;
    gw::GwArray<AgentSummaryInfo> agent_summary_info;
    gw::GwArray<void*> h00a8;
    gw::GwArray<void*> h00b8;
    uint32_t rand1;
    uint32_t rand2;
    uint8_t h00d0[24];
    gw::GwArray<AgentMovement*> agent_movement;
    gw::GwArray<void*> h00f8;
    uint32_t h0108[0x11];
    gw::GwArray<void*> agent_array1;
    gw::GwArray<void*> agent_async_movement;
    uint32_t h016c[0x10];
    uint32_t instance_timer;
};

static_assert(offsetof(AgentContext, agent_summary_info) == 0x98, "AgentContext::agent_summary_info offset mismatch");
static_assert(offsetof(AgentContext, agent_movement) == 0xE8, "AgentContext::agent_movement offset mismatch");
static_assert(offsetof(AgentContext, agent_array1) == 0x14C, "AgentContext::agent_array1 offset mismatch");
static_assert(offsetof(AgentContext, agent_async_movement) == 0x15C, "AgentContext::agent_async_movement offset mismatch");
static_assert(offsetof(AgentContext, instance_timer) == 0x1AC, "AgentContext::instance_timer offset mismatch");

}  // namespace gw::context
