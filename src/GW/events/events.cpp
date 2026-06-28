#include "base/error_handling.h"

#include "GW/events/events.h"

#include "base/CrashHandler.h"
#include "base/hooker.h"
#include "base/logger.h"
#include "base/patterns.h"
#include "base/scanner.h"

namespace {

uint32_t __cdecl OnSendEventMessage(
    void* event_context,
    uint32_t unk1,
    gw::events::EventID event_id,
    void* data_buffer,
    uint32_t data_length) {
    py4gw::HookBase::EnterHook();
    py4gw::HookStatus status = {};
    uint32_t result = 1;

    auto found = gw::events::g_callbacks.find(event_id);
    if (found == gw::events::g_callbacks.end()) {
        py4gw::HookBase::LeaveHook();
        return gw::events::g_send_event_message_original
            ? gw::events::g_send_event_message_original(event_context, unk1, event_id, data_buffer, data_length)
            : result;
    }

    auto it = found->second.begin();
    const auto end = found->second.end();
    while (it != end) {
        if (it->altitude > 0) {
            break;
        }
        it->callback(&status, event_id, data_buffer, data_length);
        ++status.altitude;
        ++it;
    }

    if (!status.blocked && gw::events::g_send_event_message_original) {
        result = gw::events::g_send_event_message_original(event_context, unk1, event_id, data_buffer, data_length);
    }

    while (it != end) {
        it->callback(&status, event_id, data_buffer, data_length);
        ++status.altitude;
        ++it;
    }

    py4gw::HookBase::LeaveHook();
    return result;
}

bool ResolveSendEventMessageTarget() {
    CrashContextScope context("startup", "events", "resolve_send_event_message_target");
    const auto* pattern = py4gw::Patterns::Get("events.send_event_message_callsite");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: events.send_event_message_callsite", "events");
        return false;
    }

    const uintptr_t callsite = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("SendEventMessage_Callsite", callsite, "events")) {
        return false;
    }

    gw::events::g_send_event_message_func = reinterpret_cast<gw::events::SendEventMessageFn>(
        py4gw::Scanner::FunctionFromNearCall(callsite));
    return Logger::AssertAddress(
        "SendEventMessage_Func",
        reinterpret_cast<uintptr_t>(gw::events::g_send_event_message_func),
        "events");
}

bool Init() {
    CrashContextScope context("startup", "events", "init");
    if (!ResolveSendEventMessageTarget()) {
        return false;
    }

    const int status = py4gw::HookBase::CreateHook(
        reinterpret_cast<void**>(&gw::events::g_send_event_message_func),
        reinterpret_cast<void*>(&OnSendEventMessage),
        reinterpret_cast<void**>(&gw::events::g_send_event_message_original));
    return Logger::AssertHook("SendEventMessage_Func", status, "events");
}

void EnableHooks() {
    CrashContextScope context("runtime", "events", "enable_hooks");
    // Legacy GWCA currently keeps this hook disabled.
    return;
}

void DisableHooks() {
    CrashContextScope context("shutdown", "events", "disable_hooks");
    if (gw::events::g_send_event_message_func) {
        py4gw::HookBase::DisableHooks(reinterpret_cast<void*>(gw::events::g_send_event_message_func));
    }
}

void Exit() {
    CrashContextScope context("shutdown", "events", "exit");
    if (gw::events::g_send_event_message_func) {
        py4gw::HookBase::RemoveHook(reinterpret_cast<void*>(gw::events::g_send_event_message_func));
    }

    gw::events::g_send_event_message_func = nullptr;
    gw::events::g_send_event_message_original = nullptr;
    gw::events::g_callbacks.clear();
}

}  // namespace

namespace gw::events {

bool Initialize() {
    CrashContextScope context("startup", "events", "initialize");
    if (g_initialized) {
        return true;
    }

    PY4GW_ASSERT(py4gw::Scanner::Initialize());
    PY4GW_ASSERT(py4gw::Patterns::Initialize());

    py4gw::HookBase::Initialize();
    if (!Init()) {
        Exit();
        py4gw::HookBase::Deinitialize();
        return false;
    }

    EnableHooks();
    g_initialized = true;
    return true;
}

void Shutdown() {
    CrashContextScope context("shutdown", "events", "shutdown");
    if (!g_initialized) {
        return;
    }

    DisableHooks();
    Exit();
    py4gw::HookBase::Deinitialize();
    g_initialized = false;
}

}  // namespace gw::events
