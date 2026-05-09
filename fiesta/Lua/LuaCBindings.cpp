// Lua/LuaCBindings.cpp
// 21 -- C bindings for the original Zone Lua API.
// EVIDENCE: PDB_CONFIRMED  symbol: cDamaged, cStaticDamage, cSetAbstate, cLinkTo, cFinishKey
#include "LuaRuntime.h"
#include "../Server/Zone/ZoneServer.h"
#include "../Server/Zone/Battle.h"
#include "../Server/Zone/AbState.h"
#include "../Server/Zone/MapField.h"

namespace fiesta {

// cDamaged(targetHandle, amount): apply damage to target.
static int Lua_cDamaged(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    int32  a = (int32)luaL_checkinteger(L, 2);
    std::map<Handle, ShinePlayer*>::const_iterator it = ZoneServer::Get().Players().find(h);
    if (it != ZoneServer::Get().Players().end()) {
        DamageInfo d; d.iPhys = a; d.iMagic = 0; d.bMiss = false; d.bCrit = false; d.bBlock = false;
        Battle::Apply(NULL, it->second, d);
    }
    return 0;
}

// cStaticDamage(handle, amount, type): script-driven static damage tick.
static int Lua_cStaticDamage(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    int32  a = (int32)luaL_checkinteger(L, 2);
    (void)luaL_optinteger(L, 3, 0);
    std::map<Handle, ShinePlayer*>::const_iterator it = ZoneServer::Get().Players().find(h);
    if (it != ZoneServer::Get().Players().end()) it->second->SetHP(it->second->GetHP() - a);
    return 0;
}

// cSetAbstate(handle, abInxName, durationMs): set abnormal state.
static int Lua_cSetAbstate(lua_State* L) {
    (void)luaL_checkinteger(L, 1);
    uint32 ab = (uint32)luaL_checkinteger(L, 2);
    int32  ms = (int32)luaL_checkinteger(L, 3);
    static AbnormalState s_kBus; // pass-1: per-player AbState wiring is in pass 2
    s_kBus.Apply(ab, ms);
    return 0;
}

// cLinkTo(handle, mapId, x, y, z): teleport.
static int Lua_cLinkTo(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    MapID  m = (MapID )luaL_checkinteger(L, 2);
    Vec3 p((float)luaL_checknumber(L, 3), (float)luaL_checknumber(L, 4), (float)luaL_checknumber(L, 5));
    std::map<Handle, ShinePlayer*>::const_iterator it = ZoneServer::Get().Players().find(h);
    if (it != ZoneServer::Get().Players().end()) TownPortal(it->second, m, p);
    return 0;
}

// cFinishKey(handle, key): script flow control marker.
static int Lua_cFinishKey(lua_State* L) {
    (void)luaL_checkinteger(L, 1); (void)luaL_checkinteger(L, 2);
    return 0;
}

void LuaRuntime::RegisterCBindings() {
    if (!m_pkL) return;
    lua_register(m_pkL, "cDamaged",      &Lua_cDamaged);
    lua_register(m_pkL, "cStaticDamage", &Lua_cStaticDamage);
    lua_register(m_pkL, "cSetAbstate",   &Lua_cSetAbstate);
    lua_register(m_pkL, "cLinkTo",       &Lua_cLinkTo);
    lua_register(m_pkL, "cFinishKey",    &Lua_cFinishKey);
}

} // namespace fiesta
