// Server/DataReader/SHN/GBSMLine.cpp
// Auto-generated: one-file-per-SHN split for GBSMLine.shn
#include "GBSMLine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBSMLineShn& GBSMLineShn::Get() { static GBSMLineShn s; return s; }

void GBSMLineShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMLine");
    if (!t) { SHINELOG_WARN("GBSMLine.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMLineRow rec;
        rec.uiGBSM_GroupID = (uint8)ShnGetU32(*t, _r, "GBSM_GroupID");
        rec.uiGBSM_Num = (uint8)ShnGetU32(*t, _r, "GBSM_Num");
        rec.uiGBSM_RatioLine = (uint16)ShnGetU32(*t, _r, "GBSM_RatioLine");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMLine.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
