# GWCA GameEntities Dependency Map

## Rule

- legacy `GWCA/GameEntities/...` headers migrate into `GW/context`
- legacy shared container headers such as `Array.h` and `List.h` migrate into `GW/common`
- legacy shared protocol headers do not belong here; they go into `GW/common`

## Done

- `Friendslist.h` -> `GW/context/friend_list.h`
- `Camera.h` -> `GW/context/camera.h`
- `Pathing.h` -> `GW/context/pathing.h`
- `Title.h` -> `GW/context/title.h`
- `Match.h` -> `GW/context/match.h`
- `Player.h` -> `GW/context/player.h`
- `NPC.h` -> `GW/context/npc.h`
- `Attribute.h` -> `GW/context/attribute.h`
- `Guild.h` -> `GW/context/guild.h`
- `Map.h` -> `GW/context/map.h`
- `Hero.h` -> `GW/context/hero.h`
- `Quest.h` -> `GW/context/quest.h`
- `Party.h` -> `GW/context/party.h`
- `Item.h` -> `GW/context/item.h`
- `Agent.h` -> `GW/context/agent.h`
- `Skill.h` reconciled into `GW/context/skill.h`
- `Constants/AgentIDs.h` -> `GW/common/constants/agent_ids.h`
- `Constants/Constants.h` -> `GW/common/constants/constants.h`
- `Constants/ItemIDs.h` -> `GW/common/constants/item_ids.h`
- `Constants/Maps.h` -> `GW/common/constants/maps.h`
- `Constants/QuestIDs.h` -> `GW/common/constants/quest_ids.h`
- `Constants/Skills.h` -> `GW/common/constants/skills.h`

Shared prerequisites already migrated:

- `GameContainers/Array.h` -> `GW/common/gw_array.h`
- `GameContainers/List.h` -> `GW/common/gw_list.h`
- `GameContainers/GamePos.h` -> `GW/common/game_pos.h`

## Ready Now

- none

## Missing

- none

## Blocking

- none

## Order

1. game entity header migration is complete for the currently mapped GWCA set
