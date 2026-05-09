// Lua/LuaAPI.h
#ifndef FIESTA_LUA_LUAAPI_H
#define FIESTA_LUA_LUAAPI_H
extern "C" { struct lua_State; }
namespace fiesta {
void RegisterAllLuaAPIs(struct lua_State* L);
} // namespace fiesta
#endif