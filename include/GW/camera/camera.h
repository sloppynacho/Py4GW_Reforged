#pragma once

#include "base/error_handling.h"

#include "GW/context/camera.h"
#include "base/memory_patcher.h"

#include <atomic>
#include <cstdint>

namespace gw::camera {

bool Initialize();
void Shutdown();

context::Camera* GetCamera();

bool ForwardMovement(float amount, bool true_forward);
bool VerticalMovement(float amount);
bool RotateMovement(float angle);
bool SideMovement(float amount);

bool SetMaxDist(float dist = 900.0f);
bool SetFieldOfView(float fov);
Vec3f ComputeCamPos(float dist = 0.0f);
bool UpdateCameraPos();

float GetFieldOfView();
float GetYaw();

bool UnlockCam(bool flag);
bool GetCameraUnlock();
bool SetFog(bool flag);

extern context::Camera* g_camera;
extern py4gw::MemoryPatcher g_patch_cam_update;
extern py4gw::MemoryPatcher g_patch_fog;
extern std::atomic<bool> g_initialized;

}  // namespace gw::camera
