#include "base/error_handling.h"

#include "GW/events/events.h"

namespace gw::events {

SendEventMessageFn g_send_event_message_func = nullptr;
SendEventMessageFn g_send_event_message_original = nullptr;
std::unordered_map<EventID, std::vector<CallbackEntry>> g_callbacks;
std::atomic<bool> g_initialized = false;

void RegisterEventCallback(
    py4gw::HookEntry* entry,
    EventID event_id,
    const EventCallback& callback,
    int altitude) {
    auto found = g_callbacks.find(event_id);
    if (found == g_callbacks.end()) {
        g_callbacks[event_id] = std::vector<CallbackEntry>();
    }

    auto it = g_callbacks[event_id].begin();
    while (it != g_callbacks[event_id].end()) {
        if (it->altitude > altitude) {
            break;
        }
        ++it;
    }

    g_callbacks[event_id].insert(it, CallbackEntry{altitude, entry, callback});
}

void RemoveEventCallback(py4gw::HookEntry* entry) {
    for (auto& callbacks : g_callbacks) {
        auto it = callbacks.second.begin();
        while (it != callbacks.second.end()) {
            if (it->entry == entry) {
                callbacks.second.erase(it);
                return;
            }
            ++it;
        }
    }
}

}  // namespace gw::events
