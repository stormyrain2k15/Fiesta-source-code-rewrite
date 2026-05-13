// Server/Zone/MoverUpgradeData.cpp
// Mount upgrade table -- adds speed / HP / skin tiers. NA2016 file is
// MoverUpgradeEffect.shn (earlier code referenced "MoverUpgrade" which
// doesn't exist).
#include "../DataReader/ShnRegistry.h"
namespace shine {
class MoverUpgradeData {
public:
    static MoverUpgradeData& Get() { static MoverUpgradeData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MoverUpgradeEffect") != NULL; }
    const ShnFile* Table() const { return ShnRegistry::Get().GetTable("MoverUpgradeEffect"); }
};
} // namespace shine
