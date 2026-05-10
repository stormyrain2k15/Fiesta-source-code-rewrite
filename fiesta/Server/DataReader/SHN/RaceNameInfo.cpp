// Server/DataReader/SHN/RaceNameInfo.cpp
// Auto-generated: one-file-per-SHN split for RaceNameInfo.shn
#include "RaceNameInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

RaceNameInfoShn& RaceNameInfoShn::Get() { static RaceNameInfoShn s; return s; }

void RaceNameInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("RaceNameInfo");
    if (!t) { SHINELOG_WARN("RaceNameInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        RaceNameInfoRow rec;
        rec.uiRaceID = (uint8)ShnGetU32(*t, _r, "RaceID");
        rec.kAcPrefix = ShnGetStr(*t, _r, "acPrefix");
        rec.kAcEngName = ShnGetStr(*t, _r, "acEngName");
        rec.kAcLocalName = ShnGetStr(*t, _r, "acLocalName");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("RaceNameInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
