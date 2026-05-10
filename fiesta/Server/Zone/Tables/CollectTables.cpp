// Server/Zone/Tables/CollectTables.cpp
// FEATURE: world-creation -- moved from GroupTables.cpp under
// the one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
#include "BindMacros.h"
#include "../GroupTables.h"

namespace fiesta {

CollectTables& CollectTables::Get() { static CollectTables s; return s; }
void CollectTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CollectCard")) {
        ITER_ROWS(t) {
            CollectCardRow rec;
            rec.uiCardID        = ShnGetU32(*t, _r, "CC_CardID");
            rec.kItemInx        = ShnGetStr(*t, _r, "CC_ItemInx");
            rec.uiCardGradeType = ShnGetU32(*t, _r, "CC_CardGradeType");
            rec.uiMobGroup      = ShnGetU32(*t, _r, "CC_CardMobGroup");
            m_kCardById[rec.uiCardID] = m_kCards.size();
            m_kCards.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardReward")) {
        ITER_ROWS(t) {
            CollectCardRewardRow rec;
            rec.uiID      = ShnGetU32(*t, _r, "ID");
            rec.uiPercent = ShnGetU32(*t, _r, "Percent");
            rec.kReward   = ShnGetStr(*t, _r, "Reward");
            rec.uiQty     = ShnGetU32(*t, _r, "Qty");
            rec.uiBonus   = ShnGetU32(*t, _r, "Bonus");
            m_kRewardById[rec.uiID] = m_kReward.size();
            m_kReward.push_back(rec);
        }
    }
}
const CollectCardRow*       CollectTables::FindCard  (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kCardById.find(uiID);
    return (it == m_kCardById.end()) ? NULL : &m_kCards[it->second]; }
const CollectCardRewardRow* CollectTables::FindReward(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kRewardById.find(uiID);
    return (it == m_kRewardById.end()) ? NULL : &m_kReward[it->second]; }


} // namespace fiesta
