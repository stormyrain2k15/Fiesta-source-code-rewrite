// Server/DataReader/SHN/TownPortal.cpp
// Auto-generated: one-file-per-SHN split for TownPortal.shn
#include "TownPortal.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

TownPortalShn& TownPortalShn::Get() { static TownPortalShn s; return s; }

void TownPortalShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("TownPortal");
    if (!t) { SHINELOG_WARN("TownPortal.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        TownPortalRow rec;
        rec.uiIndex = (uint8)ShnGetU32(*t, _r, "Index");
        rec.uiMinLevel = (uint8)ShnGetU32(*t, _r, "MinLevel");
        rec.uiTP_GroupNo = (uint8)ShnGetU32(*t, _r, "TP_GroupNo");
        rec.kMapName = ShnGetStr(*t, _r, "MapName");
        rec.uiX = ShnGetU32(*t, _r, "X");
        rec.uiY = ShnGetU32(*t, _r, "Y");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("TownPortal.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
