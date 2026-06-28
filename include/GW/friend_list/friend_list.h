#pragma once

#include "base/error_handling.h"

#include "base/hook_types.h"
#include "GW/context/friend_list.h"

#include <atomic>
#include <cstdint>
#include <unordered_map>

namespace gw::friend_list {

bool Initialize();
void Shutdown();

using FriendStatusCallback = py4gw::HookCallback<const context::Friend*, const context::Friend*>;
using FriendEventHandlerFn = void(__cdecl*)(void*, void*);
using SetOnlineStatusFn = void(__cdecl*)(context::FriendStatus status);
using AddFriendFn = void(__cdecl*)(const wchar_t* name, const wchar_t* alias, context::FriendType type);
using RemoveFriendFn = void(__cdecl*)(const uint8_t* uuid, const wchar_t* name, uint32_t arg8);

context::FriendList* GetFriendList();

context::Friend* GetFriend(const wchar_t* alias, const wchar_t* charname, context::FriendType type = context::FriendType::Friend);
context::Friend* GetFriend(uint32_t index);
context::Friend* GetFriend(const uint8_t* uuid);

uint32_t GetNumberOfFriends(context::FriendType type = context::FriendType::Friend);
uint32_t GetNumberOfIgnores();
uint32_t GetNumberOfPartners();
uint32_t GetNumberOfTraders();

context::FriendStatus GetMyStatus();
bool SetFriendListStatus(context::FriendStatus status);

void RegisterFriendStatusCallback(
    py4gw::HookEntry* entry,
    const FriendStatusCallback& callback);
void RemoveFriendStatusCallback(py4gw::HookEntry* entry);

bool AddFriend(const wchar_t* name, const wchar_t* alias = nullptr);
bool AddIgnore(const wchar_t* name, const wchar_t* alias = nullptr);
bool RemoveFriend(context::Friend* friend_entry);

extern FriendEventHandlerFn g_friend_event_handler_func;
extern FriendEventHandlerFn g_friend_event_handler_original;
extern SetOnlineStatusFn g_set_online_status_func;
extern AddFriendFn g_add_friend_func;
extern RemoveFriendFn g_remove_friend_func;
extern uintptr_t g_friend_list_addr;
extern std::unordered_map<py4gw::HookEntry*, FriendStatusCallback> g_friend_status_callbacks;
extern std::atomic<bool> g_initialized;

}  // namespace gw::friend_list
