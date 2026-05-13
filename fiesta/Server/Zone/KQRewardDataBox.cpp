// Server/Zone/KQRewardDataBox.cpp
// KQ reward data -- per-(KQID, kingdom, contribution-tier) item set.
// NA2016 file is KingdomQuestRew.shn (earlier code referenced "KQReward"
// which doesn't exist).
#include "../DataReader/ShnRegistry.h"
namespace shine {
class KQRewardDataBox {
public:
    static KQRewardDataBox& Get() { static KQRewardDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("KingdomQuestRew") != NULL; }
    const ShnFile* Table() const { return ShnRegistry::Get().GetTable("KingdomQuestRew"); }
};
} // namespace shine
