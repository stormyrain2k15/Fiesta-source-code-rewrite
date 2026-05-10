// Server/DataReader/SHN/PupMind.cpp
// Auto-generated: one-file-per-SHN split for PupMind.shn
#include "PupMind.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PupMindShn& PupMindShn::Get() { static PupMindShn s; return s; }

void PupMindShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PupMind");
    if (!t) { SHINELOG_WARN("PupMind.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PupMindRow rec;
        rec.uiPupMindType = ShnGetU32(*t, _r, "PupMindType");
        rec.uiMinMind = (uint8)ShnGetU32(*t, _r, "MinMind");
        rec.uiMaxMind = (uint8)ShnGetU32(*t, _r, "MaxMind");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PupMind.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
