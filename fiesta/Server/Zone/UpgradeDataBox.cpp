// Server/Zone/UpgradeDataBox.cpp
// Generic upgrade data box -- bundles weapon / armor / accessory
// upgrade tables for one-stop lookup.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class UpgradeDataBox {
public:
    static UpgradeDataBox& Get() { static UpgradeDataBox s; return s; }
    bool Load() {
        return ShnRegistry::Get().GetTable("WeaponUpgrade")  != NULL
            || ShnRegistry::Get().GetTable("ArmorUpgrade")   != NULL;
    }
};
} // namespace fiesta
