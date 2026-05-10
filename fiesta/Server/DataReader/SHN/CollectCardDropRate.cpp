// Server/DataReader/SHN/CollectCardDropRate.cpp
// Auto-generated: one-file-per-SHN split for CollectCardDropRate.shn
#include "CollectCardDropRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CollectCardDropRateShn& CollectCardDropRateShn::Get() { static CollectCardDropRateShn s; return s; }

void CollectCardDropRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardDropRate");
    if (!t) { SHINELOG_WARN("CollectCardDropRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CollectCardDropRateRow rec;
        rec.uiCC_CardID = (uint16)ShnGetU32(*t, _r, "CC_CardID");
        rec.uiCC_CardGetRate = (uint16)ShnGetU32(*t, _r, "CC_CardGetRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CollectCardDropRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
