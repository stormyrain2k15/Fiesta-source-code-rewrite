// Server/Zone/ChrCommonTable.h
// 09 -- World/ChrCommon.txt: 7 sub-tables.
//
//   Common              -- key/value map (RunSpeed, WalkSpeed, AttackSpeed,
//                          ShoutLevel, ShoutDelay, LevelLimit, MaxExpBonus,
//                          RestExpRate, MinNeedTime, ..., MaxFreeStat).
//   StatTable           -- 150 rows; per-level NextExp + per-class
//                          stat-checksum row (one column per class).
//   FreeStatStrTable    -- 181 rows; per-allocated-STR -> WC delta.
//   FreeStatIntTable    -- 181 rows; per-INT -> MA / MaxSP.
//   FreeStatDexTable    -- 181 rows; per-DEX -> ToHit / ToBlock / ATKSpd.
//   FreeStatConTable    -- 181 rows; per-CON -> AC / MaxHP.
//   FreeStatMenTable    -- 181 rows; per-MEN -> MR / status resists.
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied
//                                   World/ChrCommon.txt.
#ifndef FIESTA_ZONE_CHRCOMMONTABLE_H
#define FIESTA_ZONE_CHRCOMMONTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

class ChrCommonTable {
public:
    static ChrCommonTable& Get();
    bool Load(const std::string& rRoot);

    // ----- Common (key/value) -----
    int32  GetCommon(const char* szKey, int32 iDefault = 0) const;

    // ----- StatTable: per-level row -----
    struct StatRow {
        uint16 uiLevel;
        uint32 uiNextExp;
        uint32 aClassValue[32];   // index = eShineClass; per-class checksum / max-stat
        uint32 uiExpLostAtPvP;
    };
    const StatRow* StatRowByLevel(uint16 uiLevel) const;

    // ----- Free-stat tables -----
    enum eStat { STAT_STR = 0, STAT_INT = 1, STAT_DEX = 2, STAT_CON = 3, STAT_MEN = 4 };
    struct FreeStatRow {
        uint16 uiPoints;            // Stat value (0..180)
        std::vector<int32> kCols;   // raw column values (varies per stat: WC, MA, AC, etc.)
    };
    const FreeStatRow* FreeStatRowByPoints(eStat eS, uint16 uiPoints) const;

    // Convenience accessors -- read the most-common derived columns.
    int32 WCPerStr(uint16 uiPoints) const;     // FreeStatStrTable.WC
    int32 MAPerInt(uint16 uiPoints) const;     // FreeStatIntTable.MA
    int32 SPPerInt(uint16 uiPoints) const;     // FreeStatIntTable.MaxSP
    int32 HitPerDex(uint16 uiPoints) const;    // FreeStatDexTable.ToHit
    int32 BlockPerDex(uint16 uiPoints) const;  // FreeStatDexTable.ToBlock
    int32 ACPerCon(uint16 uiPoints) const;     // FreeStatConTable.AC
    int32 HPPerCon(uint16 uiPoints) const;     // FreeStatConTable.MaxHP
    int32 MRPerMen(uint16 uiPoints) const;     // FreeStatMenTable.MR

private:
    ChrCommonTable() {}
    bool LoadFreeStat(class TableScriptFile& f, const char* szTable, eStat eS);

    std::map<std::string, int32>      m_kCommon;
    std::map<uint16, StatRow>         m_kStat;
    std::map<uint16, FreeStatRow>     m_kFree[5];   // indexed by eStat
    std::vector<std::string>          m_kFreeColNames[5];  // header names
};

} // namespace fiesta
#endif
