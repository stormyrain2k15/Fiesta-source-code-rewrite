// Server/DataReader/SHN/CollectCardStarRate.cpp
// Auto-generated: one-file-per-SHN split for CollectCardStarRate.shn
#include "CollectCardStarRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CollectCardStarRateShn& CollectCardStarRateShn::Get() { static CollectCardStarRateShn s; return s; }

void CollectCardStarRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardStarRate");
    if (!t) { SHINELOG_WARN("CollectCardStarRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CollectCardStarRateRow rec;
        rec.uiCC_CardGradeType = ShnGetU32(*t, _r, "CC_CardGradeType");
        rec.uiCC_StarLot = (uint8)ShnGetU32(*t, _r, "CC_StarLot");
        rec.uiCC_StarRate = (uint16)ShnGetU32(*t, _r, "CC_StarRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CollectCardStarRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
