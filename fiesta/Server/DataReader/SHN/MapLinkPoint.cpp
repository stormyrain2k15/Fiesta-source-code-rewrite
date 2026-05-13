// Server/DataReader/SHN/MapLinkPoint.cpp
// Auto-generated: one-file-per-SHN split for MapLinkPoint.shn
#include "MapLinkPoint.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MapLinkPointShn& MapLinkPointShn::Get() { static MapLinkPointShn s; return s; }

void MapLinkPointShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MapLinkPoint");
    if (!t) { SHINELOG_WARN("MapLinkPoint.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MapLinkPointRow rec;
        rec.uiMLP_FromID = ShnGetU32(*t, _r, "MLP_FromID");
        rec.uiMLP_ToID = ShnGetU32(*t, _r, "MLP_ToID");
        rec.uiMLP_Weight = ShnGetU32(*t, _r, "MLP_Weight");
        rec.uiMLP_OneWay_Street = (uint16)ShnGetU32(*t, _r, "MLP_OneWay_Street");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MapLinkPoint.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
