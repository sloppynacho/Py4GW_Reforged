#pragma once

#include "base/error_handling.h"

#include "GW/common/constants/constants.h"
#include "GW/common/gw_array.h"

#include <cstddef>
#include <cstdint>

namespace gw::context {

struct TextCache {
    uint32_t h0000;
};
static_assert(sizeof(TextCache) == 0x4, "TextCache size mismatch");

struct TextParserSubStruct1 {
    uint32_t h0000;
};
static_assert(sizeof(TextParserSubStruct1) == 0x4, "TextParserSubStruct1 size mismatch");

struct TextParserAsyncBuffer {
    uint32_t async_decode_str_callback;
    uint32_t async_decode_str_param;
    uint32_t buffer_used;
    gw::GwArray<wchar_t> s1;
    gw::GwArray<wchar_t> s2;
    uint32_t h002c;
    uint32_t h0030;
    uint32_t h0034;
    uint8_t h0038[28];
};
static_assert(sizeof(TextParserAsyncBuffer) == 0x54, "TextParserAsyncBuffer size mismatch");

struct TextParser {
    uint32_t h0000[8];
    wchar_t* dec_start;
    wchar_t* dec_end;
    uint32_t substitute_1;
    uint32_t substitute_2;
    TextCache* cache;
    uint32_t h0034[75];
    uint32_t h0160;
    uint32_t h0164;
    uint32_t h0168;
    uint32_t h016c[5];
    TextParserSubStruct1* sub_struct;
    uint32_t h0184[19];
    gw::constants::Language language_id;
};

static_assert(offsetof(TextParser, cache) == 0x30, "TextParser::cache offset mismatch");
static_assert(offsetof(TextParser, sub_struct) == 0x180, "TextParser::sub_struct offset mismatch");
static_assert(offsetof(TextParser, language_id) == 0x1D0, "TextParser::language_id offset mismatch");
static_assert(sizeof(TextParser) == 0x1D4, "TextParser size mismatch");

}  // namespace gw::context
