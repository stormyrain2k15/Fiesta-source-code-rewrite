// Server/Zone/MoverTables.h
// typed loaders for the per-class growth and ability tables that drive
// stat distribution at character login + level up.
//   MoverMain.shn    -- one row per (Class, Level) with base HP/SP/ATK/DEF/etc.
//   MoverAbility.shn -- one row per Class with the per-stat-point gains
//                       (delta HP/SP/ATK/MATK/DEF/MDEF/Hit/Crit/Dodge per
//                        STR/END/DEX/INT/MEN allocated).
#ifndef SHINE_ZONE_MOVERTABLES_H
#define SHINE_ZONE_MOVERTABLES_H
#include "BattleStat.h"
#include "StatDistribute.h"
#include <map>
#include <string>

namespace shine {

struct LegacyMoverMainRow {
    uint8  bClass;
    uint16 uiLevel;
    int32  nBaseHP;
    int32  nBaseSP;
    int32  nBaseATK;
    int32  nBaseDEF;
    int32  nBaseMATK;
    int32  nBaseMDEF;
    int32  nBaseMoveSpeed;
    int32  nMaxStatTotal;       // hard cap on STR+END+DEX+INT+MEN sum at this level
};

struct LegacyMoverAbilityRow {
    uint8  bClass;
    int32  nHpPerEND;
    int32  nSpPerINT;
    int32  nAtkPerSTR;
    int32  nMatkPerINT;
    int32  nDefPerEND;
    int32  nHitPerDEX;
    int32  nCritPerDEX;
    int32  nDodgePerDEX;
    int32  nMaxHpFlat;
    int32  nMaxSpFlat;
};

class MoverMainTable {
public:
    static MoverMainTable& Get();
    bool                Load(const std::string& rRoot);
    const LegacyMoverMainRow* Find(uint8 bClass, uint16 uiLevel) const;
    void                FillRaw(RAWCHARSTAT* pOut, uint8 bClass, uint16 uiLevel) const;
private:
    MoverMainTable() {}
    std::map<uint32, LegacyMoverMainRow> m_kRows;       // key = (class<<16)|level
};

class MoverAbilityTable {
public:
    static MoverAbilityTable& Get();
    bool                   Load(const std::string& rRoot);
    const LegacyMoverAbilityRow* Find(uint8 bClass) const;
private:
    MoverAbilityTable() {}
    std::map<uint8, LegacyMoverAbilityRow> m_kRows;
};

} // namespace shine
#endif
