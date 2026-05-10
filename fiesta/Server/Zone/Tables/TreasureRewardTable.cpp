// Server/Zone/Tables/TreasureRewardTable.cpp
// FEATURE: world-creation -- World/TreasureReward.txt binder.
#include "TreasureRewardTable.h"
#include "../../DataReader/TableScriptFile.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

TreasureRewardTable& TreasureRewardTable::Get() {
    static TreasureRewardTable s; return s;
}

bool TreasureRewardTable::Load(const std::string& rRoot) {
    m_kContainers.clear(); m_kCByItem.clear();
    m_kContents.clear();   m_kContentByItem.clear();
    TableScriptFile f;
    if (!f.Load(rRoot + "\\World\\TreasureReward.txt")) return false;

    // FEATURE: world-creation -- column read: ItemID, Index, CardInx,
    // MinLot, MaxLot, DummyInx
    if (const TsTable* t = f.Find("Container")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            TreasureContainer x;
            x.uiItemID  = (uint32)t->GetInt(r, "ItemID");
            x.uiIndex   = (uint16)t->GetInt(r, "Index");
            x.kCardInx  = t->GetStr(r, "CardInx");
            x.uiMinLot  = (uint16)t->GetInt(r, "MinLot");
            x.uiMaxLot  = (uint16)t->GetInt(r, "MaxLot");
            x.kDummyInx = t->GetStr(r, "DummyInx");
            if (x.uiItemID == 0) continue;
            m_kCByItem[x.uiItemID] = m_kContainers.size();
            m_kContainers.push_back(x);
        }
    }
    // FEATURE: world-creation -- column read: ItemID, Index, RewardInx,
    // RewardLot, UpgradeLow, UpgradeHigh, RewardRate
    if (const TsTable* t = f.Find("Content")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            TreasureContent x;
            x.uiItemID      = (uint32)t->GetInt(r, "ItemID");
            x.uiIndex       = (uint16)t->GetInt(r, "Index");
            x.kRewardInx    = t->GetStr(r, "RewardInx");
            x.uiRewardLot   = (uint16)t->GetInt(r, "RewardLot");
            x.uiUpgradeLow  = (uint8) t->GetInt(r, "UpgradeLow");
            x.uiUpgradeHigh = (uint8) t->GetInt(r, "UpgradeHigh");
            x.uiRewardRate  = (uint32)t->GetInt(r, "RewardRate");
            if (x.uiItemID == 0 || x.kRewardInx.empty()) continue;
            m_kContentByItem[x.uiItemID].push_back(m_kContents.size());
            m_kContents.push_back(x);
        }
    }
    SHINELOG_INFO("TreasureReward.txt: %u containers, %u contents",
                  (uint32)m_kContainers.size(), (uint32)m_kContents.size());
    return true;
}

const TreasureContainer* TreasureRewardTable::FindContainer(uint32 u) const {
    std::map<uint32, size_t>::const_iterator it = m_kCByItem.find(u);
    return (it == m_kCByItem.end()) ? NULL : &m_kContainers[it->second];
}
void TreasureRewardTable::ContentsForContainer(uint32 u,
        std::vector<const TreasureContent*>& rOut) const {
    rOut.clear();
    std::map<uint32, std::vector<size_t> >::const_iterator it =
        m_kContentByItem.find(u);
    if (it == m_kContentByItem.end()) return;
    for (size_t i = 0; i < it->second.size(); ++i)
        rOut.push_back(&m_kContents[it->second[i]]);
}

} // namespace fiesta
