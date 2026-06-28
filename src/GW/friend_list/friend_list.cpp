#include "base/error_handling.h"

#include "GW/friend_list/friend_list.h"

#include "base/CrashHandler.h"
#include "base/hooker.h"
#include "base/logger.h"
#include "base/patterns.h"
#include "base/scanner.h"

#include <cstdlib>
#include <cstring>

namespace {

#pragma warning(push)
#pragma warning(disable : 4200)
struct EventData {
    uint32_t event_id;
    uint32_t unk;
    uint32_t data_size;
    uint32_t data[];
};
#pragma warning(pop)

void __cdecl OnFriendEventHandler(void* unk, EventData* event_info) {
    py4gw::HookBase::EnterHook();
    uint8_t* uuid = nullptr;
    const wchar_t* alias = nullptr;
    switch (event_info->event_id) {
    case 0x26:
        uuid = reinterpret_cast<uint8_t*>(&event_info->data[2]);
        alias = reinterpret_cast<wchar_t*>(&event_info->data[6]);
        break;
    case 0x28:
        alias = reinterpret_cast<wchar_t*>(&event_info->data[0]);
        break;
    case 0x2C:
        alias = reinterpret_cast<wchar_t*>(&event_info->data[4]);
        break;
    default:
        break;
    }

    if (!uuid && !alias) {
        if (gw::friend_list::g_friend_event_handler_original) {
            gw::friend_list::g_friend_event_handler_original(unk, event_info);
        }
        py4gw::HookBase::LeaveHook();
        return;
    }

    bool uuid_valid = false;
    for (size_t i = 0; uuid && i < 16 && !uuid_valid; ++i) {
        uuid_valid = uuid[i] != 0;
    }
    if (!uuid_valid) {
        uuid = nullptr;
    }
    if (!(alias && alias[0])) {
        alias = nullptr;
    }

    gw::context::Friend* current_state = nullptr;
    if (uuid) {
        current_state = gw::friend_list::GetFriend(uuid);
    } else if (alias) {
        current_state = gw::friend_list::GetFriend(alias, nullptr, gw::context::FriendType::Unknow);
    }

    py4gw::HookStatus hook_status = {};
    gw::context::Friend* old_state = nullptr;
    if (current_state) {
        old_state = static_cast<gw::context::Friend*>(std::malloc(sizeof(*old_state)));
        PY4GW_ASSERT(old_state && std::memcpy(old_state, current_state, sizeof(*old_state)));
    }

    if (gw::friend_list::g_friend_event_handler_original) {
        gw::friend_list::g_friend_event_handler_original(unk, event_info);
    }

    if (uuid) {
        current_state = gw::friend_list::GetFriend(uuid);
    } else if (alias) {
        current_state = gw::friend_list::GetFriend(alias, nullptr, gw::context::FriendType::Unknow);
    }

    for (auto& [entry, callback] : gw::friend_list::g_friend_status_callbacks) {
        static_cast<void>(entry);
        callback(&hook_status, old_state, current_state);
        ++hook_status.altitude;
    }

    if (old_state) {
        std::free(old_state);
    }

    py4gw::HookBase::LeaveHook();
}

bool ResolveFriendListPointer() {
    CrashContextScope context("startup", "friend_list", "resolve_friend_list_pointer");
    const auto* anchor_pattern = py4gw::Patterns::Get("friend_list.friend_list_anchor");
    const auto* list_pattern = py4gw::Patterns::Get("friend_list.friend_list_scan");
    if (!anchor_pattern || !list_pattern) {
        Logger::Instance().LogError("Missing or invalid friend list pointer pattern.", "friend_list");
        return false;
    }

    uintptr_t address = py4gw::Scanner::FindAssertion(
        anchor_pattern->assertion_file.c_str(),
        anchor_pattern->assertion_message.c_str(),
        static_cast<uint32_t>(anchor_pattern->line_number),
        anchor_pattern->offset);
    if (!Logger::AssertAddress("FriendList_Anchor", address, "friend_list")) {
        return false;
    }

    address = py4gw::Scanner::FindInRange(
        list_pattern->pattern.c_str(),
        list_pattern->mask.c_str(),
        list_pattern->offset,
        address,
        address + anchor_pattern->range);
    if (!Logger::AssertAddress("FriendList_PointerAddress", address, "friend_list")) {
        return false;
    }
    if (!py4gw::Scanner::IsValidPtr(*reinterpret_cast<uintptr_t*>(address))) {
        Logger::Instance().LogError("Friend list pointer is outside the expected data section.", "friend_list");
        return false;
    }

    gw::friend_list::g_friend_list_addr = *reinterpret_cast<uintptr_t*>(address);
    return Logger::AssertAddress("FriendList_Addr", gw::friend_list::g_friend_list_addr, "friend_list");
}

bool ResolveFriendEventHandler() {
    CrashContextScope context("startup", "friend_list", "resolve_friend_event_handler");
    const auto* pattern = py4gw::Patterns::Get("friend_list.friend_event_handler");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: friend_list.friend_event_handler", "friend_list");
        return false;
    }

    const uintptr_t scan = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("FriendEventHandler_Scan", scan, "friend_list")) {
        return false;
    }

    gw::friend_list::g_friend_event_handler_func = reinterpret_cast<gw::friend_list::FriendEventHandlerFn>(
        py4gw::Scanner::ToFunctionStart(scan));
    return Logger::AssertAddress(
        "FriendEventHandler_Func",
        reinterpret_cast<uintptr_t>(gw::friend_list::g_friend_event_handler_func),
        "friend_list");
}

bool ResolveSetOnlineStatus() {
    CrashContextScope context("startup", "friend_list", "resolve_set_online_status");
    const auto* pattern = py4gw::Patterns::Get("friend_list.set_online_status");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: friend_list.set_online_status", "friend_list");
        return false;
    }

    const uintptr_t scan = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("SetOnlineStatus_Scan", scan, "friend_list")) {
        return false;
    }

    gw::friend_list::g_set_online_status_func = reinterpret_cast<gw::friend_list::SetOnlineStatusFn>(
        py4gw::Scanner::ToFunctionStart(scan));
    return Logger::AssertAddress(
        "SetOnlineStatus_Func",
        reinterpret_cast<uintptr_t>(gw::friend_list::g_set_online_status_func),
        "friend_list");
}

bool ResolveAddFriend() {
    CrashContextScope context("startup", "friend_list", "resolve_add_friend");
    const auto* pattern = py4gw::Patterns::Get("friend_list.add_friend");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: friend_list.add_friend", "friend_list");
        return false;
    }

    const uintptr_t scan = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("AddFriend_Scan", scan, "friend_list")) {
        return false;
    }

    gw::friend_list::g_add_friend_func = reinterpret_cast<gw::friend_list::AddFriendFn>(
        py4gw::Scanner::ToFunctionStart(scan));
    return Logger::AssertAddress(
        "AddFriend_Func",
        reinterpret_cast<uintptr_t>(gw::friend_list::g_add_friend_func),
        "friend_list");
}

bool ResolveRemoveFriend() {
    CrashContextScope context("startup", "friend_list", "resolve_remove_friend");
    const auto* anchor_pattern = py4gw::Patterns::Get("friend_list.remove_friend_anchor");
    const auto* call_pattern = py4gw::Patterns::Get("friend_list.remove_friend_call");
    if (!anchor_pattern || !call_pattern) {
        Logger::Instance().LogError("Missing or invalid remove friend pattern.", "friend_list");
        return false;
    }

    uintptr_t address = py4gw::Scanner::Find(
        anchor_pattern->pattern.c_str(),
        anchor_pattern->mask.c_str(),
        anchor_pattern->offset,
        anchor_pattern->section);
    if (!Logger::AssertAddress("RemoveFriend_Anchor", address, "friend_list")) {
        return false;
    }

    address = py4gw::Scanner::FindInRange(
        call_pattern->pattern.c_str(),
        call_pattern->mask.c_str(),
        call_pattern->offset,
        address,
        address + anchor_pattern->range);
    if (!Logger::AssertAddress("RemoveFriend_Callsite", address, "friend_list")) {
        return false;
    }

    gw::friend_list::g_remove_friend_func = reinterpret_cast<gw::friend_list::RemoveFriendFn>(
        py4gw::Scanner::FunctionFromNearCall(address));
    return Logger::AssertAddress(
        "RemoveFriend_Func",
        reinterpret_cast<uintptr_t>(gw::friend_list::g_remove_friend_func),
        "friend_list");
}

bool Init() {
    CrashContextScope context("startup", "friend_list", "init");
    if (!ResolveFriendListPointer() ||
        !ResolveFriendEventHandler() ||
        !ResolveSetOnlineStatus() ||
        !ResolveAddFriend() ||
        !ResolveRemoveFriend()) {
        return false;
    }

    const int status = py4gw::HookBase::CreateHook(
        reinterpret_cast<void**>(&gw::friend_list::g_friend_event_handler_func),
        reinterpret_cast<void*>(&OnFriendEventHandler),
        reinterpret_cast<void**>(&gw::friend_list::g_friend_event_handler_original));
    return Logger::AssertHook("FriendEventHandler_Func", status, "friend_list");
}

void EnableHooks() {
    CrashContextScope context("runtime", "friend_list", "enable_hooks");
    if (gw::friend_list::g_friend_event_handler_func) {
        py4gw::HookBase::EnableHooks(reinterpret_cast<void*>(gw::friend_list::g_friend_event_handler_func));
    }
}

void DisableHooks() {
    CrashContextScope context("shutdown", "friend_list", "disable_hooks");
    if (gw::friend_list::g_friend_event_handler_func) {
        py4gw::HookBase::DisableHooks(reinterpret_cast<void*>(gw::friend_list::g_friend_event_handler_func));
    }
}

void Exit() {
    CrashContextScope context("shutdown", "friend_list", "exit");
    if (gw::friend_list::g_friend_event_handler_func) {
        py4gw::HookBase::RemoveHook(reinterpret_cast<void*>(gw::friend_list::g_friend_event_handler_func));
    }

    gw::friend_list::g_friend_event_handler_func = nullptr;
    gw::friend_list::g_friend_event_handler_original = nullptr;
    gw::friend_list::g_set_online_status_func = nullptr;
    gw::friend_list::g_add_friend_func = nullptr;
    gw::friend_list::g_remove_friend_func = nullptr;
    gw::friend_list::g_friend_list_addr = 0;
    gw::friend_list::g_friend_status_callbacks.clear();
}

}  // namespace

namespace gw::friend_list {

bool Initialize() {
    CrashContextScope context("startup", "friend_list", "initialize");
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
    CrashContextScope context("shutdown", "friend_list", "shutdown");
    if (!g_initialized) {
        return;
    }

    DisableHooks();
    Exit();
    py4gw::HookBase::Deinitialize();
    g_initialized = false;
}

}  // namespace gw::friend_list
