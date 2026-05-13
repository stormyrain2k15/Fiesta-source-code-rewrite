// Server/Zone/Tables/ItemDropGroupTable.cpp
// FEATURE: world-creation -- World/ItemDropGroup.txt binder.
// Per-group drop bag: when a mob's DropGroupA / DropGroupB resolves to
// one of these group ids, this table picks (item, qty, upgrade tier)
// out for the actual drop. The 16 Upgrade%02d columns are per-tier
// chances per-1000.
#include "../WorldTables.h"
#include "../../DataReader/TableScriptFile.h"
#include <stdio.h>

namespace shine {

ItemDropGroupTable& ItemDropGroupTable::Get() { static ItemDropGroupTable s; return s; }

bool ItemDropGroupTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\ItemDropGroup.txt")) return false;
    const TsTable* t = f.Find("ItemDropGroup"); if (!t) return false;
    char buf[16];
    // FEATURE: world-creation -- column read: ItemGroupIdx, ItemID,
    // MinQtty, MaxQtty, Upgrade00..Upgrade15
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        ItemDropGroupRow x;
        x.kItemGroupIdx = t->GetStr(r, "ItemGroupIdx");
        x.kItemID       = t->GetStr(r, "ItemID");
        x.uiMinQty      = (uint16)t->GetInt(r, "MinQtty");
        x.uiMaxQty      = (uint16)t->GetInt(r, "MaxQtty");
        for (int k = 0; k < 16; ++k) {
            sprintf_s(buf, sizeof(buf), "Upgrade%02d", k);
            x.aUpgradeChance[k] = (uint16)t->GetInt(r, buf);
        }
        if (!x.kItemGroupIdx.empty()) m_kRows[x.kItemGroupIdx] = x;
    }
    return true;
}

const ItemDropGroupRow* ItemDropGroupTable::Find(const std::string& r) const {
    std::map<std::string, ItemDropGroupRow>::const_iterator it = m_kRows.find(r);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

} // namespace shine
