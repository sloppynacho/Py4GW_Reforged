#pragma once

#include "base/error_handling.h"

#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

enum class FriendType : uint32_t {
    Unknow = 0,
    Friend = 1,
    Ignore = 2,
    Player = 3,
    Trade = 4,
};

enum class FriendStatus : uint32_t {
    Offline = 0,
    Online = 1,
    DND = 2,
    Away = 3,
    Unknown = 4,
};

struct Friend {
    FriendType type;
    FriendStatus status;
    uint8_t uuid[16];
    wchar_t alias[10];
    wchar_t charname[10];
    uint32_t friend_id;
    uint32_t zone_id;
};

using FriendsListArray = gw::GwArray<Friend*>;

struct FriendList {
    FriendsListArray friends;
    uint8_t h0010[20];
    uint32_t number_of_friend;
    uint32_t number_of_ignore;
    uint32_t number_of_partner;
    uint32_t number_of_trade;
    uint8_t h0034[108];
    FriendStatus player_status;
};

static_assert(offsetof(Friend, type) == 0x0, "Friend::type offset mismatch");
static_assert(offsetof(Friend, status) == 0x4, "Friend::status offset mismatch");
static_assert(offsetof(Friend, uuid) == 0x8, "Friend::uuid offset mismatch");
static_assert(offsetof(Friend, alias) == 0x18, "Friend::alias offset mismatch");
static_assert(offsetof(Friend, charname) == 0x2C, "Friend::charname offset mismatch");
static_assert(offsetof(Friend, friend_id) == 0x40, "Friend::friend_id offset mismatch");
static_assert(offsetof(Friend, zone_id) == 0x44, "Friend::zone_id offset mismatch");
static_assert(sizeof(Friend) == 0x48, "Friend size mismatch");

static_assert(offsetof(FriendList, friends) == 0x0, "FriendList::friends offset mismatch");
static_assert(offsetof(FriendList, number_of_friend) == 0x24, "FriendList::number_of_friend offset mismatch");
static_assert(offsetof(FriendList, number_of_ignore) == 0x28, "FriendList::number_of_ignore offset mismatch");
static_assert(offsetof(FriendList, number_of_partner) == 0x2C, "FriendList::number_of_partner offset mismatch");
static_assert(offsetof(FriendList, number_of_trade) == 0x30, "FriendList::number_of_trade offset mismatch");
static_assert(offsetof(FriendList, player_status) == 0xA0, "FriendList::player_status offset mismatch");
static_assert(sizeof(FriendList) == 0xA4, "FriendList size mismatch");

}  // namespace gw::context
