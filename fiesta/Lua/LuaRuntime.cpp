// Lua/LuaRuntime.cpp
#include "LuaRuntime.h"
#include "../Server/Shared/ShineLogSystem.h"

namespace fiesta {

LuaRuntime::LuaRuntime() : m_pkL(NULL) {}
LuaRuntime::~LuaRuntime() { Close(); }

bool LuaRuntime::Open() {
    if (m_pkL) return true;
    m_pkL = luaL_newstate();
    if (!m_pkL) return false;
    luaL_openlibs(m_pkL);
    RegisterCBindings();
    return true;
}

void LuaRuntime::Close() {
    if (m_pkL) { lua_close(m_pkL); m_pkL = NULL; }
}

bool LuaRuntime::DoFile(const char* szPath) {
    if (!m_pkL) return false;
    if (luaL_dofile(m_pkL, szPath) != LUA_OK) {
        SHINELOG_ERROR("LuaRuntime DoFile %s : %s", szPath, lua_tostring(m_pkL, -1));
        lua_pop(m_pkL, 1); return false;
    }
    return true;
}

bool LuaRuntime::DoString(const char* szSrc) {
    if (!m_pkL) return false;
    if (luaL_dostring(m_pkL, szSrc) != LUA_OK) {
        SHINELOG_ERROR("LuaRuntime DoString : %s", lua_tostring(m_pkL, -1));
        lua_pop(m_pkL, 1); return false;
    }
    return true;
}

} // namespace fiesta
