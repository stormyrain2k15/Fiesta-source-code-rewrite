// Lua/LuaEnums_Generated.cpp
//                                   06_DATA_TABLE_AND_DOC_CROSSWALK.md +
//                                   in-engine enum mirrors.
// Names exposed as global tables with documented numeric values. Server-side
// code never reads these tables -- they exist so Lua scripts can refer to
// constants by name (`ObjectType.Player`) instead of by magic number.
#include "LuaRuntime.h"
namespace fiesta {

// Helper: install a table of (name, int) pairs as a global Lua table.
struct EnumKV   { const char* szName; int iValue; };
struct EnumStrKV{ const char* szName; const char* szValue; };

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

// String-valued enum installer. Some Lua tables (notably ObjectMode)
// use lowercase string ids, not integers, when referenced from script.
static void InstallEnumStrTable(lua_State* L, const char* szTableName,
                                const EnumStrKV* pkPairs, size_t n)
{
    lua_newtable(L);
    for (size_t i = 0; i < n; ++i) {
        lua_pushstring(L, pkPairs[i].szValue);
        lua_setfield(L, -2, pkPairs[i].szName);
    }
    lua_setglobal(L, szTableName);
}

// ----- ObjectType -----------------------------------------------------------
// PDB-confirmed values from common.lua (NA2016). The earlier table was
// invented; scripts that test e.g. `ObjectType.Mob` would have got the
// wrong handle class. Don't reorder -- numeric values are part of the
// wire/script contract.
static const EnumKV kObjectType[] = {
    { "Invalid",    -1 },
    { "Flag",        0 },
    { "DropItem",    1 },
    { "Player",      2 },
    { "MiniHouse",   3 },
    { "NPC",         4 },
    { "Mob",         5 },
    { "MagicField",  6 },
    { "Door",        7 },
    { "Bandit",      8 },
    { "Effect",      9 },
    { "Servant",    10 },
    { "Mover",      11 },
    { "Pet",        12 },
    { "Max",        13 }
};

// ----- ObjectMode -----------------------------------------------------------
// Lowercase string ids from common.lua. Scripts compare against
// strings, not numbers; the earlier int table broke every state-machine
// branch. Installed via the string-valued helper.
static const EnumStrKV kObjectMode[] = {
    { "Linking",    "linking"    },
    { "Normal",     "normal"     },
    { "Fight",      "fight"      },
    { "Corpse",     "corpse"     },
    { "House",      "house"      },
    { "Booth",      "booth"      },
    { "Riding",     "riding"     },
    { "LogoutWait", "logoutwait" }
};

// ----- BasicClass / Classes -------------------------------------------------
// PDB-confirmed class IDs (steps of 5). The earlier table used 0..26
// sequential which broke every Sentinel AI script. Don't change the
// numeric values -- they're the contract with ClassName.shn and with
// every PineScript/Lua entry that does `class == Classes.Sentinel`.
static const EnumKV kClasses[] = {
    { "None",        0 },
    { "Fighter",     1 },
    { "Cleric",      6 },
    { "Archer",     11 },
    { "Mage",       16 },
    { "Joker",      21 },     // local name "Trickster" in some scripts
    { "Sentinel",   26 }      // mandatory -- omitted earlier broke Sentinel AI
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
#define INSTALL_STR(L, name, arr) \
    InstallEnumStrTable((L), (name), (arr), sizeof(arr)/sizeof((arr)[0]))

void RegisterLuaEnums(lua_State* L) {
    if (!L) return;
    INSTALL    (L, "ReturnAI",        kReturnAI);
    INSTALL    (L, "ObjectType",      kObjectType);
    INSTALL_STR(L, "ObjectMode",      kObjectMode);   // STRING-valued
    INSTALL    (L, "BasicClass",      kClasses);
    INSTALL    (L, "Classes",         kClasses);
    INSTALL    (L, "EFFECT_MSG_TYPE", kEffectMsgType);
    INSTALL    (L, "CAMERA_STATE",    kCameraState);
    INSTALL    (L, "KQ_TEAM",         kKQTeam);
    INSTALL    (L, "PlayerDamage",    kPlayerDamage);
    INSTALL    (L, "PlayerList",      kPlayerList);
    INSTALL    (L, "StatsEnum",       kStatsEnum);
}
#undef INSTALL
#undef INSTALL_STR

} // namespace fiesta
