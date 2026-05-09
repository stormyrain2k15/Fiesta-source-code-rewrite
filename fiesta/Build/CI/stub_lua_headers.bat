@echo off
REM ============================================================================
REM Lua 5.2 minimal header stubs for CI. Lets the codebase compile far
REM enough to surface ODR / syntax / symbol errors without redistributing
REM Lua 5.2 itself. NEVER use these stubs for an actual link -- pull a
REM real Lua 5.2 build for that.
REM ============================================================================
set "OUT=ThirdParty\Lua\include"
mkdir "%OUT%" 2>nul
> "%OUT%\lua.h" (
  echo #ifndef LUA_H
  echo #define LUA_H
  echo #define LUA_VERSION_NUM 502
  echo typedef struct lua_State lua_State;
  echo typedef int  ^(*lua_CFunction^)^(lua_State* L^);
  echo lua_State* luaL_newstate^(void^);
  echo void       lua_close^(lua_State* L^);
  echo int        luaL_loadbufferx^(lua_State* L, const char* buff, size_t sz, const char* name, const char* mode^);
  echo int        lua_pcallk^(lua_State* L, int nargs, int nresults, int errfunc, int ctx, lua_CFunction k^);
  echo void       lua_pushstring^(lua_State* L, const char* s^);
  echo void       lua_pushinteger^(lua_State* L, long n^);
  echo void       lua_pushnumber^(lua_State* L, double n^);
  echo void       lua_pushcfunction^(lua_State* L, lua_CFunction f^);
  echo const char* lua_tolstring^(lua_State* L, int idx, size_t* len^);
  echo long        lua_tointegerx^(lua_State* L, int idx, int* isnum^);
  echo double      lua_tonumberx^(lua_State* L, int idx, int* isnum^);
  echo int         lua_gettop^(lua_State* L^);
  echo void        lua_settop^(lua_State* L, int idx^);
  echo void        lua_setglobal^(lua_State* L, const char* name^);
  echo void        lua_getglobal^(lua_State* L, const char* name^);
  echo void        lua_createtable^(lua_State* L, int narr, int nrec^);
  echo void        lua_settable^(lua_State* L, int idx^);
  echo void        lua_setfield^(lua_State* L, int idx, const char* k^);
  echo int         lua_type^(lua_State* L, int idx^);
  echo #define LUA_MULTRET    ^(-1^)
  echo #define lua_pop^(L,n^) lua_settop^(L, -^(n^)-1^)
  echo #define lua_pcall^(L,n,r,f^) lua_pcallk^(L,^(n^),^(r^),^(f^),0,0^)
  echo #define lua_tostring^(L,i^) lua_tolstring^(L,^(i^),0^)
  echo #endif
)
> "%OUT%\lauxlib.h" (
  echo #ifndef LAUXLIB_H
  echo #define LAUXLIB_H
  echo #include "lua.h"
  echo void luaL_openlibs^(lua_State* L^);
  echo int  luaL_error^(lua_State* L, const char* fmt, ...^);
  echo const char* luaL_checklstring^(lua_State* L, int n, size_t* l^);
  echo long        luaL_checkinteger^(lua_State* L, int n^);
  echo double      luaL_checknumber^(lua_State* L, int n^);
  echo #define luaL_checkstring^(L,n^) luaL_checklstring^(^(L^),^(n^),0^)
  echo #endif
)
> "%OUT%\lualib.h" (
  echo #ifndef LUALIB_H
  echo #define LUALIB_H
  echo #include "lua.h"
  echo #endif
)
echo Stubbed Lua 5.2 headers in %OUT%
