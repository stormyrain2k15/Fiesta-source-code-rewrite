// Server/DataReader/SHN/MapWayPoint.cpp
// Auto-generated: one-file-per-SHN split for MapWayPoint.shn
#include "MapWayPoint.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MapWayPointShn& MapWayPointShn::Get() { static MapWayPointShn s; return s; }

void MapWayPointShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MapWayPoint");
    if (!t) { SHINELOG_WARN("MapWayPoint.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MapWayPointRow rec;
        rec.uiMapID = (uint16)ShnGetU32(*t, _r, "MapID");
        rec.uiX = ShnGetU32(*t, _r, "X");
        rec.uiY = ShnGetU32(*t, _r, "Y");
        rec.uiMWP_Gate = (uint8)ShnGetU32(*t, _r, "MWP_Gate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MapWayPoint.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
