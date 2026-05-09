// Server/Zone/SetItemData.cpp
// Set-bonus table. Sourced from SetItem.shn + SetEffect.shn.
// Applied in ShineItemEquip.cpp / ShineItemUnEquip.cpp when a set-indexed
// item is equipped or removed.
// WIRE-03 (Lyra, May 2026): lifted out of anonymous namespace.
#include "SetItemData.h"
#include "../DataReader/ShnRegistry.h"
#include "../DataReader/ShnFile.h"
#include <string>

namespace fiesta {

SetItemData& SetItemData::Get() { static SetItemData s; return s; }

bool SetItemData::Load() {
    return ShnRegistry::Get().GetTable("SetItem")    != NULL
        && ShnRegistry::Get().GetTable("SetEffect")  != NULL;
}

// Count how many pieces of szSetIndex the player has equipped.
// Each item in ItemInfo has a SetItemIndex field; we walk the
// player's equipment slots and count matches.
// NOTE: ShinePlayer equip-slot iteration is wired by ShineItemEquip.
int SetItemData::CountEquippedPieces(ShinePlayer* pkPlayer,
                                     const char* szSetIndex) const {
    if (!pkPlayer || !szSetIndex || szSetIndex[0] == '\0') return 0;
    int n = 0;
    const ShnFile* pItems = ShnRegistry::Get().GetTable("ItemInfo");
    if (!pItems) return 0;
    // Walk each equipment slot on the player and look up SetItemIndex.
    // TODO: ShinePlayer::EquipSlotIterator lands with unified inventory wiring.
    // For now return 0 so no set bonus fires until wiring is complete.
    (void)n; (void)pItems;
    return 0;
}

} // namespace fiesta
