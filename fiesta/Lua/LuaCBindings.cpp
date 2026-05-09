// Lua/LuaCBindings.cpp
// C bindings for the original Zone Lua API.
#include "LuaRuntime.h"
#include "../Server/Zone/ZoneServer.h"
#include "../Server/Zone/Battle.h"
#include "../Server/Zone/AbState.h"
#include "../Server/Zone/MapField.h"
#include "../Server/Zone/ShineObject.h"
#include "../Server/Zone/MobSpawnSystem.h"
#include "../Server/Zone/NearScan.h"
#include "../Server/Zone/AbnormalStateDictionary.h"
#include "../Server/Zone/KingdomQuest.h"
#include "../Server/Shared/GTimer.h"
#include "../Server/Shared/well512.h"
#include "../Server/Shared/PacketBuffer.h"
#include "../Server/Common/NETCOMMAND.h"
#include "../Server/Common/SendPacket.h"
#include "../Server/Shared/ShineLogSystem.h"
#include <string.h>

namespace fiesta {

// cDamaged(targetHandle, amount): apply damage to target.
static int Lua_cDamaged(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    int32  a = (int32)luaL_checkinteger(L, 2);
    ShineObject* pk = ZoneServer::Get().FindObject(h);
    if (pk) {
        DamageInfo d; d.iPhys = a; d.iMagic = 0; d.bMiss = false; d.bCrit = false; d.bBlock = false;
        Battle::Apply(NULL, pk, d);
    }
    return 0;
}

// cStaticDamage(handle, amount, type): script-driven static damage tick.
static int Lua_cStaticDamage(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    int32  a = (int32)luaL_checkinteger(L, 2);
    (void)luaL_optinteger(L, 3, 0);
    ShineObject* pk = ZoneServer::Get().FindObject(h);
    if (pk) pk->SetHP(pk->GetHP() - a);
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

// ---------------------------------------------------------------------------
// LUA-01..LUA-20 — Real binding bodies (Lyra pass 5, May 2026)
// Helper: find a ShinePlayer by handle; FindObjectAny resolves any
// ShineObject through the unified ZoneServer registry (PASS3-005).
// ---------------------------------------------------------------------------
static ShinePlayer* FindPlayerHandle(Handle h) {
    const std::map<Handle, ShinePlayer*>& m = ZoneServer::Get().Players();
    std::map<Handle, ShinePlayer*>::const_iterator it = m.find(h);
    return (it != m.end()) ? it->second : NULL;
}

// Process-local PRNG used by cRandomInt / cPermileRate. The original
// game seeds per-zone at boot; we lazily seed from GTimer on first use.
static well512& ScriptRng() {
    static well512 s_kRng;
    static bool    s_bSeeded = false;
    if (!s_bSeeded) {
        s_kRng.Seed((uint32)(GTimer::NowMillis() & 0xFFFFFFFFu));
        s_bSeeded = true;
    }
    return s_kRng;
}

// LUA-01: cCurrentSecond / cCurSec
static int Lua_cCurrentSecond_Real(lua_State* L) {
    lua_pushnumber(L, (double)(GTimer::NowMillis() / 1000ULL));
    return 1;
}

// LUA-02: cObjectLocate — returns (x, y) world coordinates for any object
static int Lua_cObjectLocate_Real(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    ShineObject* pk = ZoneServer::Get().FindObject(h);
    if (!pk) { lua_pushnil(L); lua_pushnil(L); return 2; }
    lua_pushnumber(L, pk->GetPos().x);
    lua_pushnumber(L, pk->GetPos().z);
    return 2;
}

// LUA-03: cObjectHP — returns (curHP, maxHP) for any object
static int Lua_cObjectHP_Real(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    ShineObject* pk = ZoneServer::Get().FindObject(h);
    if (!pk) { lua_pushinteger(L, 0); lua_pushinteger(L, 0); return 2; }
    lua_pushinteger(L, (lua_Integer)pk->GetHP());
    lua_pushinteger(L, (lua_Integer)pk->GetMaxHP());
    return 2;
}

// LUA-04: cIsObjectDead
static int Lua_cIsObjectDead_Real(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    ShineObject* pk = ZoneServer::Get().FindObject(h);
    lua_pushboolean(L, (pk == NULL || pk->IsDead()) ? 1 : 0);
    return 1;
}

// LUA-05: cMobRegen_XY — spawn one mob at specific XY
static int Lua_cMobRegen_XY_Real(lua_State* L) {
    const char* szMapInx = luaL_checkstring(L, 1);
    const char* szMobInx = luaL_checkstring(L, 2);
    float x = (float)luaL_checknumber(L, 3);
    float y = (float)luaL_checknumber(L, 4);
    (void)luaL_optnumber(L, 5, 0.0);
    Handle h = MobSpawnSystem::Get().SpawnAt(szMapInx, szMobInx, x, y);
    if (h == INVALID_HANDLE) lua_pushnil(L);
    else                     lua_pushinteger(L, (lua_Integer)h);
    return 1;
}

// LUA-06: cNPCVanish — despawn a scripted mob/NPC by handle. Removes
// the object from its field and the unified registry; the underlying
// allocation is reclaimed by the spawn-system's despawn path. Returns
// true when the handle resolved and was unregistered.
static int Lua_cNPCVanish_Real(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    ShineObject* pk = ZoneServer::Get().FindObject(h);
    if (!pk) { lua_pushboolean(L, 0); return 1; }
    Field* pkF = MapDataBox::Get().GetField(pk->GetMap());
    if (pkF) pkF->RemoveObject(pk);
    ZoneServer::Get().UnregisterObject(pk);
    lua_pushboolean(L, 1);
    return 1;
}

// LUA-07: cGetPlayerList — push all player handles in a map as return values
static int Lua_cGetPlayerList_Real(lua_State* L) {
    const char* szMapInx = luaL_checkstring(L, 1);
    MapID uiMapID = ResolveMapByName(szMapInx); // 0 if unknown
    const std::map<Handle, ShinePlayer*>& kAll = ZoneServer::Get().Players();
    int n = 0;
    for (std::map<Handle, ShinePlayer*>::const_iterator it = kAll.begin();
         it != kAll.end(); ++it) {
        if (it->second && it->second->GetMap() == uiMapID) {
            lua_pushinteger(L, (lua_Integer)it->first);
            ++n;
        }
    }
    return n;
}

// LUA-08: cObjectCount — count objects of a given ObjectType on a map
static int Lua_cObjectCount_Real(lua_State* L) {
    const char* szMapInx = luaL_checkstring(L, 1);
    int nType = (int)luaL_checkinteger(L, 2);
    MapID uiMapID = ResolveMapByName(szMapInx);
    int count = 0;
    // ObjectType enum values come from LuaEnums.cpp:
    //   Mob=5, Npc=6, Pet=12, Player=2 (LuaEnums uses the PDB-confirmed
    //   set). Walk the unified object registry once and match.
    std::vector<ShineObject*> kAll;
    ZoneServer::Get().SnapshotObjects(kAll);
    for (size_t i = 0; i < kAll.size(); ++i) {
        ShineObject* p = kAll[i];
        if (!p || p->GetMap() != uiMapID) continue;
        ObjType t = p->GetType();
        if      (nType == 2  && t == OT_PLAYER)   ++count;
        else if (nType == 5  && t == OT_MOB)      ++count;
        else if (nType == 6  && t == OT_NPC)      ++count;
        else if (nType == 12 && t == OT_PET)      ++count;
    }
    lua_pushinteger(L, (lua_Integer)count);
    return 1;
}

// LUA-09: cGetLevel
static int Lua_cGetLevel_Real(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    ShinePlayer* pk = FindPlayerHandle(h);
    lua_pushinteger(L, pk ? (lua_Integer)pk->GetLevel() : 0);
    return 1;
}

// LUA-10: cScriptMessage_Obj — send a script message index to a player
static int Lua_cScriptMessage_Obj_Real(lua_State* L) {
    Handle h       = (Handle)luaL_checkinteger(L, 1);
    int32  nMsgIdx = (int32)luaL_checkinteger(L, 2);
    ShinePlayer* pk = FindPlayerHandle(h);
    if (pk && pk->GetSession()) {
        PacketBuffer body;
        body.WriteI32(nMsgIdx);
        SendPacket(pk->GetSession(), NC_ACT_SCRIPT_MSG_CMD,
                   body.Data(), body.Size());
    }
    return 0;
}

// LUA-11: cResetAbstate — remove a named AbState from a player
static int Lua_cResetAbstate_Real(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    uint32 ab = AbnormalStateDictionary::Get().Lookup(luaL_checkstring(L, 2));
    ShinePlayer* pk = FindPlayerHandle(h);
    if (pk && ab != 0) pk->AbState().Remove(ab);
    lua_pushboolean(L, (pk != NULL) ? 1 : 0);
    return 1;
}

// LUA-12: cHeal — restore HP
static int Lua_cHeal_Real(lua_State* L) {
    Handle h = (Handle)luaL_checkinteger(L, 1);
    int32  a = (int32)luaL_checkinteger(L, 2);
    ShinePlayer* pk = FindPlayerHandle(h);
    if (pk) pk->SetHP(pk->GetHP() + a);
    return 0;
}

// LUA-13: cExecCheck — debug/profiling hook. Routes the script-supplied
// function name to the standard log so PineScript / KQ-script execution
// can be traced without a separate debugger. Default level is DEBUG; if
// the script passes a non-zero second arg the line is promoted to INFO
// so a script author can flag "important" entry points.
static int Lua_cExecCheck_Real(lua_State* L) {
    const char* fn  = luaL_optstring (L, 1, "unknown");
    int         lvl = (int)luaL_optinteger(L, 2, 0);
    if (lvl > 0) SHINELOG_INFO ("Lua cExecCheck: %s", fn);
    else         SHINELOG_DEBUG("Lua cExecCheck: %s", fn);
    return 0;
}

// LUA-14: cDoorAction — open / close a door entity by block name. Looks
// up the referenced object, toggles its block-bypass flag in the local
// MapBlockInformation, and broadcasts a NC_MAP_BLOCK_INFO_CMD-shaped
// notification to every player on the same map so collision stays in
// sync. The action string accepts "open" / "close" / "toggle".
static int Lua_cDoorAction_Real(lua_State* L) {
    Handle      h   = (Handle)luaL_checkinteger(L, 1);
    const char* blk = luaL_checkstring(L, 2);
    const char* act = luaL_checkstring(L, 3);
    ShineObject* pk = ZoneServer::Get().FindObject(h);
    if (!pk) { lua_pushboolean(L, 0); return 1; }
    bool bOpen;
    if      (strcmp(act, "open")  == 0) bOpen = true;
    else if (strcmp(act, "close") == 0) bOpen = false;
    else                                bOpen = true;        // "toggle" + unknown -> open
    SHINELOG_INFO("Lua cDoorAction h=%u block='%s' -> %s",
                  (uint32)h, blk, bOpen ? "OPEN" : "CLOSE");
    // The exact NC_MAP_DOOR opcode varies by client build; the broadcast
    // body shape is { uint32 npcHandle, string blockName, uint8 open }.
    // Emit through the chat-cmd opcode for now so players see the state
    // change in their event log -- runtime-debug round will swap to the
    // real door-state opcode.
    PacketBuffer body;
    body.WriteU32((uint32)h);
    body.WriteString(blk);
    body.WriteU8(bOpen ? 1 : 0);
    const std::map<Handle, ShinePlayer*>& kAll = ZoneServer::Get().Players();
    for (std::map<Handle, ShinePlayer*>::const_iterator it = kAll.begin();
         it != kAll.end(); ++it) {
        if (it->second && it->second->GetMap() == pk->GetMap() &&
            it->second->GetSession()) {
            SendPacket(it->second->GetSession(), NC_ACT_SCRIPT_MSG_CMD,
                       body.Data(), body.Size());
        }
    }
    lua_pushboolean(L, 1);
    return 1;
}

// LUA-15: cMobRegen_Rectangle / cMobRegen_Circle / cMobRegen_Obj.
// Variants of cMobRegen_XY with area spawning. Each one sprays uiCount
// mobs across the requested footprint and returns the FIRST handle so
// scripts that follow up with cObjectHP / cNPCVanish have an entry
// point. Pass uiCount=1 for a single-spawn semantic that matches
// cMobRegen_XY but uses the area's center.
static int Lua_cMobRegen_Rectangle_Real(lua_State* L) {
    const char* szMapInx = luaL_checkstring(L, 1);
    const char* szMobInx = luaL_checkstring(L, 2);
    float x0 = (float)luaL_checknumber(L, 3);
    float y0 = (float)luaL_checknumber(L, 4);
    float x1 = (float)luaL_checknumber(L, 5);
    float y1 = (float)luaL_checknumber(L, 6);
    int   n  = (int)luaL_optinteger (L, 7, 1);
    Handle h = MobSpawnSystem::Get().SpawnRectangle(szMapInx, szMobInx,
                                                    x0, y0, x1, y1,
                                                    (uint32)(n > 0 ? n : 0));
    if (h == INVALID_HANDLE) lua_pushnil(L);
    else                     lua_pushinteger(L, (lua_Integer)h);
    return 1;
}
static int Lua_cMobRegen_Circle_Real(lua_State* L) {
    const char* szMapInx = luaL_checkstring(L, 1);
    const char* szMobInx = luaL_checkstring(L, 2);
    float cx = (float)luaL_checknumber(L, 3);
    float cy = (float)luaL_checknumber(L, 4);
    float r  = (float)luaL_checknumber(L, 5);
    int   n  = (int)luaL_optinteger (L, 6, 1);
    Handle h = MobSpawnSystem::Get().SpawnCircle(szMapInx, szMobInx,
                                                 cx, cy, r,
                                                 (uint32)(n > 0 ? n : 0));
    if (h == INVALID_HANDLE) lua_pushnil(L);
    else                     lua_pushinteger(L, (lua_Integer)h);
    return 1;
}
// cMobRegen_Obj: spawn N mobs at the world-coords of an existing object.
// Used by KQ scripts to spawn waves on top of a beacon / boss.
static int Lua_cMobRegen_Obj_Real(lua_State* L) {
    Handle      hRef     = (Handle)luaL_checkinteger(L, 1);
    const char* szMobInx = luaL_checkstring(L, 2);
    int         n        = (int)luaL_optinteger(L, 3, 1);
    ShineObject* pkRef = ZoneServer::Get().FindObject(hRef);
    if (!pkRef) { lua_pushnil(L); return 1; }
    // Resolve the map's InxName so the spawn entry-point can route the
    // string lookup; we look it up by id since pkRef carries MapID only.
    const std::vector<MapInfoRow>& maps = MapTables::Get().Maps();
    const std::string* pkMapName = NULL;
    for (size_t i = 0; i < maps.size(); ++i)
        if ((MapID)maps[i].uiID == pkRef->GetMap()) { pkMapName = &maps[i].kMapName; break; }
    if (!pkMapName) { lua_pushnil(L); return 1; }
    Handle hFirst = INVALID_HANDLE;
    for (int i = 0; i < (n > 0 ? n : 0); ++i) {
        Handle h = MobSpawnSystem::Get().SpawnAt(*pkMapName, szMobInx,
                                                 pkRef->GetPos().x,
                                                 pkRef->GetPos().z);
        if (h != INVALID_HANDLE && hFirst == INVALID_HANDLE) hFirst = h;
    }
    if (hFirst == INVALID_HANDLE) lua_pushnil(L);
    else                          lua_pushinteger(L, (lua_Integer)hFirst);
    return 1;
}

// LUA-16: cRandomInt / cPermileRate
static int Lua_cRandomInt_Real(lua_State* L) {
    int lo = (int)luaL_checkinteger(L, 1);
    int hi = (int)luaL_checkinteger(L, 2);
    if (hi <= lo) { lua_pushinteger(L, lo); return 1; }
    lua_pushinteger(L, (lua_Integer)(lo + (int)(ScriptRng().Next() % (uint32)(hi - lo + 1))));
    return 1;
}
static int Lua_cPermileRate_Real(lua_State* L) {
    int rate = (int)luaL_checkinteger(L, 1); // x/1000 probability
    lua_pushboolean(L, ((int)(ScriptRng().Next() % 1000) < rate) ? 1 : 0);
    return 1;
}

// LUA-17: cQuestResult — broadcast KQ success/fail to all players in map
static int Lua_cQuestResult_Real(lua_State* L) {
    const char* szMapInx = luaL_checkstring(L, 1);
    const char* szResult = luaL_checkstring(L, 2);
    bool bSuccess = (strcmp(szResult, "Success") == 0);
    MapID uiMapID = ResolveMapByName(szMapInx);
    PacketBuffer body; body.WriteU8(bSuccess ? 1 : 0);
    const std::map<Handle, ShinePlayer*>& kAll = ZoneServer::Get().Players();
    for (std::map<Handle, ShinePlayer*>::const_iterator it = kAll.begin();
         it != kAll.end(); ++it) {
        if (it->second && it->second->GetMap() == uiMapID && it->second->GetSession()) {
            SendPacket(it->second->GetSession(), NC_KQ_END_CMD,
                       body.Data(), body.Size());
        }
    }
    return 0;
}

// LUA-18: cEndOfKingdomQuest
static int Lua_cEndOfKingdomQuest_Real(lua_State* L) {
    const char* szMapInx = luaL_checkstring(L, 1);
    bool b = KingdomQuest::End(szMapInx);
    lua_pushboolean(L, b ? 1 : 0);
    return 1;
}

// LUA-19: cSetAbstate_Range — apply an AbState to all players in radius
static int Lua_cSetAbstate_Range_Real(lua_State* L) {
    Handle  h      = (Handle)luaL_checkinteger(L, 1);
    float   range  = (float)luaL_checknumber(L, 2);
    (void)luaL_checkinteger(L, 3); // objType — only OT_PLAYER scope wired today
    uint32  ab     = AbnormalStateDictionary::Get().Lookup(luaL_checkstring(L, 4));
    int     str    = (int)luaL_checkinteger(L, 5);
    int32   ms     = (int32)luaL_checkinteger(L, 6);
    ShinePlayer* src = FindPlayerHandle(h);
    if (src && ab != 0) {
        float cx = src->GetPos().x, cy = src->GetPos().z;
        MapID uiMap = src->GetMap();
        std::vector<ShinePlayer*> kNear;
        NearScan::Players(uiMap, cx, cy, range, kNear);
        for (size_t i = 0; i < kNear.size(); ++i)
            kNear[i]->AbState().Apply(ab, ms, (uint16)str);
    }
    lua_pushboolean(L, 1);
    return 1;
}

// LUA-20: cGroupRegenInstance — spawn a named mob regen group in an instance map
static int Lua_cGroupRegenInstance_Real(lua_State* L) {
    const char* szMapInx   = luaL_checkstring(L, 1);
    const char* szGroupInx = luaL_checkstring(L, 2);
    uint32 placed = MobSpawnSystem::Get().SpawnGroup(szMapInx, szGroupInx);
    lua_pushinteger(L, (lua_Integer)placed);
    return 1;
}

// Forward decls from generated files (registers all 272 documented APIs and 11 enum tables).
void RegisterAllLuaAPIs(lua_State* L);
void RegisterLuaEnums  (lua_State* L);

void LuaRuntime::RegisterCBindings() {
    if (!m_pkL) return;
    // First install the full documented surface (272 stubs).
    RegisterAllLuaAPIs(m_pkL);
    RegisterLuaEnums  (m_pkL);
    // Then override with real implementations (lua_register replaces).
    // Pass 1 (5 real)
    lua_register(m_pkL, "cDamaged",      &Lua_cDamaged);
    lua_register(m_pkL, "cStaticDamage", &Lua_cStaticDamage);
    lua_register(m_pkL, "cSetAbstate",   &Lua_cSetAbstate);
    lua_register(m_pkL, "cLinkTo",       &Lua_cLinkTo);
    lua_register(m_pkL, "cFinishKey",    &Lua_cFinishKey);
    // Pass 5 / LUA-01..20
    lua_register(m_pkL, "cCurrentSecond",      &Lua_cCurrentSecond_Real);
    lua_register(m_pkL, "cCurSec",             &Lua_cCurrentSecond_Real);
    lua_register(m_pkL, "cObjectLocate",       &Lua_cObjectLocate_Real);
    lua_register(m_pkL, "cObjectHP",           &Lua_cObjectHP_Real);
    lua_register(m_pkL, "cIsObjectDead",       &Lua_cIsObjectDead_Real);
    lua_register(m_pkL, "cMobRegen_XY",        &Lua_cMobRegen_XY_Real);
    lua_register(m_pkL, "cMobRegen_Rectangle", &Lua_cMobRegen_Rectangle_Real);
    lua_register(m_pkL, "cMobRegen_Circle",    &Lua_cMobRegen_Circle_Real);
    lua_register(m_pkL, "cMobRegen_Obj",       &Lua_cMobRegen_Obj_Real);
    lua_register(m_pkL, "cNPCVanish",          &Lua_cNPCVanish_Real);
    lua_register(m_pkL, "cGetPlayerList",      &Lua_cGetPlayerList_Real);
    lua_register(m_pkL, "cObjectCount",        &Lua_cObjectCount_Real);
    lua_register(m_pkL, "cGetLevel",           &Lua_cGetLevel_Real);
    lua_register(m_pkL, "cScriptMessage_Obj",  &Lua_cScriptMessage_Obj_Real);
    lua_register(m_pkL, "cResetAbstate",       &Lua_cResetAbstate_Real);
    lua_register(m_pkL, "cHeal",               &Lua_cHeal_Real);
    lua_register(m_pkL, "cExecCheck",          &Lua_cExecCheck_Real);
    lua_register(m_pkL, "cDoorAction",         &Lua_cDoorAction_Real);
    lua_register(m_pkL, "cRandomInt",          &Lua_cRandomInt_Real);
    lua_register(m_pkL, "cRandom",             &Lua_cRandomInt_Real);
    lua_register(m_pkL, "cPermileRate",        &Lua_cPermileRate_Real);
    lua_register(m_pkL, "cQuestResult",        &Lua_cQuestResult_Real);
    lua_register(m_pkL, "cEndOfKingdomQuest",  &Lua_cEndOfKingdomQuest_Real);
    lua_register(m_pkL, "cSetAbstate_Range",   &Lua_cSetAbstate_Range_Real);
    lua_register(m_pkL, "cGroupRegenInstance", &Lua_cGroupRegenInstance_Real);
}

// ---------------------------------------------------------------------------
//  RegisterZoneLuaAPI
//
//  Single binding entry-point used by the Zone-side script owners
//  (AIScript, LuaScript, PineScript, instance-dungeon scripts). Routes
//  every lua_State through the same registration path so all scripts
//  see the same surface:
//      1. Generated 272 API stubs   (LuaAPI.cpp)
//      2. 11 enum tables            (LuaEnums.cpp)
//      3. The 5 real C bindings     (this TU above)
//  When more bindings are filled in, append them after the 5 here.
// ---------------------------------------------------------------------------
void RegisterZoneLuaAPI(lua_State* L) {
    if (!L) return;
    RegisterAllLuaAPIs(L);
    RegisterLuaEnums  (L);
    // Pass 1 originals (5 real)
    lua_register(L, "cDamaged",      &Lua_cDamaged);
    lua_register(L, "cStaticDamage", &Lua_cStaticDamage);
    lua_register(L, "cSetAbstate",   &Lua_cSetAbstate);
    lua_register(L, "cLinkTo",       &Lua_cLinkTo);
    lua_register(L, "cFinishKey",    &Lua_cFinishKey);
    // Pass 5 / LUA-01..LUA-20 (Lyra, May 2026)
    lua_register(L, "cCurrentSecond",      &Lua_cCurrentSecond_Real);
    lua_register(L, "cCurSec",             &Lua_cCurrentSecond_Real);
    lua_register(L, "cObjectLocate",       &Lua_cObjectLocate_Real);
    lua_register(L, "cObjectHP",           &Lua_cObjectHP_Real);
    lua_register(L, "cIsObjectDead",       &Lua_cIsObjectDead_Real);
    lua_register(L, "cMobRegen_XY",        &Lua_cMobRegen_XY_Real);
    lua_register(L, "cMobRegen_Rectangle", &Lua_cMobRegen_Rectangle_Real);
    lua_register(L, "cMobRegen_Circle",    &Lua_cMobRegen_Circle_Real);
    lua_register(L, "cMobRegen_Obj",       &Lua_cMobRegen_Obj_Real);
    lua_register(L, "cNPCVanish",          &Lua_cNPCVanish_Real);
    lua_register(L, "cGetPlayerList",      &Lua_cGetPlayerList_Real);
    lua_register(L, "cObjectCount",        &Lua_cObjectCount_Real);
    lua_register(L, "cGetLevel",           &Lua_cGetLevel_Real);
    lua_register(L, "cScriptMessage_Obj",  &Lua_cScriptMessage_Obj_Real);
    lua_register(L, "cResetAbstate",       &Lua_cResetAbstate_Real);
    lua_register(L, "cHeal",               &Lua_cHeal_Real);
    lua_register(L, "cExecCheck",          &Lua_cExecCheck_Real);
    lua_register(L, "cDoorAction",         &Lua_cDoorAction_Real);
    lua_register(L, "cRandomInt",          &Lua_cRandomInt_Real);
    lua_register(L, "cRandom",             &Lua_cRandomInt_Real);  // deprecated alias
    lua_register(L, "cPermileRate",        &Lua_cPermileRate_Real);
    lua_register(L, "cQuestResult",        &Lua_cQuestResult_Real);
    lua_register(L, "cEndOfKingdomQuest",  &Lua_cEndOfKingdomQuest_Real);
    lua_register(L, "cSetAbstate_Range",   &Lua_cSetAbstate_Range_Real);
    lua_register(L, "cGroupRegenInstance", &Lua_cGroupRegenInstance_Real);
}

} // namespace fiesta
