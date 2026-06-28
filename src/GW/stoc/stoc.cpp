#include "base/error_handling.h"

#include "GW/stoc/stoc.h"

#include "base/CrashHandler.h"
#include "base/hooker.h"
#include "base/logger.h"
#include "base/patterns.h"
#include "base/scanner.h"
#include "GW/game_thread/game_thread.h"

#include <string>

void SafeInitializeCriticalSection(CRITICAL_SECTION* mtx) {
    if (!mtx || mtx->DebugInfo) {
        return;
    }
    ::InitializeCriticalSection(&gw::stoc::g_mutex);
    gw::stoc::g_mutex_initialized = true;
}

bool ResolveGameServerHandlers() {
    CrashContextScope context("startup", "stoc", "resolve_game_server_handlers");
    const auto* pattern = py4gw::Patterns::Get("stoc.handler_table_pointer");
    if (!pattern) {
        Logger::Instance().LogError("Missing or invalid pattern: stoc.handler_table_pointer", "stoc");
        return false;
    }

    const uintptr_t pointer_location = py4gw::Scanner::Find(
        pattern->pattern.c_str(),
        pattern->mask.c_str(),
        pattern->offset,
        pattern->section);
    if (!Logger::AssertAddress("StoCHandler_PointerLocation", pointer_location, "stoc")) {
        return false;
    }

    const uintptr_t handlers_addr = *reinterpret_cast<uintptr_t*>(pointer_location);
    if (!Logger::AssertAddress("StoCHandler_Addr", handlers_addr, "stoc")) {
        return false;
    }

    auto** game_server = reinterpret_cast<gw::stoc::GameServer**>(handlers_addr);
    if (!(game_server && *game_server && (*game_server)->gs_codec)) {
        Logger::Instance().LogError("Game server handler table is not fully initialized.", "stoc");
        return false;
    }

    gw::stoc::g_game_server_handlers = &(*game_server)->gs_codec->handlers;
    return gw::stoc::g_game_server_handlers != nullptr;
}

bool __cdecl StoCHandler_Func(gw::packet::stoc::PacketBase* packet) {
    py4gw::HookBase::EnterHook();
    py4gw::HookStatus status = {};

    auto it = gw::stoc::g_packet_entries[packet->header].begin();
    const auto end = gw::stoc::g_packet_entries[packet->header].end();
    while (it != end) {
        if (it->altitude > 0) {
            break;
        }
        it->callback(&status, packet);
        ++status.altitude;
        ++it;
    }

    if (!status.blocked && gw::stoc::g_original_functions) {
        gw::stoc::g_original_functions[packet->header].handler_func(packet);
    }

    while (it != end) {
        it->callback(&status, packet);
        ++status.altitude;
        ++it;
    }

    py4gw::HookBase::LeaveHook();
    return true;
}

bool OriginalHandler(gw::packet::stoc::PacketBase* packet) {
    bool ok = false;
    SafeInitializeCriticalSection(&gw::stoc::g_mutex);
    ::EnterCriticalSection(&gw::stoc::g_mutex);
    if (gw::stoc::g_game_server_handlers &&
        gw::stoc::g_original_functions &&
        gw::stoc::g_stoc_handler_count > packet->header) {
        gw::stoc::g_original_functions[packet->header].handler_func(packet);
    }
    ::LeaveCriticalSection(&gw::stoc::g_mutex);
    return ok;
}

void EnableHooks() {
    ::EnterCriticalSection(&gw::stoc::g_mutex);
    gw::stoc::g_hooks_enabled = true;
    for (uint32_t i = 0; gw::stoc::g_original_functions && i < gw::stoc::g_stoc_handler_count; ++i) {
        gw::stoc::g_original_functions[i] = gw::stoc::g_game_server_handlers->at(i);
        if (!gw::stoc::g_packet_entries[i].empty()) {
            gw::stoc::g_game_server_handlers->at(i).handler_func = &StoCHandler_Func;
        }
    }
    ::LeaveCriticalSection(&gw::stoc::g_mutex);
}

void DisableHooks() {
    CrashContextScope context("shutdown", "stoc", "disable_hooks");
    ::EnterCriticalSection(&gw::stoc::g_mutex);
    gw::stoc::g_hooks_enabled = false;
    if (gw::stoc::g_original_functions) {
        for (uint32_t i = 0; gw::stoc::g_game_server_handlers && i < gw::stoc::g_game_server_handlers->size(); ++i) {
            gw::stoc::g_game_server_handlers->at(i).handler_func = gw::stoc::g_original_functions[i].handler_func;
        }
    }
    ::LeaveCriticalSection(&gw::stoc::g_mutex);
}

void InitOnGameThread() {
    CrashContextScope context("startup", "stoc", "init_on_game_thread");
    SafeInitializeCriticalSection(&gw::stoc::g_mutex);
    ::EnterCriticalSection(&gw::stoc::g_mutex);

    if (!ResolveGameServerHandlers() || !gw::stoc::g_game_server_handlers) {
        ::LeaveCriticalSection(&gw::stoc::g_mutex);
        return;
    }

    gw::stoc::g_stoc_handler_count = gw::stoc::g_game_server_handlers->size();
    Logger::Instance().LogInfo("STOC_HEADER_COUNT [" + std::to_string(gw::stoc::g_stoc_handler_count) + "]");
    PY4GW_ASSERT(gw::stoc::g_stoc_handler_count == gw::stoc::kStoCHeaderCount);

    if (!gw::stoc::g_original_functions) {
        gw::stoc::g_original_functions = new gw::stoc::StoCHandler[gw::stoc::g_stoc_handler_count];
        gw::stoc::g_mutex_initialized = true;
    }
    gw::stoc::g_packet_entries.resize(gw::stoc::g_stoc_handler_count);

    EnableHooks();
    gw::stoc::g_initialized = true;
    ::LeaveCriticalSection(&gw::stoc::g_mutex);
}

void Exit() {
    CrashContextScope context("shutdown", "stoc", "exit");
    DisableHooks();

    delete[] gw::stoc::g_original_functions;
    gw::stoc::g_original_functions = nullptr;
    gw::stoc::g_game_server_handlers = nullptr;
    gw::stoc::g_stoc_handler_count = 0;
    gw::stoc::g_packet_entries.clear();

    if (gw::stoc::g_mutex_initialized) {
        ::DeleteCriticalSection(&gw::stoc::g_mutex);
        gw::stoc::g_mutex_initialized = false;
    }
}

namespace gw::stoc {

CRITICAL_SECTION g_mutex;
bool g_mutex_initialized = false;
bool g_hooks_enabled = false;
std::atomic<bool> g_initialized = false;
size_t g_stoc_handler_count = 0;
StoCHandlerArray* g_game_server_handlers = nullptr;
StoCHandler* g_original_functions = nullptr;
std::vector<std::vector<CallbackEntry>> g_packet_entries;

bool Initialize() {
    CrashContextScope context("startup", "stoc", "initialize");
    if (g_initialized) {
        return true;
    }

    PY4GW_ASSERT(py4gw::Scanner::Initialize());
    PY4GW_ASSERT(py4gw::Patterns::Initialize());

    SafeInitializeCriticalSection(&g_mutex);
    game_thread::Enqueue([] {
        InitOnGameThread();
    });
    return true;
}

void Shutdown() {
    CrashContextScope context("shutdown", "stoc", "shutdown");
    if (!g_mutex_initialized) {
        return;
    }

    Exit();
    g_initialized = false;
}

}  // namespace gw::stoc
