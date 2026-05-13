// Server/DataReader/SHN/GBSMNPC.cpp
// Auto-generated: one-file-per-SHN split for GBSMNPC.shn
#include "GBSMNPC.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBSMNPCShn& GBSMNPCShn::Get() { static GBSMNPCShn s; return s; }

void GBSMNPCShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMNPC");
    if (!t) { SHINELOG_WARN("GBSMNPC.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMNPCRow rec;
        rec.kGBSM_MobInx = ShnGetStr(*t, _r, "GBSM_MobInx");
        rec.uiGBSMBet = ShnGetU32(*t, _r, "GBSMBet");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMNPC.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
