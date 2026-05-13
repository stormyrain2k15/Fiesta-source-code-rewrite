// Server/Zone/AccUpgradeTable.cpp
// Accessory-upgrade table (AccUpgrade.shn) -- different formula from
// weapon upgrades. The accessory variant uses additive deltas, not %.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class AccUpgradeTable {
public:
    static AccUpgradeTable& Get() { static AccUpgradeTable s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("AccUpgrade") != NULL; }
};
} // namespace shine
