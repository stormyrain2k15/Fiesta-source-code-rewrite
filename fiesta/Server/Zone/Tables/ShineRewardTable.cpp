// Server/Zone/Tables/ShineRewardTable.cpp
// FEATURE: world-creation -- ShineReward.shn binder.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace shine {
ShineRewardTable& ShineRewardTable::Get() { static ShineRewardTable s; return s; }
void ShineRewardTable::Bind() {
    // FEATURE: world-creation -- column read: RewardHandle, RewardType,
    // Argument, Quantity, Upgrade, OptionDegree, TitleDegree
    BIND_BEGIN(t, "ShineReward")
    ITER_ROWS(t) {
        Row r;
        r.uiHandle       = ShnGetU32(*t,_r,"RewardHandle");
        r.uiRewardType   = ShnGetU32(*t,_r,"RewardType");
        r.kArgument      = ShnGetStr(*t,_r,"Argument");
        r.uiQuantity     = ShnGetU32(*t,_r,"Quantity");
        r.uiUpgrade      = ShnGetU32(*t,_r,"Upgrade");
        r.uiOptionDegree = ShnGetU32(*t,_r,"OptionDegree");
        r.uiTitleDegree  = ShnGetU32(*t,_r,"TitleDegree");
        m_kByHandle[r.uiHandle] = m_kRows.size();
        m_kRows.push_back(r);
    }
}
const ShineRewardTable::Row* ShineRewardTable::Find(uint32 h) const {
    std::map<uint32,size_t>::const_iterator i=m_kByHandle.find(h);
    return i==m_kByHandle.end()?NULL:&m_kRows[i->second];
}
} // namespace shine
