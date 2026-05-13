// Server/Zone/LuaScript/LuaScript.cpp
// Real implementation lives in /app/shine/Lua/LuaRuntime; this file
// is the Zone-side adapter that owns one lua_State per InstanceDungeon
// instance and binds the C-side game API.
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "LuaScript.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

// C-side bindings prefixed `Shine_*` are registered in
// /app/shine/Lua/Generated/api_zone.cpp -- a long table pulled from the
// PDB's Lua thunk symbols. Forward-declared here so we don't pull the
// generated header into every Zone include path.
extern void RegisterZoneLuaAPI(lua_State* L);

LuaScript::LuaScript() : m_pkL(luaL_newstate()) {
    if (m_pkL) {
        luaL_openlibs(m_pkL);
        RegisterAllBindings(m_pkL);
    }
}

LuaScript::~LuaScript() { if (m_pkL) lua_close(m_pkL); }

bool LuaScript::LoadFile(const std::string& rPath) {
    if (!m_pkL) return false;
    if (luaL_loadfile(m_pkL, rPath.c_str()) || lua_pcall(m_pkL, 0, 0, 0)) {
        SHINELOG_WARN("Lua load '%s' err: %s", rPath.c_str(), lua_tostring(m_pkL, -1));
        lua_pop(m_pkL, 1); return false;
    }
    return true;
}

bool LuaScript::Call(const char* szFunc) {
    if (!m_pkL) return false;
    lua_getglobal(m_pkL, szFunc);
    if (!lua_isfunction(m_pkL, -1)) { lua_pop(m_pkL, 1); return false; }
    if (lua_pcall(m_pkL, 0, 0, 0)) {
        SHINELOG_WARN("Lua call '%s' err: %s", szFunc, lua_tostring(m_pkL, -1));
        lua_pop(m_pkL, 1); return false;
    }
    return true;
}

bool LuaScript::CallI(const char* szFunc, int32 iArg) {
    if (!m_pkL) return false;
    lua_getglobal(m_pkL, szFunc);
    if (!lua_isfunction(m_pkL, -1)) { lua_pop(m_pkL, 1); return false; }
    lua_pushinteger(m_pkL, iArg);
    if (lua_pcall(m_pkL, 1, 0, 0)) {
        SHINELOG_WARN("Lua call '%s' err: %s", szFunc, lua_tostring(m_pkL, -1));
        lua_pop(m_pkL, 1); return false;
    }
    return true;
}

bool LuaScript::CallII(const char* szFunc, int32 a, int32 b) {
    if (!m_pkL) return false;
    lua_getglobal(m_pkL, szFunc);
    if (!lua_isfunction(m_pkL, -1)) { lua_pop(m_pkL, 1); return false; }
    lua_pushinteger(m_pkL, a);
    lua_pushinteger(m_pkL, b);
    if (lua_pcall(m_pkL, 2, 0, 0)) {
        SHINELOG_WARN("Lua call '%s' err: %s", szFunc, lua_tostring(m_pkL, -1));
        lua_pop(m_pkL, 1); return false;
    }
    return true;
}

void LuaScript::RegisterAllBindings(lua_State* L) { RegisterZoneLuaAPI(L); }

} // namespace shine
