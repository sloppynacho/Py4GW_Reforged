#include "base/error_handling.h"

#include "GW/context/context.h"
#include "GW/context/game_context.h"
#include "GW/context/world_context.h"

#include "base/CrashHandler.h"
#include "base/logger.h"
#include "base/patterns.h"
#include "base/scanner.h"

namespace gw::context {

uintptr_t g_base_ptr = 0;
uintptr_t g_pregame_context_addr = 0;
uintptr_t g_gameplay_context_addr = 0;

}  // namespace gw::context

namespace {

bool g_initialized = false;

bool ResolveBasePointer() {
    CrashContextScope context("startup", "context", "resolve_base_ptr");
    const auto* pattern = py4gw::Patterns::Get("context.base_ptr_ref");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: context.base_ptr_ref", "context");
        return false;
    }

    const uintptr_t scan = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("base_ptr_ref", scan, "context")) {
        return false;
    }

    const uintptr_t candidate = *reinterpret_cast<const uintptr_t*>(scan);
    if (!Logger::AssertAddress("base_ptr", candidate, "context")) {
        return false;
    }
    if (!py4gw::Scanner::IsValidPtr(candidate, py4gw::ScannerSection::Data)) {
        Logger::Instance().LogError("base_ptr is outside the expected data section.", "context");
        return false;
    }

    gw::context::g_base_ptr = candidate;
    return true;
}

bool ResolveGameplayContextPointer() {
    CrashContextScope context("startup", "context", "resolve_gameplay_context_ptr");
    const auto* pattern = py4gw::Patterns::Get("context.gameplay_context_ref");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: context.gameplay_context_ref", "context");
        return false;
    }

    const uintptr_t scan = py4gw::Scanner::FindAssertion(
        pattern->assertion_file.c_str(),
        pattern->assertion_message.c_str(),
        pattern->line_number,
        pattern->offset);
    if (!Logger::AssertAddress("gameplay_context_ref", scan, "context")) {
        return false;
    }

    const uintptr_t candidate = *reinterpret_cast<const uintptr_t*>(scan);
    if (!Logger::AssertAddress("gameplay_context_addr", candidate, "context")) {
        return false;
    }
    if (!py4gw::Scanner::IsValidPtr(candidate, py4gw::ScannerSection::Data)) {
        Logger::Instance().LogError("gameplay_context_addr is outside the expected data section.", "context");
        return false;
    }

    gw::context::g_gameplay_context_addr = candidate;
    return true;
}

bool ResolvePreGameContextPointer() {
    CrashContextScope context("startup", "context", "resolve_pregame_context_ptr");
    const auto* pattern = py4gw::Patterns::Get("context.pregame_context_ref");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: context.pregame_context_ref", "context");
        return false;
    }

    const uintptr_t scan = py4gw::Scanner::FindAssertion(
        pattern->assertion_file.c_str(),
        pattern->assertion_message.c_str(),
        pattern->line_number,
        pattern->offset);
    if (!Logger::AssertAddress("pregame_context_ref", scan, "context")) {
        return false;
    }

    const uintptr_t candidate = *reinterpret_cast<const uintptr_t*>(scan);
    if (!Logger::AssertAddress("pregame_context_addr", candidate, "context")) {
        return false;
    }
    if (!py4gw::Scanner::IsValidPtr(candidate, py4gw::ScannerSection::Data)) {
        Logger::Instance().LogError("pregame_context_addr is outside the expected data section.", "context");
        return false;
    }

    gw::context::g_pregame_context_addr = candidate;
    return true;
}

}  // namespace

namespace gw::context {

bool Initialize() {
    CrashContextScope context("startup", "context", "initialize");
    if (g_initialized) {
        return true;
    }

    PY4GW_ASSERT(py4gw::Scanner::Initialize());
    PY4GW_ASSERT(py4gw::Patterns::Initialize());

    if (!ResolveBasePointer()) {
        g_base_ptr = 0;
        return false;
    }
    if (!ResolveGameplayContextPointer()) {
        g_base_ptr = 0;
        g_gameplay_context_addr = 0;
        return false;
    }
    if (!ResolvePreGameContextPointer()) {
        g_base_ptr = 0;
        g_gameplay_context_addr = 0;
        g_pregame_context_addr = 0;
        return false;
    }

    g_initialized = true;
    return true;
}

void Shutdown() {
    CrashContextScope context("shutdown", "context", "shutdown");
    g_base_ptr = 0;
    g_pregame_context_addr = 0;
    g_gameplay_context_addr = 0;
    g_initialized = false;
}

}  // namespace gw::context
