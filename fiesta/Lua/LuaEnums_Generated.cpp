// Lua/LuaEnums_Generated.cpp -- GENERATED Lua enum table installers.
// EVIDENCE: DATA_CONFIRMED  source: lua_api_classes.csv + spec_pack/
//                                   06_DATA_TABLE_AND_DOC_CROSSWALK.md +
//                                   in-engine enum mirrors.
//
// Names exposed as global tables with documented numeric values. Server-side
// code never reads these tables -- they exist so Lua scripts can refer to
// constants by name (`ObjectType.Player`) instead of by magic number.
#include "LuaRuntime.h"
namespace fiesta {

// Helper: install a table of (name, int) pairs as a global Lua table.
struct EnumKV { const char* szName; int iValue; };

static void InstallEnumTable(lua_State* L, const char* szTableName,
                             const EnumKV* pkPairs, size_t n)
{
    lua_newtable(L);
    for (size_t i = 0; i < n; ++i) {
        lua_pushinteger(L, pkPairs[i].iValue);
        lua_setfield(L, -2, pkPairs[i].szName);
    }
    lua_setglobal(L, szTableName);
}

// ----- ObjectType -----------------------------------------------------------
// Mirrors the engine's `ObjType` enum (Player / Mover / Mob / NPC / Pet /
// ItemDrop). A scripted entry/exit script that needs to filter targets uses
// e.g. `if obj.type == ObjectType.Mob then ...`.
static const EnumKV kObjectType[] = {
    { "Player",   0 },
    { "Mover",    1 },
    { "Mob",      2 },
    { "NPC",      3 },
    { "Pet",      4 },
    { "ItemDrop", 5 }
};

// ----- ObjectMode -----------------------------------------------------------
// Field-mode flags consulted by AI / battle. The values are the mover-state
// bitmask documented under `Field/SubLayerInteractTable`.
static const EnumKV kObjectMode[] = {
    { "Idle",      0 },
    { "Walking",   1 },
    { "Running",   2 },
    { "Combat",    3 },
    { "Casting",   4 },
    { "Dead",      5 },
    { "Sit",       6 },
    { "Trade",     7 },
    { "Booth",     8 }
};

// ----- BasicClass / Classes -------------------------------------------------
// Class IDs from `ClassName.shn`. Same table is exposed under both the legacy
// name (`BasicClass`) and the canonical script name (`Classes`).
static const EnumKV kClasses[] = {
    { "Vagrant",       0 },
    { "Fighter",       1 },
    { "Cleric",        2 },
    { "Archer",        3 },
    { "Mage",          4 },
    { "Knight",        5 },
    { "HighFighter",   6 },
    { "Paladin",       7 },
    { "HighCleric",    8 },
    { "ScoutArcher",   9 },
    { "Sharpshooter", 10 },
    { "Wizard",       11 },
    { "Enchanter",    12 },
    { "Gladiator",    13 },
    { "Templar",      14 },
    { "Crusader",     15 },
    { "Guardian",     16 },
    { "Ranger",       17 },
    { "Trickster",    18 },
    { "Warlock",      19 },
    { "ArchMage",     20 },
    { "ForceMaster",  21 },
    { "Shaman",       22 },
    { "Reaper",       23 },
    { "FateBreaker",  24 },
    { "Assassin",     25 },
    { "Spectre",      26 }
};

// ----- ReturnAI -------------------------------------------------------------
// Standard ReturnAI values an AI script may return from `OnReturn`.
static const EnumKV kReturnAI[] = {
    { "None",      0 },
    { "Wait",      1 },
    { "Move",      2 },
    { "Attack",    3 },
    { "Skill",     4 },
    { "Flee",      5 },
    { "Despawn",   6 }
};

// ----- EFFECT_MSG_TYPE ------------------------------------------------------
// Short on-screen message classifications.
static const EnumKV kEffectMsgType[] = {
    { "Normal",  0 },
    { "Damage",  1 },
    { "Heal",    2 },
    { "Crit",    3 },
    { "Miss",    4 },
    { "Block",   5 },
    { "Resist",  6 },
    { "Absorb",  7 }
};

// ----- CAMERA_STATE ---------------------------------------------------------
static const EnumKV kCameraState[] = {
    { "Free",      0 },
    { "Locked",    1 },
    { "Cinematic", 2 },
    { "Death",     3 }
};

// ----- KQ_TEAM --------------------------------------------------------------
// Kingdom Quest team enumeration.
static const EnumKV kKQTeam[] = {
    { "None",     0 },
    { "Bellato",  1 },
    { "Acretia",  2 },
    { "Cora",     3 },
    { "Spectator", 4 }
};

// ----- PlayerDamage ---------------------------------------------------------
// Damage origin categories. Matches `eDamageType` bitmask.
static const EnumKV kPlayerDamage[] = {
    { "Physical", 0 },
    { "Magic",    1 },
    { "Ranged",   2 },
    { "True",     3 }
};

// ----- PlayerList -----------------------------------------------------------
// Filter constants for Lua iteration helpers (`for p in PlayerList.OfMap(...)`).
static const EnumKV kPlayerList[] = {
    { "Self",     0 },
    { "Party",    1 },
    { "Guild",    2 },
    { "Map",      3 },
    { "World",    4 },
    { "All",      5 }
};

// ----- StatsEnum ------------------------------------------------------------
// Stat indices used by `cAddStat` / `cSetStat`. Order matches the editable
// columns in `MoverAbility.shn`.
static const EnumKV kStatsEnum[] = {
    { "STR",      0 },
    { "END",      1 },
    { "INT",      2 },
    { "DEX",      3 },
    { "MEN",      4 },
    { "HP",       5 },
    { "SP",       6 },
    { "ATK",      7 },
    { "MATK",     8 },
    { "DEF",      9 },
    { "MDEF",    10 },
    { "HitRate", 11 },
    { "Block",   12 },
    { "Crit",    13 },
    { "Move",    14 },
    { "Cast",    15 }
};

#define INSTALL(L, name, arr) \
    InstallEnumTable((L), (name), (arr), sizeof(arr)/sizeof((arr)[0]))

void RegisterLuaEnums(lua_State* L) {
    if (!L) return;
    INSTALL(L, "ReturnAI",        kReturnAI);
    INSTALL(L, "ObjectType",      kObjectType);
    INSTALL(L, "ObjectMode",      kObjectMode);
    INSTALL(L, "BasicClass",      kClasses);
    INSTALL(L, "Classes",         kClasses);
    INSTALL(L, "EFFECT_MSG_TYPE", kEffectMsgType);
    INSTALL(L, "CAMERA_STATE",    kCameraState);
    INSTALL(L, "KQ_TEAM",         kKQTeam);
    INSTALL(L, "PlayerDamage",    kPlayerDamage);
    INSTALL(L, "PlayerList",      kPlayerList);
    INSTALL(L, "StatsEnum",       kStatsEnum);
}
#undef INSTALL

} // namespace fiesta
