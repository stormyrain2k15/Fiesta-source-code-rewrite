// Lua/LuaAPI.h
#ifndef SHINE_LUA_LUAAPI_H
#define SHINE_LUA_LUAAPI_H
extern "C" { struct lua_State; }
namespace shine {
void RegisterAllLuaAPIs(struct lua_State* L);
} // namespace shine
#endif