// Server/DataReader/SHN/KQItem.cpp
// Auto-generated: one-file-per-SHN split for KQItem.shn
#include "KQItem.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

KQItemShn& KQItemShn::Get() { static KQItemShn s; return s; }

void KQItemShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("KQItem");
    if (!t) { SHINELOG_WARN("KQItem.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        KQItemRow rec;
        rec.kItemIndex = ShnGetStr(*t, _r, "ItemIndex");
        rec.uiMoveSpdRate = (uint16)ShnGetU32(*t, _r, "MoveSpdRate");
        rec.uiAbsoluteAttack = (uint16)ShnGetU32(*t, _r, "AbsoluteAttack");
        rec.uiPickupLimit = (uint16)ShnGetU32(*t, _r, "PickupLimit");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("KQItem.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
