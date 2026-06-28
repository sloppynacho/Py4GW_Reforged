#pragma once

#include "base/error_handling.h"

#include "GW/common/game_pos.h"
#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct PathingTrapezoid {
    uint32_t id;
    PathingTrapezoid* adjacent[4];
    uint16_t portal_left;
    uint16_t portal_right;
    float XTL;
    float XTR;
    float YT;
    float XBL;
    float XBR;
    float YB;
};
static_assert(sizeof(PathingTrapezoid) == 0x30, "PathingTrapezoid size mismatch");

struct Node {
    uint32_t type;
    uint32_t id;
};

struct XNode : Node {
    Vec2f pos;
    Vec2f dir;
    Node* left;
    Node* right;
};
static_assert(sizeof(XNode) == 0x20, "XNode size mismatch");

struct YNode : Node {
    Vec2f pos;
    Node* left;
    Node* right;
};
static_assert(sizeof(YNode) == 0x18, "YNode size mismatch");

struct SinkNode : Node {
    PathingTrapezoid** trapezoid;
};
static_assert(sizeof(SinkNode) == 0xC, "SinkNode size mismatch");

struct Portal {
    uint16_t left_layer_id;
    uint16_t right_layer_id;
    uint32_t h0004;
    Portal* pair;
    uint32_t count;
    PathingTrapezoid** trapezoids;
};
static_assert(sizeof(Portal) == 0x14, "Portal size mismatch");

struct PathingMap {
    uint32_t zplane;
    uint32_t h0004;
    uint32_t h0008;
    uint32_t h000C;
    uint32_t h0010;
    uint32_t trapezoid_count;
    PathingTrapezoid* trapezoids;
    uint32_t sink_node_count;
    SinkNode* sink_nodes;
    uint32_t x_node_count;
    XNode* x_nodes;
    uint32_t y_node_count;
    YNode* y_nodes;
    uint32_t h0034;
    uint32_t h0038;
    uint32_t portal_count;
    Portal* portals;
    Node* root_node;
    uint32_t* h0048;
    uint32_t* h004C;
    uint32_t* h0050;
};
static_assert(sizeof(PathingMap) == 0x54, "PathingMap size mismatch");

struct PropByType {
    uint32_t object_id;
    uint32_t prop_index;
};

struct PropModelInfo {
    uint32_t h0000;
    uint32_t h0004;
    uint32_t h0008;
    uint32_t h000C;
    uint32_t h0010;
    uint32_t h0014;
};
static_assert(sizeof(PropModelInfo) == 0x18, "PropModelInfo size mismatch");

struct RecObject {
    uint32_t h0000;
    uint32_t h0004;
    uint32_t accessKey;
};
static_assert(sizeof(RecObject) == 0xC, "RecObject size mismatch");

struct MapProp {
    uint32_t h0000[5];
    uint32_t uptime_seconds;
    uint32_t h0018;
    uint32_t prop_index;
    Vec3f position;
    uint32_t model_file_id;
    uint32_t h0030[2];
    float rotation_angle;
    float rotation_cos;
    float rotation_sin;
    uint32_t h0040[5];
    RecObject* interactive_model;
    uint32_t h005C[4];
    uint32_t appearance_bitmap;
    uint32_t animation_bits;
    uint32_t h0074[5];
    PropByType* prop_object_info;
    uint32_t h008C;
};
static_assert(sizeof(MapProp) == 0x90, "MapProp size mismatch");

using PathingMapArray = gw::GwArray<PathingMap>;

}  // namespace gw::context
