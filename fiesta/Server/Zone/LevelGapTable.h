// Server/Zone/LevelGapTable.h
// FEATURE: lvgap-damage-curve
//
// Loads the three level-gap damage modifier tables from the NA2016
// SHN drop and answers atk/def-level lookups during damage resolution
// (Battle.cpp step 3).
//
//   DamageLvGapPVE.shn   2 cols  -- (LvGap, DamageRate). 1-D curve.
//                                   Player -> mob multiplier per gap.
//   DamageLvGapEVP.shn   2 cols  -- same shape. Mob -> player.
//   DamageLvGapPVP.shn   151 cols -- (MyLv, TargetLv1, .. TargetLv150).
//                                   2-D matrix; player vs player has its
//                                   own per-(myLv, targetLv) multiplier.
//
// All multipliers are stored per-1000 (1000 = 1.0x = no change).
#ifndef SHINE_ZONE_LEVELGAPTABLE_H
#define SHINE_ZONE_LEVELGAPTABLE_H
#include "BattleStat.h"
#include "BattleTunables.h"

namespace shine {

// 1-D curve range. PvE/EvP gaps clamp into this window.
const int kLvGapTableSize = 200;       // gap range -100..+99
const int kLvGapOffset    = 100;

// 2-D PvP matrix dimensions. NA2016 ships 151 levels (0..150) on both
// axes, matching the cap. Levels outside the matrix clamp to the edge.
const int kPvPMatrixSize  = 151;       // index 0..150 inclusive

class LevelGapTable {
public:
    static LevelGapTable& Get();

    // rRoot is the Data\Shine path. Returns true if every shipped table
    // parsed cleanly; partial failure logs a WARN per missing file but
    // leaves the unread slots at neutral 1.0x so combat keeps running.
    bool   Load(const std::string& rRoot);

    // Damage multiplier per 1000. 1000 = 1.0x. The PvP variant uses the
    // 2-D matrix; PvE/EvP use the 1-D curve keyed on (atk-def).
    int32  GetMultiplier(int32 nAtkLevel, int32 nDefLevel, eLvGapType eType) const;

private:
    LevelGapTable();
    bool   LoadCurve1D(const std::string& rPath, int32* pTable);
    bool   LoadMatrix2D(const std::string& rPath);

    int32 m_aPvE[kLvGapTableSize];
    int32 m_aEvP[kLvGapTableSize];
    int32 m_aPvP[kPvPMatrixSize][kPvPMatrixSize];
    bool  m_bLoaded;
};

} // namespace shine
#endif
