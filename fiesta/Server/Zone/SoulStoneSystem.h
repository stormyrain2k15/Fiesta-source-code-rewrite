// Server/Zone/SoulStoneSystem.h
// 14 -- soul-stone consumption on respawn (HP / SP / Shine soul stones).
//
// On player death:
//   1. The death packet pauses combat and shows the resurrect dialog.
//   2. If the player chooses "use HP/SP soul stone" -- consume one of the
//      tracked counts and restore HP%/SP% per the chosen tier (lo/hi).
//   3. Otherwise the player is town-warped with HP/SP regen ticks.
//
// Counts live on the character DB row (DefaultCharacterData.txt initial
// values + lifetime increments). This system is purely server-side; the
// client just sends a "use soul stone" intent.
//
// EVIDENCE: PDB_CONFIRMED   symbols: cMoverHpRegen, cMoverSpRegen,
//                                    cShineSoulRegen.
//                           DATA_CONFIRMED  DefaultCharacterData.txt.
#ifndef FIESTA_ZONE_SOULSTONESYSTEM_H
#define FIESTA_ZONE_SOULSTONESYSTEM_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

class ShinePlayer;

// Quantities of each kind of soul stone the character holds. Mirrors the
// columns in DefaultCharacterData.txt.
struct SoulStoneCounts {
    uint16 uiHpLo;
    uint16 uiHpHi;
    uint16 uiSpLo;
    uint16 uiSpHi;
    uint16 uiShineSoul;        // free-revive token (KQ-only on live)
};

enum eSoulRevive {
    SOUL_REVIVE_NONE       = 0,
    SOUL_REVIVE_HP_LO      = 1,    // restore 30% HP, 0% SP
    SOUL_REVIVE_HP_HI      = 2,    // restore 60% HP, 0% SP
    SOUL_REVIVE_SP_LO      = 3,    // restore 0% HP,  30% SP
    SOUL_REVIVE_SP_HI      = 4,    // restore 0% HP,  60% SP
    SOUL_REVIVE_SHINE      = 5,    // restore 100/100, no XP loss
    SOUL_REVIVE_TOWN       = 6     // free, town respawn, XP penalty
};

class SoulStoneSystem {
public:
    // Try to revive `pkP` using the given `eType`. Returns true on success.
    // On success, decrements the matching SoulStoneCounts entry, restores
    // HP/SP, and clears the dead flag. Town respawn always succeeds.
    static bool TryRevive(ShinePlayer* pkP, SoulStoneCounts& rCounts, eSoulRevive eType);

    // Helpers for HP/SP percent restoration (used by skills + GM commands too).
    static void  RestoreFraction(ShinePlayer* pkP, int32 nHpPctX100, int32 nSpPctX100);

    // XP penalty constants (editable via BattleTunables.h's regen group if
    // needed; kept inline because they belong with the death flow).
    static int32 ExpLossOnTownRevive(int32 nLevel);
};

} // namespace fiesta
#endif
