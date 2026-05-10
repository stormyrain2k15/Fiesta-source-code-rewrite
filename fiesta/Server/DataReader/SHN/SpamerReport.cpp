// Server/DataReader/SHN/SpamerReport.cpp
// Auto-generated: one-file-per-SHN split for SpamerReport.shn
#include "SpamerReport.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

SpamerReportShn& SpamerReportShn::Get() { static SpamerReportShn s; return s; }

void SpamerReportShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("SpamerReport");
    if (!t) { SHINELOG_WARN("SpamerReport.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        SpamerReportRow rec;
        rec.uiSR_Term = ShnGetU32(*t, _r, "SR_Term");
        rec.uiSR_Number = (uint16)ShnGetU32(*t, _r, "SR_Number");
        rec.kSR_Message = ShnGetStr(*t, _r, "SR_Message");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("SpamerReport.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
