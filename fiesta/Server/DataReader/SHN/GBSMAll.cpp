// Server/DataReader/SHN/GBSMAll.cpp
// Auto-generated: one-file-per-SHN split for GBSMAll.shn
#include "GBSMAll.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBSMAllShn& GBSMAllShn::Get() { static GBSMAllShn s; return s; }

void GBSMAllShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMAll");
    if (!t) { SHINELOG_WARN("GBSMAll.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMAllRow rec;
        rec.uiGBSM_GroupID = (uint8)ShnGetU32(*t, _r, "GBSM_GroupID");
        rec.uiGBSM_Num = (uint8)ShnGetU32(*t, _r, "GBSM_Num");
        rec.uiGBSM_RatioAll = (uint16)ShnGetU32(*t, _r, "GBSM_RatioAll");
        rec.uiGBSM_IsJP = (uint8)ShnGetU32(*t, _r, "GBSM_IsJP");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMAll.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
