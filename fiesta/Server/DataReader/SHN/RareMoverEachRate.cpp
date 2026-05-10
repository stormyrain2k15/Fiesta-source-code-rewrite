// Server/DataReader/SHN/RareMoverEachRate.cpp
// Auto-generated: one-file-per-SHN split for RareMoverEachRate.shn
#include "RareMoverEachRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

RareMoverEachRateShn& RareMoverEachRateShn::Get() { static RareMoverEachRateShn s; return s; }

void RareMoverEachRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("RareMoverEachRate");
    if (!t) { SHINELOG_WARN("RareMoverEachRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        RareMoverEachRateRow rec;
        rec.kRMER_ItemIDX = ShnGetStr(*t, _r, "RMER_ItemIDX");
        rec.uiRMER_Rate = (uint16)ShnGetU32(*t, _r, "RMER_Rate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("RareMoverEachRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
