// Server/DataReader/SHN/BRAccUpgrade.cpp
// Auto-generated: one-file-per-SHN split for BRAccUpgrade.shn
#include "BRAccUpgrade.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

BRAccUpgradeShn& BRAccUpgradeShn::Get() { static BRAccUpgradeShn s; return s; }

void BRAccUpgradeShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("BRAccUpgrade");
    if (!t) { SHINELOG_WARN("BRAccUpgrade.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        BRAccUpgradeRow rec;
        rec.uiID = ShnGetU32(*t, _r, "ID");
        rec.uiCriFail = (uint16)ShnGetU32(*t, _r, "CriFail");
        rec.uiDownFail = (uint16)ShnGetU32(*t, _r, "DownFail");
        rec.uiNormalFail = (uint16)ShnGetU32(*t, _r, "NormalFail");
        rec.uiNCon = (uint16)ShnGetU32(*t, _r, "nCon");
        rec.uiLuckySuc = (uint16)ShnGetU32(*t, _r, "LuckySuc");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("BRAccUpgrade.shn: %u rows", (uint32)m_kRows.size());
}

const BRAccUpgradeRow* BRAccUpgradeShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
