// Server/Zone/SoulStoneSystem.h
// 14 -- HP and SP soul stones.
//
// Model (from project owner's clarification):
//
//   * Two flat counters per character row: `nHpSoulCount` and `nSpSoulCount`.
//     NOT inventory items, NOT per-tier counters -- just two integers.
//
//   * Tier is derived automatically from the character level via
//     `kSoulStoneTierByLevel[]`. The player never sees tier; the UI just
//     shows "HP Soul Stones: N" and "SP Soul Stones: N" with a Use button.
//
//   * HP and SP have INDEPENDENT cooldown clocks. Using an HP soul does
//     not start the SP cooldown and vice versa. Cooldown is `kSoulStoneCooldownMs`
//     (same duration as the matching potion tier on live -- see SkillTimers).
//
//   * Vendors: NPCs flagged `Role=Merchant RoleArg0=SoulStone` in
//     World/NPC.txt sell soul stones. The shop UI shows two SKUs ("HP Soul
//     Stone" and "SP Soul Stone") at a level-appropriate price (`kSoulStonePrice`).
//     Every confirmed vendor in the supplied World/NPC.txt:
//        RouSoulMctJulia, EldSoulMctAvon, UruSoulPooring, HednisSoulKeroll,
//        StoneMctTomson, SoulMctChloe, AlruinSoulRunadilla, BeraSoulOlivia,
//        TempSoul, ... .
//
// EVIDENCE: PDB_CONFIRMED   symbols: cMoverHpRegen, cMoverSpRegen,
//                                    SoulStoneCount, SoulStoneUse_Req.
//                           DATA_CONFIRMED  World/NPC.txt SoulStone vendors.
#ifndef FIESTA_ZONE_SOULSTONESYSTEM_H
#define FIESTA_ZONE_SOULSTONESYSTEM_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class ShinePlayer;

// =============================================================================
//  EDITABLE TUNABLES
// =============================================================================

// Number of tiers in the ladder. Used by the level->tier and the heal tables.
const int    kSoulStoneTierCount     = 6;

// Heal amounts per tier (flat HP/SP restored per stone consumed).
// Tiers 0..5 align with potion tiers 1..6 on live.
const int32  kHpSoulHealByTier[kSoulStoneTierCount] = {
    100, 500, 1000, 3000, 8000, 20000
};
const int32  kSpSoulHealByTier[kSoulStoneTierCount] = {
    100, 500, 1000, 3000, 8000, 20000
};

// Tier selected automatically by character level. Index = level (clamped).
// Live game thresholds: T1 1-19, T2 20-39, T3 40-59, T4 60-89, T5 90-119, T6 120+.
const int    kMaxLevelForTierTable   = 200;
const uint8  kSoulStoneTierByLevel[kMaxLevelForTierTable] = {
    /*   1- 19 */ 0,0,0,0,0,0,0,0,0,0, 0,0,0,0,0,0,0,0,0,0,
    /*  20- 39 */ 1,1,1,1,1,1,1,1,1,1, 1,1,1,1,1,1,1,1,1,1,
    /*  40- 59 */ 2,2,2,2,2,2,2,2,2,2, 2,2,2,2,2,2,2,2,2,2,
    /*  60- 89 */ 3,3,3,3,3,3,3,3,3,3, 3,3,3,3,3,3,3,3,3,3,
                  3,3,3,3,3,3,3,3,3,3,
    /*  90-119 */ 4,4,4,4,4,4,4,4,4,4, 4,4,4,4,4,4,4,4,4,4,
                  4,4,4,4,4,4,4,4,4,4,
    /* 120-200 */ 5,5,5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,5,5,
                  5,5,5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,5,5,
                  5,5,5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,5,5,
                  5,5,5,5,5,5,5,5,5,5, 5,5,5,5,5,5,5,5,5,5,5
};

// Maximum count the character can hold (single counter, not per-tier).
const uint16 kSoulStoneMaxCount      = 9999;

// Cooldown per stone use, ms (HP and SP independent). Live ~1.5s.
const uint32 kSoulStoneCooldownMs    = 1500;

// Vendor sell price per stone (Vis), keyed by tier.
const int32  kSoulStonePriceByTier[kSoulStoneTierCount] = {
    50, 200, 800, 3000, 10000, 40000
};

// =============================================================================
//  RUNTIME STATE  (lives on the character DB row, NOT inventory)
// =============================================================================

struct SoulStoneCounts {
    uint16 uiHpCount;
    uint16 uiSpCount;
    uint64 uiNextHpUseTickMs;       // independent HP cooldown
    uint64 uiNextSpUseTickMs;       // independent SP cooldown

    SoulStoneCounts()
        : uiHpCount(0), uiSpCount(0), uiNextHpUseTickMs(0), uiNextSpUseTickMs(0) {}
};

enum eSoulStoneUseResult {
    SS_USE_OK             = 0,
    SS_USE_NONE_LEFT      = 1,
    SS_USE_ON_COOLDOWN    = 2,
    SS_USE_AT_MAX_HP      = 3,
    SS_USE_AT_MAX_SP      = 4,
    SS_USE_DEAD           = 5
};

class SoulStoneSystem {
public:
    // Active in-combat use. bIsHp=true selects HP soul, false selects SP.
    static eSoulStoneUseResult Use(ShinePlayer*      pkP,
                                    SoulStoneCounts& rC,
                                    bool             bIsHp);

    // Add stones (drop, quest reward, vendor purchase). Clamps to kSoulStoneMaxCount.
    static void Grant(SoulStoneCounts& rC, bool bIsHp, uint16 uiQty);

    // Time until the next HP / SP soul stone may be used (ms; 0 = ready).
    static uint32 RemainingHpCooldownMs(const SoulStoneCounts& rC);
    static uint32 RemainingSpCooldownMs(const SoulStoneCounts& rC);

    // Tier resolution helpers (UI / vendor / log).
    static uint8 TierForLevel(int32 nLevel);
    static int32 HealForLevel(int32 nLevel, bool bIsHp);
    static int32 PriceForLevel(int32 nLevel);
};

} // namespace fiesta
#endif
