// Server/Zone/AIScript/AIScript.cpp
// Per-mob Lua AI -- each mob's "AIScript" column in MobInfoServer.shn
// names a *.lua file under Data/AI/. Multiple mobs share one lua_State
// for memory efficiency; per-mob context is pushed as a table at call.
extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}
#include "../../Shared/ShineTypes.h"
#include "../../Shared/ShineLogSystem.h"
#include <string>
#include <map>

namespace fiesta {

extern void RegisterZoneLuaAPI(lua_State* L);

class AIScript {
public:
    static AIScript& Get() { static AIScript s; return s; }
    bool LoadAll(const std::string& rDir);
    bool RunDecide(uint32 uiMobId, const std::string& rScript);
    lua_State* L() { return m_pkL; }
private:
    AIScript() : m_pkL(luaL_newstate()) {
        if (m_pkL) { luaL_openlibs(m_pkL); RegisterZoneLuaAPI(m_pkL); }
    }
    lua_State* m_pkL;
    std::map<std::string, bool> m_kLoaded;
};

bool AIScript::LoadAll(const std::string& /*rDir*/) { return true; }

bool AIScript::RunDecide(uint32 uiMobId, const std::string& rScript) {
    if (!m_pkL || rScript.empty()) return false;
    if (!m_kLoaded[rScript]) {
        if (luaL_loadfile(m_pkL, rScript.c_str()) || lua_pcall(m_pkL, 0, 0, 0)) {
            SHINELOG_WARN("AI Lua '%s' load err: %s", rScript.c_str(), lua_tostring(m_pkL, -1));
            lua_pop(m_pkL, 1); return false;
        }
        m_kLoaded[rScript] = true;
    }
    lua_getglobal(m_pkL, "Decide");
    if (!lua_isfunction(m_pkL, -1)) { lua_pop(m_pkL, 1); return false; }
    lua_pushinteger(m_pkL, (lua_Integer)uiMobId);
    if (lua_pcall(m_pkL, 1, 0, 0)) {
        SHINELOG_WARN("AI Decide err: %s", lua_tostring(m_pkL, -1));
        lua_pop(m_pkL, 1); return false;
    }
    return true;
}

} // namespace fiesta
