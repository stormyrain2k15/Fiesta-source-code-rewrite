// Server/DataReader/SHN/GBSMGroup.cpp
// Auto-generated: one-file-per-SHN split for GBSMGroup.shn
#include "GBSMGroup.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBSMGroupShn& GBSMGroupShn::Get() { static GBSMGroupShn s; return s; }

void GBSMGroupShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMGroup");
    if (!t) { SHINELOG_WARN("GBSMGroup.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMGroupRow rec;
        rec.uiGBSM_GroupID = (uint8)ShnGetU32(*t, _r, "GBSM_GroupID");
        rec.uiGBSMCard = ShnGetU32(*t, _r, "GBSMCard");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMGroup.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
