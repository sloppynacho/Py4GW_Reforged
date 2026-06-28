#pragma once

#include "base/error_handling.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct LoginCharacter {
    uint32_t appearance_packed;
    uint32_t pvp_flag;
    uint32_t guild_guid_0;
    uint32_t guild_guid_1;
    uint32_t guild_guid_2;
    uint32_t guild_guid_3;
    void* items_data;
    uint32_t items_capacity;
    uint32_t items_count;
    uint32_t items_param;
    uint32_t level;
    uint32_t current_map_id;
    uint32_t field_0x30;
    uint32_t primary_profession;
    uint32_t profession_enum;
    uint32_t field_0x3c;
    uint32_t field_0x40;
    uint32_t field_0x44;
    uint32_t field_0x48;
    void* char_model_ptr;
    wchar_t character_name[20];
};
static_assert(sizeof(LoginCharacter) == 0x78, "LoginCharacter size mismatch");

struct PreGameContext {
    uint32_t frame_id;
    uint32_t scene_type;
    uint32_t scene_controller_iface;
    float camera_pitch_frequency;
    float camera_pitch_current;
    float camera_pitch_target;
    float camera_pitch_velocity;
    uint32_t h001c[12];
    uint32_t camera_mode;
    uint32_t h0050[5];
    uint32_t h0064;
    float camera_limits_frequency;
    float camera_limits_min_current;
    float camera_limits_max_current;
    float camera_limits_min_target;
    float camera_limits_max_target;
    float camera_limits_min_velocity;
    float camera_limits_max_velocity;
    float scroll_offset_frequency;
    float scroll_offset_current;
    float scroll_offset_target;
    float scroll_offset_velocity;
    float scroll_speed_frequency;
    float scroll_speed_current;
    float scroll_speed_target;
    float scroll_speed_velocity;
    float camera_height;
    float camera_height_min;
    float camera_height_max;
    float camera_rotation_frequency;
    float camera_rotation_current;
    float camera_rotation_target;
    float camera_rotation_velocity;
    uint32_t h00c0[4];
    uint32_t max_characters;
    int32_t chosen_character_index;
    int32_t preview_character_index;
    int32_t pending_character_index;
    LoginCharacter* chars_buffer;
    uint32_t chars_capacity;
    uint32_t chars_count;
    int32_t char_creation_flag;
    int32_t create_slot_index;
    uint32_t sentinel_guard;
    void* self_link;
    uint32_t list_head;
};

static_assert(offsetof(PreGameContext, camera_mode) == 0x4C, "PreGameContext::camera_mode offset mismatch");
static_assert(offsetof(PreGameContext, max_characters) == 0xD0, "PreGameContext::max_characters offset mismatch");
static_assert(sizeof(PreGameContext) == 0x100, "PreGameContext size mismatch");

}  // namespace gw::context
