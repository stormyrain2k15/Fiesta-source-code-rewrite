// Server/DataReader/SHN/GBSMCenter.cpp
// Auto-generated: one-file-per-SHN split for GBSMCenter.shn
#include "GBSMCenter.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBSMCenterShn& GBSMCenterShn::Get() { static GBSMCenterShn s; return s; }

void GBSMCenterShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMCenter");
    if (!t) { SHINELOG_WARN("GBSMCenter.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMCenterRow rec;
        rec.uiGBSM_GroupID = (uint8)ShnGetU32(*t, _r, "GBSM_GroupID");
        rec.uiGBSM_RatioCenter = (uint16)ShnGetU32(*t, _r, "GBSM_RatioCenter");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMCenter.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
