#include "base/error_handling.h"

#include "GW/guild/guild.h"

#include "GW/context/context.h"
#include "GW/context/guild_context.h"
#include "GW/map/map.h"
#include "GW/ui/ui.h"

namespace gw::guild {

wchar_t* GetPlayerGuildAnnouncer() {
    auto* guild = context::GetGuildContext();
    return guild ? guild->announcement_author : nullptr;
}

wchar_t* GetPlayerGuildAnnouncement() {
    auto* guild = context::GetGuildContext();
    return guild ? guild->announcement : nullptr;
}

uint32_t GetPlayerGuildIndex() {
    auto* guild = context::GetGuildContext();
    return guild ? guild->player_guild_index : 0;
}

context::GuildArray* GetGuildArray() {
    auto* guild = context::GetGuildContext();
    return guild && guild->guilds.valid() ? &guild->guilds : nullptr;
}

context::Guild* GetPlayerGuild() {
    return GetGuildInfo(GetPlayerGuildIndex());
}

context::Guild* GetCurrentGH() {
    auto* map_info = map::GetCurrentMapInfo();
    if (!map_info || map_info->type != context::RegionType::GuildHall) {
        return nullptr;
    }

    auto* guilds = GetGuildArray();
    if (!guilds) {
        return nullptr;
    }

    for (auto* guild : *guilds) {
        if (guild) {
            return guild;
        }
    }
    return nullptr;
}

context::Guild* GetGuildInfo(uint32_t guild_id) {
    auto* guilds = GetGuildArray();
    return guilds && guild_id < guilds->size() ? guilds->at(guild_id) : nullptr;
}

bool TravelGH() {
    auto* guild = context::GetGuildContext();
    return guild ? TravelGH(guild->player_gh_key) : false;
}

bool TravelGH(context::GHKey key) {
    return ui::SendUIMessage(ui::UIMessage::kGuildHall, &key);
}

bool LeaveGH() {
    return ui::SendUIMessage(ui::UIMessage::kLeaveGuildHall);
}

}  // namespace gw::guild
