// Lua/LuaRuntime.h
// thin Lua 5.2 host. The original Zone embeds Lua 5.2; this wrapper exposes
// per-script states + lifecycle for instance/KQ/event/script-AI use.
// NOTE: lua.h / lauxlib.h / lualib.h must be a Lua 5.2 distribution (5.2.x). The
// vendored Lua source is expected to be added to the build by the user; this
// header expresses the consumer-side interface only.
#ifndef SHINE_LUA_LUARUNTIME_H
#define SHINE_LUA_LUARUNTIME_H
#include "../Server/Shared/ShineTypes.h"

extern "C" {
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
}

namespace shine {

class LuaRuntime {
public:
    LuaRuntime();
    ~LuaRuntime();
    bool   Open();
    void   Close();
    bool   DoFile  (const char* szPath);
    bool   DoString(const char* szSrc);
    void   RegisterCBindings();   // installs cDamaged / cStaticDamage / cSetAbstate / cLinkTo / cFinishKey
    lua_State* L() { return m_pkL; }
private:
    lua_State* m_pkL;
};

} // namespace shine
#endif
