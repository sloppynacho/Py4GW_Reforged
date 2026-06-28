#pragma once

#include "base/error_handling.h"

#include "GW/common/game_pos.h"

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace gw::context {

struct Camera {
    uint32_t look_at_agent_id;
    uint32_t h0004;
    float h0008;
    float h000C;
    float max_distance;
    float h0014;
    float yaw;
    float pitch;
    float distance;
    uint32_t h0024[4];
    float yaw_right_click;
    float yaw_right_click2;
    float pitch_right_click;
    float distance2;
    float acceleration_constant;
    float time_since_last_keyboard_rotation;
    float time_since_last_mouse_rotation;
    float time_since_last_mouse_move;
    float time_since_last_agent_selection;
    float time_in_the_map;
    float time_in_the_district;
    float yaw_to_go;
    float pitch_to_go;
    float dist_to_go;
    float max_distance2;
    float h0070[2];
    Vec3f position;
    Vec3f camera_pos_to_go;
    Vec3f cam_pos_inverted;
    Vec3f cam_pos_inverted_to_go;
    Vec3f look_at_target;
    Vec3f look_at_to_go;
    float field_of_view;
    float field_of_view2;
    uint32_t h00C8;
    uint32_t h00CC;
    uint32_t h00D0;
    uint32_t h00D4;
    uint32_t h00D8;
    uint32_t h00DC;
    uint32_t h00E0;
    uint32_t h00E4;
    uint32_t h00E8;
    uint32_t h00EC;
    uint32_t h00F0;
    uint32_t h00F4;
    uint32_t h00F8;
    uint32_t h00FC;
    uint32_t h0100;
    uint32_t h0104;
    uint32_t h0108;
    uint32_t h010C;
    uint32_t h0110;
    uint32_t h0114;
    uint32_t h0118;
    uint32_t camera_mode;

    float GetYaw() const { return yaw; }
    float GetPitch() const { return pitch; }
    float GetFieldOfView() const { return field_of_view; }
    bool IsCameraUnlocked() const { return camera_mode == 3; }

    void SetYaw(float value) {
        yaw_to_go = value;
        yaw = value;
    }

    float GetCurrentYaw() const {
        const Vec3f dir = position - look_at_target;
        const float curtan = std::atan2(dir.y, dir.x);
        constexpr float kPi = 3.141592741f;
        return curtan >= 0.0f ? curtan - kPi : curtan + kPi;
    }

    void SetPitch(float value) {
        pitch_to_go = value;
    }

    float GetCameraZoom() const { return distance; }
    Vec3f GetLookAtTarget() const { return look_at_target; }
    Vec3f GetCameraPosition() const { return position; }

    void SetCameraPos(Vec3f new_position) {
        position = new_position;
    }

    void SetLookAtTarget(Vec3f new_position) {
        look_at_target = new_position;
    }
};

static_assert(offsetof(Camera, max_distance) == 0x10, "Camera::max_distance offset mismatch");
static_assert(offsetof(Camera, yaw) == 0x18, "Camera::yaw offset mismatch");
static_assert(offsetof(Camera, pitch) == 0x1C, "Camera::pitch offset mismatch");
static_assert(offsetof(Camera, distance) == 0x20, "Camera::distance offset mismatch");
static_assert(offsetof(Camera, position) == 0x78, "Camera::position offset mismatch");
static_assert(offsetof(Camera, look_at_target) == 0xA8, "Camera::look_at_target offset mismatch");
static_assert(offsetof(Camera, field_of_view) == 0xC0, "Camera::field_of_view offset mismatch");
static_assert(offsetof(Camera, camera_mode) == 0x11C, "Camera::camera_mode offset mismatch");

}  // namespace gw::context
