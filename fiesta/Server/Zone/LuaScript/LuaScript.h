// Server/Zone/LuaScript/LuaScript.h
// Lua 5.2 VM wrapper. Used exclusively for Instance-Dungeon scripting --
// the per-ID script declares OnEnter / OnPhase / OnTimer / OnMobDeath
// callbacks. Each ID gets its own lua_State; AI scripts (per-mob behaviour)
// share the global state owned by AIScript.cpp.
#ifndef SHINE_ZONE_LUASCRIPT_H
#define SHINE_ZONE_LUASCRIPT_H
#include "../../Shared/ShineTypes.h"
#include <string>

struct lua_State;

namespace shine {

class LuaScript {
public:
    LuaScript();
    ~LuaScript();
    bool LoadFile(const std::string& rPath);
    bool Call    (const char* szFunc);                            // 0-arg
    bool CallI   (const char* szFunc, int32 iArg);                // 1 int arg
    bool CallII  (const char* szFunc, int32 a, int32 b);          // 2 int args
    lua_State* L() { return m_pkL; }
    static void RegisterAllBindings(lua_State* L);                // C-side bindings
private:
    lua_State* m_pkL;
};

} // namespace shine
#endif
