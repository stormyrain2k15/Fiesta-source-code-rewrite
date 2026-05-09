// Server/Zone/SetItemData.cpp
// Set-bonus table. Sourced from SetItem.shn + SetEffect.shn.
// Applied in ShineItemEquip.cpp / ShineItemUnEquip.cpp when a set-indexed
// item is equipped or removed.
// WIRE-03 (Lyra, May 2026): lifted out of anonymous namespace.
// Pass 6 (Feb 2026): real piece-count walk against the player's
// `Inventory` view (bEquipped == 1 entries).
#include "SetItemData.h"
#include "ShineObject.h"
#include "GroupTables.h"
#include "../DataReader/ShnRegistry.h"
#include "../DataReader/ShnFile.h"
#include <string>

namespace fiesta {

SetItemData& SetItemData::Get() { static SetItemData s; return s; }

bool SetItemData::Load() {
    return ShnRegistry::Get().GetTable("SetItem")    != NULL
        && ShnRegistry::Get().GetTable("SetEffect")  != NULL;
}

// Count how many pieces of szSetIndex the player has equipped. Walks
// the per-player `Inventory` (bEquipped == 1) and matches each item's
// resolved `ItemInfoRow.kSetItemIndex` against the requested set key.
int SetItemData::CountEquippedPieces(ShinePlayer* pkPlayer,
                                     const char* szSetIndex) const {
    if (!pkPlayer || !szSetIndex || szSetIndex[0] == '\0') return 0;
    int n = 0;
    const std::vector<ShineItem>& vAll = pkPlayer->Inv().All();
    for (size_t i = 0; i < vAll.size(); ++i) {
        if (!vAll[i].bEquipped) continue;
        const ItemInfoRow* p = ItemTables::Get().FindItem((uint32)vAll[i].uiInxName);
        if (p && !p->kSetItemIndex.empty() && p->kSetItemIndex == szSetIndex)
            ++n;
    }
    return n;
}

} // namespace fiesta
