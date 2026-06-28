#pragma once

#include "base/error_handling.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct Cinematic;
struct MapContext;
struct TextParser;
struct CharContext;
struct ItemContext;
struct AgentContext;
struct GuildContext;
struct PartyContext;
struct TradeContext;
struct WorldContext;
struct GadgetContext;
struct AccountContext;

struct GameContext {
    void* h0000;
    void* h0004;
    AgentContext* agent;
    void* h000c;
    void* h0010;
    MapContext* map;
    TextParser* text_parser;
    void* h001c;
    uint32_t some_number;
    void* h0024;
    AccountContext* account;
    WorldContext* world;
    Cinematic* cinematic;
    void* h0034;
    GadgetContext* gadget;
    GuildContext* guild;
    ItemContext* items;
    CharContext* character;
    void* h0048;
    PartyContext* party;
    void* h0050;
    void* h0054;
    TradeContext* trade;
};

static_assert(offsetof(GameContext, agent) == 0x8, "GameContext::agent offset mismatch");
static_assert(offsetof(GameContext, map) == 0x14, "GameContext::map offset mismatch");
static_assert(offsetof(GameContext, character) == 0x44, "GameContext::character offset mismatch");
static_assert(offsetof(GameContext, party) == 0x4C, "GameContext::party offset mismatch");
static_assert(offsetof(GameContext, world) == 0x2C, "GameContext::world offset mismatch");
static_assert(sizeof(GameContext) == 0x5C, "GameContext size mismatch");

}  // namespace gw::context
