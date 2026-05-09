// Server/Zone/SoulStoneSystem.h
// 14 -- Soul Stones are active-use combat consumables. They are NOT inventory
// items: they live on the character row as flat per-tier counters, and the
// client UI exposes them in its own dedicated bar with one counter per tier
// (HP100 / HP500 / HP1000 / HP3000 / ... / SP100 / SP500 / SP1000 / SP3000).
//
// On use:
//   * One stone of the chosen tier is decremented.
//   * A fixed flat HP (or SP) heal is applied immediately.
//   * The shared "soul stone cooldown" clock is set; another soul stone
//     cannot be used until the cooldown expires.
//
// Tunables (tiers, heal amounts, cooldown ms) live below as named const --
// edit in place to retune. None of these numbers leak into business logic
// anywhere else.
//
// ---------------------------------------------------------------------------
// Death revive is a separate mechanic and lives in DeathReviveSystem.h
// (Shine Soul Stones, town respawn, etc.). Do not conflate the two.
// ---------------------------------------------------------------------------
//
// EVIDENCE: PDB_CONFIRMED   symbols: cMoverHpRegen, cMoverSpRegen,
//                                    SoulStoneCount, SoulStoneUse_Req.
//                           DATA_CONFIRMED  DefaultCharacterData.txt has
//                                    initial counters per tier.
#ifndef FIESTA_ZONE_SOULSTONESYSTEM_H
#define FIESTA_ZONE_SOULSTONESYSTEM_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class ShinePlayer;

// =============================================================================
//  EDITABLE TUNABLES
// =============================================================================

// Number of tiers in each (HP / SP) ladder. Edit `kSoulStoneTierCount` plus
// the tables below to add or remove tiers.
const int    kSoulStoneTierCount     = 6;

// Heal amount per HP tier (flat HP restored per stone consumed).
const int32  kHpSoulHeal[kSoulStoneTierCount] = {
    100,    // tier 0  -- "HP 100"
    500,    // tier 1  -- "HP 500"
    1000,   // tier 2  -- "HP 1000"
    3000,   // tier 3  -- "HP 3000"
    8000,   // tier 4  -- "HP 8000"
    20000   // tier 5  -- "HP 20000" (cash-shop / endgame)
};
// Heal amount per SP tier.
const int32  kSpSoulHeal[kSoulStoneTierCount] = {
    100, 500, 1000, 3000, 8000, 20000
};

// Per-tier display names (sent in NC_MISC_SHN_VERIFY-equivalent flows /
// debug logs). Kept here so the UI mapping is in one place.
const char* const kHpSoulName[kSoulStoneTierCount] = {
    "HP100","HP500","HP1000","HP3000","HP8000","HP20000"
};
const char* const kSpSoulName[kSoulStoneTierCount] = {
    "SP100","SP500","SP1000","SP3000","SP8000","SP20000"
};

// Maximum number of stones the character may hold per tier.
const uint16 kSoulStoneMaxPerTier    = 9999;

// Shared cooldown (ms) gating the next soul-stone use. Real game value
// is ~1.5 sec. Setting to 0 disables the cooldown (PvE testing).
const uint32 kSoulStoneCooldownMs    = 1500;

// =============================================================================
//  RUNTIME STATE
// =============================================================================

// One ledger per character. Lives on the character DB row -- not in the
// inventory. Initial values come from DefaultCharacterData.txt.
struct SoulStoneCounts {
    uint16 aHpStones[kSoulStoneTierCount];
    uint16 aSpStones[kSoulStoneTierCount];
    uint64 uiNextUseTickMs;     // shared cooldown clock (uses GetTickCount64)

    SoulStoneCounts() : uiNextUseTickMs(0) {
        for (int i = 0; i < kSoulStoneTierCount; ++i) {
            aHpStones[i] = 0;
            aSpStones[i] = 0;
        }
    }
};

// Use-result reported back to the client.
enum eSoulStoneUseResult {
    SS_USE_OK             = 0,
    SS_USE_NONE_LEFT      = 1,
    SS_USE_ON_COOLDOWN    = 2,
    SS_USE_INVALID_TIER   = 3,
    SS_USE_AT_MAX_HP      = 4,    // already full -- some clients hide the bar instead
    SS_USE_AT_MAX_SP      = 5,
    SS_USE_DEAD           = 6
};

class SoulStoneSystem {
public:
    // Active in-combat use. eIsHp = true for HP soul stone, false for SP.
    static eSoulStoneUseResult Use(ShinePlayer*       pkP,
                                    SoulStoneCounts&  rC,
                                    bool              bIsHp,
                                    int               iTier);

    // Add stones (drop, quest reward, vendor purchase). Clamps to kSoulStoneMaxPerTier.
    static void Grant(SoulStoneCounts& rC, bool bIsHp, int iTier, uint16 uiQty);

    // Time until the next soul stone may be used (ms; 0 = ready now).
    static uint32 RemainingCooldownMs(const SoulStoneCounts& rC);

    // Helper for client UI / quest checks: total stones across all tiers.
    static uint32 TotalHp(const SoulStoneCounts& rC);
    static uint32 TotalSp(const SoulStoneCounts& rC);
};

} // namespace fiesta
#endif
