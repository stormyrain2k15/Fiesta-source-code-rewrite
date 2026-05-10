// Server/DataReader/SHN/MapBuff.cpp
// Auto-generated: one-file-per-SHN split for MapBuff.shn
#include "MapBuff.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MapBuffShn& MapBuffShn::Get() { static MapBuffShn s; return s; }

void MapBuffShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MapBuff");
    if (!t) { SHINELOG_WARN("MapBuff.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MapBuffRow rec;
        rec.kMapName = ShnGetStr(*t, _r, "MapName");
        rec.uiModeIDLv = ShnGetU32(*t, _r, "ModeIDLv");
        rec.kAbStateIDX = ShnGetStr(*t, _r, "AbStateIDX");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MapBuff.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
