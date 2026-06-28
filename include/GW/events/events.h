#pragma once

#include "base/error_handling.h"

#include "base/hook_types.h"

#include <atomic>
#include <cstdint>
#include <unordered_map>
#include <vector>

namespace gw::events {

bool Initialize();
void Shutdown();

enum class EventID {
    kRecvPing = 0x8,
    kSendFriendState = 0x26,
    kRecvFriendState = 0x2c
};

using EventCallback = py4gw::HookCallback<EventID, void*, uint32_t>;
using SendEventMessageFn = uint32_t(__cdecl*)(void* event_context, uint32_t unk1, EventID event_id, void* data_buffer, uint32_t data_length);

struct CallbackEntry {
    int altitude = 0;
    py4gw::HookEntry* entry = nullptr;
    EventCallback callback;
};

void RegisterEventCallback(
    py4gw::HookEntry* entry,
    EventID event_id,
    const EventCallback& callback,
    int altitude = -0x8000);
void RemoveEventCallback(py4gw::HookEntry* entry);

extern SendEventMessageFn g_send_event_message_func;
extern SendEventMessageFn g_send_event_message_original;
extern std::unordered_map<EventID, std::vector<CallbackEntry>> g_callbacks;
extern std::atomic<bool> g_initialized;

}  // namespace gw::events
