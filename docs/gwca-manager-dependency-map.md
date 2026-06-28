# GWCA Manager Dependency Map

## Rule Used Here

This file treats dependencies the same way `RenderMgr -> UIMgr` had to be treated:

- `hard`: missing it blocks the manager's core functionality
- `situational`: only needed for a feature path, action path, or helper path

So a manager is still a valid migration start if its missing dependencies are only situational.

Important interpretation:

- a "reduced" or "first slice" start here applies only to the manager being chosen
- it does not authorize silently migrating a reduced version of some other missing shared prerequisite
- if a chosen manager turns out to need unmigrated shared code for parity, stop and call out that prerequisite explicitly before porting further

## Current Base In Reforged

Already migrated:

- `GameThreadMgr`
- `StoCMgr`
- `RenderMgr`
- `UIMgr` except the remaining unrecovered utility declarations `Default_UICallback`, `GetCommandLinePref`, `SetCommandLinePref`, `IsInControllerMode`, and `IsInControllerCursorMode`
- `CameraMgr`
- `EffectMgr`
- `EventMgr`
- `FriendListMgr`
- `PlayerMgr` except the commented `SkillbarMgr`-dependent `ChangeSecondProfession(...)` path
- `QuestMgr` except the commented active/abandon action-hook paths
- `GuildMgr`
- `MapMgr` except the commented `UIMgr`-dependent travel, map test, map UI context, and enter-challenge paths

Shared prerequisites already migrated for current manager work:

- `GW` context access
- shared skill/effect helper code used by `EffectMgr`

Not migrated:

- `ChatMgr`
- `AgentMgr`
- `ItemMgr`
- `PartyMgr`
- `TradeMgr`
- `MerchantMgr`
- `SkillbarMgr`

## What You Can Start

Ordered from easiest to start to hardest to start.

### Easiest full/core starts

- none remaining in the previous top-priority bucket

### Easiest reduced-slice starts

- `MerchantMgr`
- `TradeMgr`
- `ChatMgr`
- `AgentMgr`
- `ItemMgr`
- `SkillbarMgr`
- `PartyMgr`

### Hardest if you want full parity immediately

- `MapMgr`
- `ChatMgr`
- `MerchantMgr`
- `TradeMgr`
- `AgentMgr`
- `ItemMgr`
- `SkillbarMgr`
- `PartyMgr`

## Migration Path Summary

| Manager | Start now? | First useful slice | Full parity blocker |
|---|---|---|---|
| `FriendListMgr` | migrated | friend list reads and status hooks | none |
| `PlayerMgr` | migrated except deferred profession-change path | player/title/account reads | `SkillbarMgr` only for profession-change path |
| `QuestMgr` | migrated except deferred action-hook paths | quest log, active quest, quest lookup, quest info requests, async decode helpers | remaining quest action hooks for set/abandon |
| `GuildMgr` | migrated | guild/faction data reads and guild-hall travel helpers | none |
| `MapMgr` | migrated except deferred UI paths | map/context/state reads, query altitude, pathing, cinematics, instance metadata | `UIMgr` for travel, map test, map UI contexts, and enter-challenge paths |
| `UIMgr` | migrated except deferred internal parity slices | UI frame/message foundation, callback registration, frame lookup, frame-state helpers, window helpers, async decode, title decode, generic create/destroy component path, typed control builders, button or mouse utility helpers, `ButtonFrame`, `CheckboxFrame`, `DropdownFrame`, `SliderFrame`, `EditableTextFrame`, `ProgressBar`, `TextLabelFrame`, `MultiLineTextLabelFrame`, `TabsFrame`, `ScrollableFrame`, dropdown selection helper, preference read and write side | remaining unrecovered utility declarations `Default_UICallback`, `GetCommandLinePref`, `SetCommandLinePref`, `IsInControllerMode`, and `IsInControllerCursorMode` |
| `MerchantMgr` | reduced only | merchant item reads | `UIMgr` for quote and transact actions |
| `TradeMgr` | reduced only | trade state and offered-item reads | `UIMgr` for trade actions |
| `ChatMgr` | reduced only | chat log, typing state, channel state | `UIMgr` for the real chat behavior surface |
| `AgentMgr` | reduced only | agent arrays, ids, basic lookups | `UIMgr` for actions; full parity reconnects into gameplay cluster |
| `ItemMgr` | reduced only | item arrays, bags, storage, gold, lookups | full parity reconnects into `UIMgr` + gameplay cluster |
| `SkillbarMgr` | reduced only | skill data, attributes, skillbar arrays, templates | full parity reconnects into `UIMgr` + gameplay cluster |
| `PartyMgr` | reduced only | party info, counts, loaded/leader state | full parity reconnects into `UIMgr` + gameplay cluster |

## Manager Read

| Manager | Hard deps for core migration | Situational deps you can activate later | Viable first slice | Read |
|---|---|---|---|---|
| `UIMgr` | migrated except deferred internal parity slices | unrecovered utility declarations `Default_UICallback`, `GetCommandLinePref`, `SetCommandLinePref`, `IsInControllerMode`, and `IsInControllerCursorMode` | frame lookup, related-frame traversal, frame-array introspection, message dispatch, callback registration, frame-state helpers, window helpers, async decode, title decode, generic create/destroy component path, typed control builders, button or mouse utility helpers, `ButtonFrame`, `CheckboxFrame`, `DropdownFrame`, `SliderFrame`, `EditableTextFrame`, `ProgressBar`, `TextLabelFrame`, `MultiLineTextLabelFrame`, `TabsFrame`, `ScrollableFrame`, dropdown selection helper, preference read and write side | migrated; no external blocker remained after the shared text-parser access path was restored, and the remaining work is limited to legacy declarations with no recovered GWCA body rather than another manager dependency |
| `FriendListMgr` | migrated | migrated | migrated | already migrated |
| `PlayerMgr` | migrated except deferred profession-change path | `SkillbarMgr`, weak `UIMgr` | player array, active player, titles, account/player reads | migrated, with `ChangeSecondProfession(...)` intentionally left commented until `SkillbarMgr` exists |
| `QuestMgr` | migrated except deferred action-hook paths | none external now | quest log, active quest, quest lookup, quest info requests, async decode helpers | migrated; async decode helpers are active now, while set/abandon action hooks remain a quest-local follow-up |
| `GuildMgr` | migrated | none | guild array, current guild, faction/guild data reads, guild-hall travel helpers | fully migrated for the current legacy surface |
| `MapMgr` | migrated except deferred UI paths | `UIMgr` | instance/map context, region, outpost/explorable state, map identity, pathing, altitude, cinematic state | migrated; travel, map test, map UI context, and enter-challenge helpers remain as the next `MapMgr` follow-up slice on top of the now-migrated UI foundation |
| `ChatMgr` | `GameThreadMgr` for init timing; `UIMgr` for most active behavior | none | chat log read, channel mapping, typing state, channel color state | small read/config slice exists now, but most of legacy chat is UI-shaped |
| `MerchantMgr` | none for merchant item reads; `UIMgr` for quote/transact actions | none | merchant item array reads | core reads can start now; transactions still want `UIMgr` |
| `TradeMgr` | none for trade-context reads; `UIMgr` for actions | `ItemMgr` | trade state, offered-item inspection | core reads can start now; active trade actions still want `UIMgr` |
| `AgentMgr` | none for agent/map data reads; `UIMgr` for actions | `MapMgr`, `PartyMgr`, `PlayerMgr`, `ItemMgr` | agent arrays, lookups, target/observing ids, player/npc resolution | core reads can start; action surface belongs to gameplay cluster |
| `ItemMgr` | none for inventory/query core | `UIMgr`, `MapMgr`, `StoCMgr`, `AgentMgr` | item arrays, bags, storage state, item lookup/counting, gold reads | core reads can start, but full manager is cluster work |
| `SkillbarMgr` | `GameThreadMgr` for hook/init timing | `UIMgr`, `MapMgr`, `PartyMgr`, `AgentMgr` | skill data, attribute data, skillbar arrays, template encode/decode | core data can start, but full manager is cluster work |
| `PartyMgr` | `GameThreadMgr` | `UIMgr`, `MapMgr`, `ChatMgr`, `PlayerMgr`, `SkillbarMgr`, `AgentMgr` | party info, counts, loaded/leader/hardmode state, hero metadata | read/state slice is possible; full manager is the most entangled |

## Main Hubs

- `UIMgr` is the biggest shared hub
- `MapMgr` is the next shared world-state hub
- `AgentMgr` is the gameplay hub
- `PartyMgr` is the most entangled follow-on manager

## Migrated Since Baseline

- `EffectMgr` is no longer a candidate manager; it is already migrated
- `EventMgr` is no longer a candidate manager; it is already migrated
- `StoCMgr` is no longer a candidate manager; it is already migrated
- `FriendListMgr` is no longer a candidate manager; it is already migrated
- `PlayerMgr` is no longer a candidate manager for core reads; it is migrated except for the intentionally deferred `SkillbarMgr`-dependent `ChangeSecondProfession(...)` path
- `UIMgr` is no longer a candidate manager; the message, callback, frame, related-frame, frame-array, frame-state, window, async decode, title decode, generic create/destroy component, typed control builder, button or mouse utility, `ButtonFrame`, `CheckboxFrame`, `DropdownFrame`, `SliderFrame`, `EditableTextFrame`, `ProgressBar`, `TextLabelFrame`, `MultiLineTextLabelFrame`, `TabsFrame`, `ScrollableFrame`, dropdown selection helper, and preference read and write side foundation is now migrated, with only the unrecovered utility declarations `Default_UICallback`, `GetCommandLinePref`, `SetCommandLinePref`, `IsInControllerMode`, and `IsInControllerCursorMode` still deferred
- the remaining `UIMgr` slices are not blocked by another manager anymore; they are local UI-specific scan and hook work inside legacy `UIMgr.cpp`
- `QuestMgr` is no longer a candidate manager for core reads; it is migrated except for the intentionally deferred quest action-hook paths
- `GuildMgr` is no longer a candidate manager for core reads; it is fully migrated
- `MapMgr` is no longer a candidate manager for core reads; it is migrated except for the intentionally deferred `UIMgr`-dependent travel, map test, map UI context, and enter-challenge paths
- `EffectMgr` was unblocked by migrating its shared prerequisites first instead of adding local compatibility code
- current baseline now includes manager migrations plus the shared `GW/context` support needed by those migrations

## Why Some Dependencies Are Not Blockers

- `RenderMgr -> UIMgr` was only screenshot gating, so render was still migratable.
- `MapMgr -> UIMgr` is mostly travel, enter-mission, and world-map style actions; map context reads do not need that path first.
- `ChatMgr -> UIMgr` is mostly send/print/whisper/log routing; chat log and typing-state reads do not need that path first.
- `MerchantMgr -> UIMgr` is mostly request-quote and transact-item routing; merchant item reads do not need that path first.
- `TradeMgr -> UIMgr` is mostly open/accept/cancel/offer/remove actions; trade context inspection does not need that path first.
- `AgentMgr -> UIMgr` is mostly dialog, target, call-target, and movement/action routing; agent arrays and lookups are not blocked by that.
- `ItemMgr -> UIMgr` is mostly use/move/salvage/interact actions; inventory and bag reads are not blocked by that.
- `QuestMgr -> UIMgr` used to block async decode and action plumbing; async decode is now unblocked, and the remaining set/abandon work is quest-local.
- `GuildMgr -> UIMgr` and `GuildMgr -> MapMgr` used to block guild-hall travel helpers; that path is now unblocked.
- `MapMgr -> UIMgr` also covers travel and world/mission map UI ownership; core map state reads do not need that path first.
- `FriendListMgr -> UIMgr` is a weak/commented edge in legacy code, not a core blocker.

## Short Answer

If you want the easiest path first, start with:

1. `ChatMgr`

If you are willing to start with reduced read/core slices instead of full parity, these are also valid starts:

1. `MerchantMgr`
2. `TradeMgr`
3. `ChatMgr`
4. `AgentMgr`
5. `ItemMgr`
6. `SkillbarMgr`
7. `PartyMgr`

Leave these for later only if you want their full action-heavy behavior immediately:

1. `PartyMgr`
2. `SkillbarMgr`
3. `ItemMgr`
4. `AgentMgr`
5. `TradeMgr`
6. `MerchantMgr`
7. `ChatMgr`
8. `MapMgr`
