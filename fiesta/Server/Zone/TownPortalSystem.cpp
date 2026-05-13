// Server/Zone/TownPortalSystem.cpp
// FEATURE: portals
#include "TownPortalSystem.h"
#include "MapField.h"
#include "GroupTables.h"
#include "ShineObject.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"
#include <string.h>

namespace shine {

TownPortalSystem& TownPortalSystem::Get() { static TownPortalSystem s; return s; }

bool TownPortalSystem::Load() {
    m_kRows.clear();
    // FEATURE: portals -- column read: Index, MinLevel, TP_GroupNo,
    // MapName, X, Y
    const ShnFile* t = ShnRegistry::Get().GetTable("TownPortal");
    if (!t) {
        SHINELOG_WARN("TownPortalSystem: TownPortal.shn missing");
        return false;
    }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        LegacyTownPortalRow r;
        r.uiIndex    = (uint8) ShnGetU32(*t, i, "Index");
        r.uiMinLevel = (uint8) ShnGetU32(*t, i, "MinLevel");
        r.uiGroupNo  = (uint8) ShnGetU32(*t, i, "TP_GroupNo");
        r.kMapName   =          ShnGetStr(*t, i, "MapName");
        r.uiX        =          ShnGetU32(*t, i, "X");
        r.uiY        =          ShnGetU32(*t, i, "Y");
        if (r.kMapName.empty()) continue;
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("TownPortalSystem: %u destinations loaded",
                  (uint32)m_kRows.size());
    return !m_kRows.empty();
}

int32 TownPortalSystem::ResolveItemToGroup(uint32 /*uiItemId*/,
                                           const std::string& rInxName) const
{
    // NA2016 town-portal scroll inx names follow a tier convention.
    // Tier 0 (capital cities) -- Lv1+ scrolls.
    // Tier 1 (mid-zone hubs)  -- Lv30+ scrolls.
    // Tier 2 (high-zone hubs) -- Lv60+ scrolls.
    // Tier 3 (event/special)  -- reserved.
    // Mapping is by inx-name substring; the actual scroll item ids
    // live in ItemInfo.shn under MaTeleport*, which is what the
    // dictionary calls them.
    //
    // PROVISIONAL_BODY: confirm scroll inx-name -> group mapping
    // against the live NA2016 data once the inventory log is wired.
    if (rInxName.empty()) return -1;
    // Cheap case-insensitive substring check.
    const char* s = rInxName.c_str();
    if (strstr(s, "Tele1") || strstr(s, "TPort1") ||
        strstr(s, "tele1") || strstr(s, "tport1")) return 0;
    if (strstr(s, "Tele2") || strstr(s, "TPort2") ||
        strstr(s, "tele2") || strstr(s, "tport2")) return 1;
    if (strstr(s, "Tele3") || strstr(s, "TPort3") ||
        strstr(s, "tele3") || strstr(s, "tport3")) return 2;
    // Generic "Mateleport" / "Karen" scrolls map to group 0.
    if (strstr(s, "Mateleport") || strstr(s, "Karen") ||
        strstr(s, "MaTele") || strstr(s, "Recall")) return 0;
    return -1;
}

void TownPortalSystem::BuildList(uint8 uiGroup, uint8 uiPlayerLv,
                                 std::vector<LegacyTownPortalRow>& rOut) const
{
    rOut.clear();
    for (size_t i = 0; i < m_kRows.size(); ++i) {
        const LegacyTownPortalRow& r = m_kRows[i];
        if (r.uiGroupNo  != uiGroup)     continue;
        if (r.uiMinLevel >  uiPlayerLv)  continue;
        rOut.push_back(r);
    }
}

bool TownPortalSystem::Teleport(ShinePlayer* pk, uint8 uiGroup,
                                uint8 uiIndex) const
{
    if (!pk) return false;
    for (size_t i = 0; i < m_kRows.size(); ++i) {
        const LegacyTownPortalRow& r = m_kRows[i];
        if (r.uiGroupNo != uiGroup) continue;
        if (r.uiIndex   != uiIndex) continue;
        if (r.uiMinLevel > pk->GetLevel()) {
            SHINELOG_WARN("TownPortal: cid=%u tried under-level "
                          "tp (group=%u idx=%u min=%u, has=%u)",
                          pk->GetCharID(), uiGroup, uiIndex,
                          r.uiMinLevel, pk->GetLevel());
            return false;
        }
        // Resolve map inx-name -> MapID (MapTables loaded from MapInfo.shn).
        const LegacyMapInfoRow* pkMap = MapTables::Get().FindByName(r.kMapName);
        if (!pkMap) {
            SHINELOG_WARN("TownPortal: cid=%u map '%s' unknown",
                          pk->GetCharID(), r.kMapName.c_str());
            return false;
        }
        Vec3 spawn((float)r.uiX, (float)r.uiY, 0.0f);
        TownPortal(pk, (MapID)pkMap->uiID, spawn);
        return true;
    }
    return false;
}

} // namespace shine
