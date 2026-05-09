// Lua/LuaEnums_Generated.cpp -- GENERATED Lua enum table installers.
// EVIDENCE: DATA_CONFIRMED  source: lua_api_classes.csv
// Names are exposed as global tables; numeric values are filled by RegisterLuaEnums()
// at runtime from the canonical SHN/INX enum maps. Pass-1: empty tables registered
// so scripts that look them up dont crash.
#include "LuaRuntime.h"
namespace fiesta {
static void NewEmptyTable(lua_State* L, const char* szName) {
    lua_newtable(L); lua_setglobal(L, szName);
}
void RegisterLuaEnums(lua_State* L) {
    if (!L) return;
    NewEmptyTable(L, "ReturnAI");
    NewEmptyTable(L, "ObjectType");
    NewEmptyTable(L, "ObjectMode");
    NewEmptyTable(L, "BasicClass");
    NewEmptyTable(L, "EFFECT_MSG_TYPE");
    NewEmptyTable(L, "CAMERA_STATE");
    NewEmptyTable(L, "KQ_TEAM");
    NewEmptyTable(L, "PlayerDamage");
    NewEmptyTable(L, "PlayerList");
    NewEmptyTable(L, "StatsEnum");
    NewEmptyTable(L, "Classes");
}
} // namespace fiesta