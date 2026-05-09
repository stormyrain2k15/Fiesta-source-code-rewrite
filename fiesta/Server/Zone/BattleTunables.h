// Server/Zone/BattleTunables.h
// Single home for every "magic number" in the damage pipeline. Edit
// these to retune combat without touching business logic anywhere else.
// All values are they are reasonable starting points for the
// 145-cap live-game balance shape. Verify against community theorycraft
// before treating any of them as ground truth.
#ifndef FIESTA_ZONE_BATTLETUNABLES_H
#define FIESTA_ZONE_BATTLETUNABLES_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

// =============================================================================
//  Damage formula constants
// =============================================================================

// Minimum final damage for a non-dodged hit. Cannot drop below this.
const int32  kDamageFloor              = 1;

// Maximum effective level gap (used as a clamp for level-gap table lookups
// when MoverMain.shn data is unavailable).
const int32  kMaxLevelGap              = 100;

// Critical roll resolution. Crit chance is stored x100 (5000 = 50.00%).
const int32  kCritRollMax              = 10000;
// Crit damage bonus is stored x10000 (5000 = +50% damage).
const int32  kCritDamageScaler         = 10000;

// Random damage variance (+/- this fraction of raw, x10000). 0 = deterministic.
const int32  kDamageVarianceX10k       = 800;     // +/- 8%

// Angle bonus multipliers (x1000). Applied AFTER level-gap, BEFORE crit.
const int32  kBackHitMultiplierX1k     = 1200;    // 1.20x for >90deg
const int32  kFlankHitMultiplierX1k    = 1100;    // 1.10x for 45..90deg

// PvP global damage scaler (x1000). 1000 = no change.
const int32  kPvPDamageScalerX1k       = 700;     // 0.70x in PvP by default

// =============================================================================
//  Stat distribution constants (BuildBattleStat)
// =============================================================================

// HP / SP per stat point allocated. Real values are class-dependent --
// these are fallbacks if MoverAbility.shn isn't loaded.
const int32  kHpPerEND                 = 14;
const int32  kSpPerINT                 = 12;
const int32  kAtkPerSTR                = 2;
const int32  kMatkPerINT               = 2;
const int32  kDefPerEND                = 1;
const int32  kHitPerDEX                = 5;       // x100 = %
const int32  kCritPerDEX               = 3;       // x100 = %
const int32  kDodgePerDEX              = 4;       // x100 = %

// =============================================================================
//  Item upgrade (enhancement) constants
// =============================================================================

// Multiplier on UpSucRatio when the upgrade resource is consumed. Values
// from ItemInfo.shn are stored per-mille (1000 = 100.00%); this scaler
// can throttle the global success rate down (e.g. 1000 = stock).
const int32  kUpgradeSucScalerX1k      = 1000;

// Maximum upgrade level a single item may reach (cap; ItemInfo.shn UpLimit
// still applies on top).
const uint8  kMaxUpgradeLevel          = 20;

// Failure consequences:
//   0 = no penalty (Tier 0..3 stones)
//   1 = downgrade by 1
//   2 = destroy
const uint8  kFailPenaltyAtLevel[20]   = {
    0, 0, 0, 0, 0, 1, 1, 1, 1, 1,
    1, 2, 2, 2, 2, 2, 2, 2, 2, 2
};

// =============================================================================
//  Free-stat allocation
// =============================================================================

const int32  kFreeStatPointsPerLevel   = 1;   // points granted per level
const int32  kMaxFreeStatPerStat       = 999; // soft cap shown to client

// =============================================================================
//  Drop / loot constants
// =============================================================================

// Global drop rate scaler (x1000). 1000 = stock SHN values.
const int32  kDropRateGlobalScalerX1k  = 1000;

// =============================================================================
//  Regen / vitals
// =============================================================================

const int32  kHpRegenTickMs            = 5000;    // 5 sec
const int32  kSpRegenTickMs            = 5000;
const int32  kHpRegenIdlePctX10k       = 200;     // 2% of MaxHP per tick when idle
const int32  kSpRegenIdlePctX10k       = 250;     // 2.5%

// =============================================================================
//  Roe_* free-function pipeline (PineScript / scripted attack callers)
//  These are the constants used by RuleOfEngagement.cpp's `Roe_getattack`,
//  `Roe_defendpower`, `Roe_hitrate`, `Roe_criticalrate`, and
//  `Roe_isdamageincrease`. They DO NOT replace the `RuleOfEngagement::
//  CalcDamage` pipeline above -- that continues to be the canonical
//  combat path. The Roe_* helpers exist for scripted callers that want
//  a plain int32 answer and aren't routed through the BATTLESTAT
//  pipeline. Adjust here to retune scripted formulas independently.
// =============================================================================

// Random ATK swing range (x1k). Final ATK = nATK + rand[0..nATK*kRoeRangeX1k/1000].
// 0 = no swing. 200 = up to +20% above the base figure.
const int32  kRoeAtkRangeX1k           = 200;

// Element resist scaler (x1000). Applied as a flat damage reduction
// when the attacker has an element column matching the target's resist.
const int32  kRoeElementResistX1k      = 700;     // 0.70x against matched resist

// Hit-rate baseline and slope. Hit% = clamp(base + (Hit - Avoid) * slope/1000,
// floor, cap). Hit / Avoid come from BATTLESTAT (Accuracy / Dodge).
const int32  kRoeHitBasePct            = 90;
const int32  kRoeHitSlopeX1k           = 500;    // 0.5 % per net point
const int32  kRoeHitFloorPct           = 5;
const int32  kRoeHitCapPct             = 99;

// Critical baseline and slope. Crit% = clamp(base + Crit/slope - TgtDodge/resist,
// 0, cap).
const int32  kRoeCritBasePct           = 5;
const int32  kRoeCritDivisor           = 100;    // points-per-percent
const int32  kRoeCritResistDivisor     = 200;
const int32  kRoeCritCapPct            = 50;

// =============================================================================
//  GMEvent ids (LiveOpsBoosts <-> GMEvent.shn)
// =============================================================================
//
// GENERIC: real GMEvent.shn EventNo numbers aren't catalogued in this
// tree; these are the ids the WM emits internally. Edit to match the
// per-server data drop without touching consumers.
const uint32 kGMEvent_LuckyHour        = 1001;
const uint32 kGMEvent_DoubleExp        = 1002;
const uint32 kGMEvent_DoubleDrop       = 1003;
const uint32 kGMEvent_GoldenHour       = 1004;

// =============================================================================
//  KingdomQuest reward curve
// =============================================================================
//
// Gold reward = base + perPoint * contribution, capped at maxGold.
const int32  kKQRewardBaseGold         = 5000;
const int32  kKQRewardGoldPerPoint     = 25;
const int32  kKQRewardMaxGold          = 5000000;

} // namespace fiesta
#endif
