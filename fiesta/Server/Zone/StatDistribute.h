// Server/Zone/StatDistribute.h
// input structures + BuildBattleStat for compiling a BATTLESTAT from
// raw character stats + equipment + active buffs.
#ifndef FIESTA_ZONE_STATDISTRIBUTE_H
#define FIESTA_ZONE_STATDISTRIBUTE_H
#include "BattleStat.h"

namespace fiesta {

// Base stats before any compilation. Sourced from character DB +
// MoverMain.shn lookup at login time.
struct RAWCHARSTAT {
    int32 nLevel;
    uint8 bClass;
    uint8 bJob;

    // Allocated stat points (raw player choices).
    int32 nSTR, nEND, nDEX, nINT, nMEN;

    // Base values from MoverMain.shn for this class / level.
    int32 nBaseHP;
    int32 nBaseSP;
    int32 nBaseATK;
    int32 nBaseDEF;
    int32 nBaseMATK;
    int32 nBaseMDEF;
    int32 nBaseMoveSpeed;

    void Clear() { memset(this, 0, sizeof(*this)); }
};

// Summed equipment bonuses across all equipped slots. Built by iterating
// inventory equipped slots against ItemInfo.shn and adding upgrade-level
// bonuses computed from BasicUpInx / AddUpInx.
struct EQUIPSUMMARY {
    int32 nATK, nMATK, nRATK;
    int32 nDEF, nMDEF, nRDEF;
    int32 nHP,  nSP;
    int32 nCritical, nDodge, nBlock, nAccuracy;
    int32 nMoveSpeed;
    int32 nEleATK   [ELEMENT_MAX];
    int32 nEleResist[ELEMENT_MAX];
    // Upgrade-derived absolutes (UpLimit-tier bonus -- summed across slots).
    int32 nUpgradeATK;
    int32 nUpgradeDEF;

    void Clear() { memset(this, 0, sizeof(*this)); }
};

// Volatile modifiers from active abnormal states.
struct BUFFMODIFIERS {
    int32 nATK, nMATK, nRATK;
    int32 nDEF, nMDEF;
    int32 nMaxHP, nMaxSP;
    int32 nHPRegen, nSPRegen;
    int32 nMoveSpeed;
    int32 nCritical, nDodge, nAccuracy;
    int32 nEleResist[ELEMENT_MAX];
    int32 nEleATK   [ELEMENT_MAX];
    int32 nAbsorbDmg;
    int32 nAbstateResist;

    void Clear() { memset(this, 0, sizeof(*this)); }
};

// Composes everything into a final BATTLESTAT. pEquip / pBuff may be NULL.
void BuildBattleStat(BATTLESTAT*           pOut,
                     const RAWCHARSTAT*    pRaw,
                     const EQUIPSUMMARY*   pEquip,
                     const BUFFMODIFIERS*  pBuff);

class ShinePlayer;

// Builds a RAWCHARSTAT from the player's persistent state. The class
// progression baseline (`nBaseHP/SP/ATK/DEF/MATK/MDEF/MoveSpeed`) is taken
// from `ClassParamTable` (per-class growth file) -- with a fallback to
// `MoverMainTable` when ClassParamTable hasn't been loaded for the class.
// The allocated portion (`nSTR/nEND/nDEX/nINT/nMEN`) comes from the
// player's free-stat ledger.
void FillRawCharStatFromPlayer(RAWCHARSTAT& rOut, const ShinePlayer& kP);

} // namespace fiesta
#endif

