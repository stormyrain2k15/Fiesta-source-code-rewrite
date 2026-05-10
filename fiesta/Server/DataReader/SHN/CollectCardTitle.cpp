// Server/DataReader/SHN/CollectCardTitle.cpp
// Auto-generated: one-file-per-SHN split for CollectCardTitle.shn
#include "CollectCardTitle.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CollectCardTitleShn& CollectCardTitleShn::Get() { static CollectCardTitleShn s; return s; }

void CollectCardTitleShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardTitle");
    if (!t) { SHINELOG_WARN("CollectCardTitle.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CollectCardTitleRow rec;
        rec.uiType = ShnGetU32(*t, _r, "Type");
        rec.kCC_ItemInx = ShnGetStr(*t, _r, "CC_ItemInx");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CollectCardTitle.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
