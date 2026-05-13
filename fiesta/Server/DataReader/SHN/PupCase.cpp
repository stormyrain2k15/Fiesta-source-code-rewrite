// Server/DataReader/SHN/PupCase.cpp
// Auto-generated: one-file-per-SHN split for PupCase.shn
#include "PupCase.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

PupCaseShn& PupCaseShn::Get() { static PupCaseShn s; return s; }

void PupCaseShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PupCase");
    if (!t) { SHINELOG_WARN("PupCase.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PupCaseRow rec;
        rec.uiPupCaseType = ShnGetU32(*t, _r, "PupCaseType");
        rec.uiPupMindType = ShnGetU32(*t, _r, "PupMindType");
        rec.uiStressNum = (uint8)ShnGetU32(*t, _r, "StressNum");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PupCase.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
