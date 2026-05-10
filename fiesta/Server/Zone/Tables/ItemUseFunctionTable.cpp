// Server/Zone/Tables/ItemUseFunctionTable.cpp
// FEATURE: world-creation -- World/ItemUseFunction.txt binder.
// Item -> function name dispatch table. When the player uses an item,
// the runtime looks up the inx-name here to decide which use-function
// to run (potion, scroll, town-portal, capsule, summon, etc.).
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"

namespace fiesta {

ItemUseFunctionTable& ItemUseFunctionTable::Get() { static ItemUseFunctionTable s; return s; }

bool ItemUseFunctionTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ItemUseFunction.txt")) return false;
    const TsTable* t = f.Find("ItemUseFunction"); if (!t) return false;
    // FEATURE: world-creation -- column read: ItemIndex, BroadCast, UseFunction
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        ItemUseFunctionRow x;
        x.kItemIndex   = t->GetStr(r, "ItemIndex");
        x.uiBroadcast  = (uint8)t->GetInt(r, "BroadCast");
        x.kUseFunction = t->GetStr(r, "UseFunction");
        if (!x.kItemIndex.empty()) m_kRows[x.kItemIndex] = x;
    }
    return true;
}

const ItemUseFunctionRow* ItemUseFunctionTable::Find(const std::string& r) const {
    std::map<std::string, ItemUseFunctionRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

} // namespace fiesta
