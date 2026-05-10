// Server/DataReader/SHN/PupMain.cpp
// Auto-generated: one-file-per-SHN split for PupMain.shn
#include "PupMain.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PupMainShn& PupMainShn::Get() { static PupMainShn s; return s; }

void PupMainShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PupMain");
    if (!t) { SHINELOG_WARN("PupMain.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PupMainRow rec;
        rec.uiPupID = ShnGetU32(*t, _r, "PupID");
        rec.kPupIDX = ShnGetStr(*t, _r, "PupIDX");
        rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
        rec.uiPupSpeed = (uint16)ShnGetU32(*t, _r, "PupSpeed");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PupMain.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
