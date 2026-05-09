// Server/Zone/KQRewardDataBox.cpp
// KQ reward data -- per-(KQID, kingdom, contribution-tier) item set.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class KQRewardDataBox {
public:
    static KQRewardDataBox& Get() { static KQRewardDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("KQReward") != NULL; }
};
}} // anonymous
