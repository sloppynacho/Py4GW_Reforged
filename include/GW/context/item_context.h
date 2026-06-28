#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"
#include "GW/context/item.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct InventoryTableEntry {
    uint32_t stride;
    uint32_t end;
    Inventory* start;
};
static_assert(sizeof(InventoryTableEntry) == 0xC, "InventoryTableEntry size mismatch");

struct ItemContext {
    gw::GwArray<void*> h0000;
    gw::GwArray<void*> h0010;
    uint32_t h0020;
    gw::GwArray<Bag*> bags_array;
    uint32_t h0034;
    uint32_t h0038;
    uint32_t h003c;
    gw::GwArray<void*> h0040;
    gw::GwArray<void*> h0050;
    uint32_t h0060;
    uint32_t h0064;
    uint32_t h0068;
    uint32_t h006c;
    uint32_t h0070;
    uint32_t h0074;
    uint32_t h0078;
    uint32_t h007c;
    uint32_t h0080;
    uint32_t h0084;
    uint32_t h0088;
    uint32_t h008c;
    uint32_t h0090;
    uint32_t h0094;
    uint32_t h0098;
    uint32_t h009c;
    uint32_t h00a0;
    uint32_t h00a4;
    uint32_t h00a8;
    uint32_t h00ac;
    uint32_t h00b0;
    uint32_t h00b4;
    ItemArray item_array;
    uint32_t h00c8;
    uint32_t h00cc;
    uint32_t h00d0;
    uint32_t h00d4;
    uint32_t h00d8;
    uint32_t h00dc;
    uint32_t h00e0;
    gw::GwArray<InventoryTableEntry> inventory_table;
    uint32_t h00f4;
    Inventory* inventory;
    gw::GwArray<void*> h00fc;
};

static_assert(offsetof(ItemContext, bags_array) == 0x24, "ItemContext::bags_array offset mismatch");
static_assert(offsetof(ItemContext, item_array) == 0xB8, "ItemContext::item_array offset mismatch");
static_assert(offsetof(ItemContext, inventory_table) == 0xE4, "ItemContext::inventory_table offset mismatch");
static_assert(offsetof(ItemContext, inventory) == 0xF8, "ItemContext::inventory offset mismatch");
static_assert(sizeof(ItemContext) == 0x10C, "ItemContext size mismatch");

}  // namespace gw::context
