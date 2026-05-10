// Server/Zone/Tables/MapTables.cpp
// FEATURE: world-creation -- MapInfo.shn binder.
// One .cpp per .shn (NA2016 canon). The world IS its maps; this is
// the most foundational table in the engine. Without this, no Field
// can exist and every other system that resolves a position is dead.
#include "BindMacros.h"
#include "../GroupTables.h"

namespace fiesta {

MapTables& MapTables::Get() { static MapTables s; return s; }

void MapTables::Bind() {
    // FEATURE: world-creation -- column read: ID, MapName, Name,
    // IsWMLink, RegenX, RegenY, KingdomMap, MapFolderName, InSide, Sight
    BIND_BEGIN(t, "MapInfo")
    m_kMaps.reserve(t->Rows().size());
    ITER_ROWS(t) {
        LegacyMapInfoRow rec;
        rec.uiID         = ShnGetU32(*t, _r, "ID");
        rec.kMapName     = ShnGetStr(*t, _r, "MapName");
        rec.kName        = ShnGetStr(*t, _r, "Name");
        rec.uiIsWMLink   = ShnGetU32(*t, _r, "IsWMLink");
        rec.iRegenX      = ShnGetI32(*t, _r, "RegenX");
        rec.iRegenY      = ShnGetI32(*t, _r, "RegenY");
        rec.uiKingdomMap = ShnGetU32(*t, _r, "KingdomMap");
        rec.kFolder      = ShnGetStr(*t, _r, "MapFolderName");
        rec.uiInSide     = ShnGetU32(*t, _r, "InSide");
        rec.uiSight      = ShnGetU32(*t, _r, "Sight");
        m_kById[rec.uiID]       = m_kMaps.size();
        m_kByName[rec.kMapName] = m_kMaps.size();
        m_kMaps.push_back(rec);
    }
    SHINELOG_INFO("MapInfo.shn: %u rows", (uint32)m_kMaps.size());
}

const LegacyMapInfoRow* MapTables::Find(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kMaps[it->second];
}

const LegacyMapInfoRow* MapTables::FindByName(const std::string& rN) const {
    std::map<std::string, size_t>::const_iterator it = m_kByName.find(rN);
    return (it == m_kByName.end()) ? NULL : &m_kMaps[it->second];
}

} // namespace fiesta
