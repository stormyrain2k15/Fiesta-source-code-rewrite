// Server/Zone/BattleStat.h
// 12 -- compiled per-object stat block.
// Built once on zone login then kept hot in memory; rebuilt on equip/unequip,
// level up, buff apply/remove. Field names mirror the supplied
// MoverMain.shn / MoverAbility.shn / ItemInfo.shn / AbStateInfo.shn columns.
//
// EVIDENCE: PDB_CONFIRMED + DATA_CONFIRMED (from F2/Battle/battlestat.h shape).
#ifndef FIESTA_ZONE_BATTLESTAT_H
#define FIESTA_ZONE_BATTLESTAT_H
#include "../Shared/ShineTypes.h"
#include <string.h>

namespace fiesta {

// Element type. Matches EleType column in WeaponAttrib.shn / AbStateInfo.shn.
enum eElement {
    ELEMENT_NONE  = 0,
    ELEMENT_FIRE  = 1,
    ELEMENT_WATER = 2,
    ELEMENT_EARTH = 3,
    ELEMENT_WIND  = 4,
    ELEMENT_LIGHT = 5,
    ELEMENT_DARK  = 6,
    ELEMENT_MAX   = 7
};

// Damage type bitflags.
enum eDamageType {
    DMG_TYPE_NORMAL   = 0x0000,
    DMG_TYPE_MAGIC    = 0x0001,
    DMG_TYPE_SKILL    = 0x0002,
    DMG_TYPE_CRITICAL = 0x0004,
    DMG_TYPE_RANGED   = 0x0008,
    DMG_TYPE_REFLECT  = 0x0010,
    DMG_TYPE_TRUE     = 0x0020   // ignore DEF
};

// Level-gap table type.
enum eLvGapType {
    LVGAP_PVE = 0,    // player attacking mob
    LVGAP_PVP = 1,    // player attacking player
    LVGAP_EVP = 2,    // mob attacking player
    LVGAP_MAX = 3
};

// =============================================================================
// BATTLESTAT -- post-formula, ready for the damage calculation.
// =============================================================================
struct BATTLESTAT {
    // Core offensive
    int32 nATK;
    int32 nMATK;
    int32 nRATK;
    int32 nATKSpeed;
    int32 nCritical;            // x100 = %
    int32 nCriticalDmg;         // x100 = % bonus
    int32 nPenetrate;

    // Core defensive
    int32 nDEF;
    int32 nMDEF;
    int32 nRDEF;
    int32 nDodge;               // x100 = %
    int32 nBlock;               // x100 = %
    int32 nBlockDef;            // x100 = % reduction on block
    int32 nAbsorbDmg;

    // Vitals
    int32 nMaxHP;
    int32 nMaxSP;
    int32 nHPRegen;
    int32 nSPRegen;

    // Movement
    int32 nMoveSpeed;
    int32 nMoveSpeedPct;

    // Elemental
    int32 nEleResist[ELEMENT_MAX];
    int32 nEleATK   [ELEMENT_MAX];

    // Level / class
    int32 nLevel;
    uint8 bClass;               // basic class (Fighter/Cleric/Archer/Mage/Trickster)
    uint8 bJob;                 // job tier (0=base 1=1st 2=2nd 3=3rd)

    // Allocated stat points (free-stat)
    int32 nSTR, nEND, nDEX, nINT, nMEN;

    // PvP modifiers
    int32 nPvPATKBonus;         // x100 = %
    int32 nPvPDEFBonus;

    // Misc
    int32 nAccuracy;            // hit-rate modifier
    int32 nLuck;                // affects drop variance
    int32 nAbstateResist;       // x100 = % vs all abstates

    void Clear() { memset(this, 0, sizeof(*this)); }
};

// =============================================================================
// DAMAGERESULT -- output of CalcDamage.
// =============================================================================
struct DAMAGERESULT {
    int32   nDamage;
    int32   nDamageType;        // eDamageType flags
    bool    bCritical;
    bool    bDodged;
    bool    bBlocked;
    bool    bMissed;
    int32   nAbsorbed;
    eElement eElement;

    void Clear() { memset(this, 0, sizeof(*this)); }
};

} // namespace fiesta
#endif
