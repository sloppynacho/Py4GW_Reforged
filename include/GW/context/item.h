#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/constants/item_ids.h"
#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace gw::context {

struct Item;
using ItemArray = gw::GwArray<Item*>;

enum class BagType {
    None,
    Inventory,
    Equipped,
    NotCollected,
    Storage,
    MaterialStorage
};

enum class DyeColor : uint8_t {
    None = 0,
    Blue = 2,
    Green = 3,
    Purple = 4,
    Red = 5,
    Yellow = 6,
    Brown = 7,
    Orange = 8,
    Silver = 9,
    Black = 10,
    Gray = 11,
    White = 12,
    Pink = 13
};

struct DyeInfo {
    uint8_t dye_tint;
    DyeColor dye1 : 4;
    DyeColor dye2 : 4;
    DyeColor dye3 : 4;
    DyeColor dye4 : 4;
};
static_assert(sizeof(DyeInfo) == 0x3, "DyeInfo size mismatch");

struct ItemData {
    uint32_t model_file_id = 0;
    gw::constants::ItemType type = static_cast<gw::constants::ItemType>(0xff);
    DyeInfo dye{};
    uint32_t value = 0;
    uint32_t interaction = 0;
};
static_assert(sizeof(ItemData) == 0x10, "ItemData size mismatch");

struct MaterialCost {
    gw::constants::MaterialSlot material;
    uint32_t amount;
    uint32_t h0008;
    uint32_t h000c;
};
static_assert(sizeof(MaterialCost) == 0x10, "MaterialCost size mismatch");

struct ItemFormula {
    uint32_t h0000;
    uint32_t gold_cost;
    uint32_t skill_point_cost;
    uint32_t material_cost_count;
    MaterialCost* material_cost_buffer;
};
static_assert(sizeof(ItemFormula) == 0x14, "ItemFormula size mismatch");

struct Bag {
    BagType bag_type;
    uint32_t index;
    uint32_t unknown_0;
    uint32_t container_item;
    uint32_t items_count;
    Bag* bag_array;
    ItemArray items;

    bool IsInventoryBag() const { return bag_type == BagType::Inventory; }
    bool IsStorageBag() const { return bag_type == BagType::Storage; }
    bool IsMaterialStorage() const { return bag_type == BagType::MaterialStorage; }

    static constexpr size_t npos = static_cast<size_t>(-1);

    size_t find_dye(uint32_t model_id, DyeInfo extra_id, size_t pos = 0) const;
    size_t find1(uint32_t model_id, size_t pos = 0) const;
    size_t find2(const Item* item, size_t pos = 0) const;

    [[nodiscard]] gw::constants::Bag bag_id() const {
        return static_cast<gw::constants::Bag>(index + 1U);
    }
};
static_assert(sizeof(Bag) == 0x28, "Bag size mismatch");

struct ItemModifier {
    uint32_t mod = 0;

    uint32_t identifier() const { return mod >> 16; }
    uint32_t arg1() const { return (mod & 0x0000FF00U) >> 8; }
    uint32_t arg2() const { return mod & 0x000000FFU; }
    uint32_t arg() const { return mod & 0x0000FFFFU; }
    explicit operator bool() const { return mod != 0; }
};
static_assert(sizeof(ItemModifier) == 0x4, "ItemModifier size mismatch");

struct Item {
    uint32_t item_id;
    uint32_t agent_id;
    Bag* bag_equipped;
    Bag* bag;
    ItemModifier* mod_struct;
    uint32_t mod_struct_size;
    wchar_t* customized;
    uint32_t model_file_id;
    gw::constants::ItemType type;
    DyeInfo dye;
    uint16_t value;
    uint16_t h0026;
    uint32_t interaction;
    uint32_t model_id;
    wchar_t* info_string;
    wchar_t* name_enc;
    wchar_t* complete_name_enc;
    wchar_t* single_item_name;
    uint32_t h0040[2];
    uint16_t item_formula;
    uint8_t is_material_salvageable;
    uint8_t h004b;
    uint16_t quantity;
    uint8_t equipped;
    uint8_t profession;
    uint8_t slot;

    bool GetIsStackable() const { return (interaction & 0x80000U) != 0; }
    bool GetIsInscribable() const { return (interaction & 0x08000000U) != 0; }

    bool GetIsMaterial() const;
    bool GetIsZcoin() const;
    ItemModifier* GetModifier(uint32_t identifier) const;
};
static_assert(sizeof(Item) == 0x54, "Item size mismatch");

struct WeaponSet {
    Item* weapon;
    Item* offhand;
};
static_assert(sizeof(WeaponSet) == 0x8, "WeaponSet size mismatch");

struct Inventory {
    union {
        Bag* bags[23];
        struct {
            Bag* unused_bag;
            Bag* backpack;
            Bag* belt_pouch;
            Bag* bag1;
            Bag* bag2;
            Bag* equipment_pack;
            Bag* material_storage;
            Bag* unclaimed_items;
            Bag* storage1;
            Bag* storage2;
            Bag* storage3;
            Bag* storage4;
            Bag* storage5;
            Bag* storage6;
            Bag* storage7;
            Bag* storage8;
            Bag* storage9;
            Bag* storage10;
            Bag* storage11;
            Bag* storage12;
            Bag* storage13;
            Bag* storage14;
            Bag* equipped_items;
        };
    };
    Item* bundle;
    uint32_t storage_panes_unlocked;
    union {
        WeaponSet weapon_sets[4];
        struct {
            Item* weapon_set0;
            Item* offhand_set0;
            Item* weapon_set1;
            Item* offhand_set1;
            Item* weapon_set2;
            Item* offhand_set2;
            Item* weapon_set3;
            Item* offhand_set3;
        };
    };
    uint32_t active_weapon_set;
    uint32_t h0088[2];
    uint32_t gold_character;
    uint32_t gold_storage;
};
static_assert(sizeof(Inventory) == 0x98, "Inventory size mismatch");

struct PvPItemUpgradeInfo {
    uint32_t file_id;
    uint32_t name_id;
    uint32_t upgrade_type;
    uint32_t campaign_id;
    uint32_t interaction;
    uint32_t is_dev;
    uint32_t profession;
    uint32_t h0018;
    uint32_t mod_struct_size;
    uint32_t* mod_struct;
};
static_assert(sizeof(PvPItemUpgradeInfo) == 0x28, "PvPItemUpgradeInfo size mismatch");

struct PvPItemInfo {
    uint32_t unk[9];
};
static_assert(sizeof(PvPItemInfo) == 0x24, "PvPItemInfo size mismatch");

struct CompositeModelInfo {
    uint32_t class_flags;
    uint32_t file_ids[11];
};
static_assert(sizeof(CompositeModelInfo) == 0x30, "CompositeModelInfo size mismatch");

struct SalvageSessionInfo {
    void* vtable;
    uint32_t frame_id;
    uint32_t item_id;
    uint32_t salvagable_1;
    uint32_t salvagable_2;
    uint32_t salvagable_3;
    uint32_t chosen_salvagable;
    uint32_t h001c;
    uint32_t kit_id;
};
static_assert(sizeof(SalvageSessionInfo) == 0x24, "SalvageSessionInfo size mismatch");

using MerchItemArray = gw::GwArray<uint32_t>;

}  // namespace gw::context
