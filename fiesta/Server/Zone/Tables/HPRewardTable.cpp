// Server/Zone/Tables/HPRewardTable.cpp
// FEATURE: world-creation -- HolyPromiseReward.shn binder.
// PromiseCount -> Reward lookup for the Holy Promise daily/weekly
// reward chain.
#include "BindMacros.h"
#include "../MiscTables.h"

namespace shine {

HPRewardTable& HPRewardTable::Get() { static HPRewardTable s; return s; }

void HPRewardTable::Bind() {
    // FEATURE: world-creation -- column read: PromiseCount, Reward
    if (const ShnFile* t = ShnRegistry::Get().GetTable("HolyPromiseReward"))
        for (size_t i = 0; i < t->Rows().size(); ++i)
            m_kRow[ShnGetU32(*t, i, "PromiseCount")] = ShnGetI32(*t, i, "Reward");
}

int32 HPRewardTable::ForCount(uint32 uiCount) const {
    std::map<uint32, int32>::const_iterator it = m_kRow.find(uiCount);
    return (it == m_kRow.end()) ? 0 : it->second;
}

} // namespace shine
