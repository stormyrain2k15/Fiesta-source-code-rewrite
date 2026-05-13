// Server/DataReader/SHN/AnnounceData.cpp
// Auto-generated: one-file-per-SHN split for AnnounceData.shn
#include "AnnounceData.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

AnnounceDataShn& AnnounceDataShn::Get() { static AnnounceDataShn s; return s; }

void AnnounceDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AnnounceData");
    if (!t) { SHINELOG_WARN("AnnounceData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        AnnounceDataRow rec;
        rec.uiType = ShnGetU32(*t, _r, "Type");
        rec.kMessage = ShnGetStr(*t, _r, "Message");
        rec.uiValue = ShnGetU32(*t, _r, "Value");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AnnounceData.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
