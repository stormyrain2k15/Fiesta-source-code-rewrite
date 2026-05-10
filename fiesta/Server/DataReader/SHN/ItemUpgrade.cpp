// Server/DataReader/SHN/ItemUpgrade.cpp
// Auto-generated: one-file-per-SHN split for ItemUpgrade.shn
#include "ItemUpgrade.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemUpgradeShn& ItemUpgradeShn::Get() { static ItemUpgradeShn s; return s; }

void ItemUpgradeShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemUpgrade");
    if (!t) { SHINELOG_WARN("ItemUpgrade.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemUpgradeRow rec;
        rec.uiID = ShnGetU32(*t, _r, "ID");
        rec.uiCriFail = (uint16)ShnGetU32(*t, _r, "CriFail");
        rec.uiDownFail = (uint16)ShnGetU32(*t, _r, "DownFail");
        rec.uiNormalFail = (uint16)ShnGetU32(*t, _r, "NormalFail");
        rec.uiNCon = (uint16)ShnGetU32(*t, _r, "nCon");
        rec.uiLuckySuc = (uint16)ShnGetU32(*t, _r, "LuckySuc");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemUpgrade.shn: %u rows", (uint32)m_kRows.size());
}

const ItemUpgradeRow* ItemUpgradeShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
