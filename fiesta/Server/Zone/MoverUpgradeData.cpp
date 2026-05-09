// Server/Zone/MoverUpgradeData.cpp
// Mount upgrade table -- adds speed / HP / skin tiers.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class MoverUpgradeData {
public:
    static MoverUpgradeData& Get() { static MoverUpgradeData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MoverUpgrade") != NULL; }
};
}} // anonymous
