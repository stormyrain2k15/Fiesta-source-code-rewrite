// Server/Zone/Tables/RecallCoordTable.cpp
// FEATURE: world-creation -- World/RecallCoord.txt binder.
// Recall-stone destinations: each item->coords mapping. Used by
// recall-scroll item-use to teleport the player back to a saved
// town/shrine.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

RecallCoordTable& RecallCoordTable::Get() { static RecallCoordTable s; return s; }

bool RecallCoordTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\RecallCoord.txt")) return false;
    // FEATURE: world-creation -- column read: ItemIndex, ItemIdent,
    // MapName, LinkX, LinkY
    if (const TsTable* t = f.Find("RecallPoint")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            RecallCoordRow x;
            x.kItemIndex  = t->GetStr(r, "ItemIndex");
            x.uiItemIdent = (uint32)t->GetInt(r, "ItemIdent");
            x.kMapName    = t->GetStr(r, "MapName");
            x.uiLinkX     = (uint16)t->GetInt(r, "LinkX");
            x.uiLinkY     = (uint16)t->GetInt(r, "LinkY");
            if (!x.kItemIndex.empty()) m_kRows[x.kItemIndex] = x;
        }
    }
    return true;
}

const RecallCoordRow* RecallCoordTable::FindByItem(const std::string& r) const {
    std::map<std::string, RecallCoordRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

} // namespace shine
