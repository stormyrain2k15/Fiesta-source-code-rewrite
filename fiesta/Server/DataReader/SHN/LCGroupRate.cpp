// Server/DataReader/SHN/LCGroupRate.cpp
// Auto-generated: one-file-per-SHN split for LCGroupRate.shn
#include "LCGroupRate.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

LCGroupRateShn& LCGroupRateShn::Get() { static LCGroupRateShn s; return s; }

void LCGroupRateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("LCGroupRate");
    if (!t) { SHINELOG_WARN("LCGroupRate.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        LCGroupRateRow rec;
        rec.uiItem_ID = ShnGetU32(*t, _r, "Item_ID");
        rec.uiLCR_Group = (uint8)ShnGetU32(*t, _r, "LCR_Group");
        rec.uiLCR_Rate = ShnGetU32(*t, _r, "LCR_Rate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("LCGroupRate.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
