// Server/DataReader/SHN/MapInfo.cpp
// Auto-generated: one-file-per-SHN split for MapInfo.shn
#include "MapInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MapInfoShn& MapInfoShn::Get() { static MapInfoShn s; return s; }

void MapInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MapInfo");
    if (!t) { SHINELOG_WARN("MapInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MapInfoRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.kMapName = ShnGetStr(*t, _r, "MapName");
        rec.kName = ShnGetStr(*t, _r, "Name");
        rec.uiIsWMLink = ShnGetU32(*t, _r, "IsWMLink");
        rec.uiRegenX = ShnGetU32(*t, _r, "RegenX");
        rec.uiRegenY = ShnGetU32(*t, _r, "RegenY");
        rec.uiKingdomMap = (uint8)ShnGetU32(*t, _r, "KingdomMap");
        rec.kMapFolderName = ShnGetStr(*t, _r, "MapFolderName");
        rec.uiInSide = (uint8)ShnGetU32(*t, _r, "InSide");
        rec.uiSight = ShnGetU32(*t, _r, "Sight");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MapInfo.shn: %u rows", (uint32)m_kRows.size());
}

const MapInfoRow* MapInfoShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
