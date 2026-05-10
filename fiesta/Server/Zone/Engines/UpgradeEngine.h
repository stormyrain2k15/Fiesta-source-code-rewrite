// Server/Zone/Engines/UpgradeEngine.h
// Auto-generated: UpgradeEngine — aggregates 13 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_UPGRADEENGINE_H
#define FIESTA_ZONE_ENGINES_UPGRADEENGINE_H
#include "../../DataReader/SHN/AccUpGradeInfo.h"
#include "../../DataReader/SHN/AccUpgrade.h"
#include "../../DataReader/SHN/BRAccUpgrade.h"
#include "../../DataReader/SHN/BRAccUpgradeInfo.h"
#include "../../DataReader/SHN/ColorInfo.h"
#include "../../DataReader/SHN/EnchantSocketRate.h"
#include "../../DataReader/SHN/Gather.h"
#include "../../DataReader/SHN/JobEquipInfo.h"
#include "../../DataReader/SHN/RandomOption.h"
#include "../../DataReader/SHN/RandomOptionCount.h"
#include "../../DataReader/SHN/SetItem.h"
#include "../../DataReader/SHN/SetItemEffect.h"
#include "../../DataReader/SHN/UpgradeInfo.h"
#include <vector>
#include <string>

namespace fiesta {

class UpgradeEngine {
public:
    static UpgradeEngine& Get();
    void Bind();

    // Per-SHN accessors
    AccUpGradeInfoShn& accUpGradeInfo() { return AccUpGradeInfoShn::Get(); }
    AccUpgradeShn& accUpgrade() { return AccUpgradeShn::Get(); }
    BRAccUpgradeShn& bRAccUpgrade() { return BRAccUpgradeShn::Get(); }
    BRAccUpgradeInfoShn& bRAccUpgradeInfo() { return BRAccUpgradeInfoShn::Get(); }
    ColorInfoShn& colorInfo() { return ColorInfoShn::Get(); }
    EnchantSocketRateShn& enchantSocketRate() { return EnchantSocketRateShn::Get(); }
    GatherShn& gather() { return GatherShn::Get(); }
    JobEquipInfoShn& jobEquipInfo() { return JobEquipInfoShn::Get(); }
    RandomOptionShn& randomOption() { return RandomOptionShn::Get(); }
    RandomOptionCountShn& randomOptionCount() { return RandomOptionCountShn::Get(); }
    SetItemShn& setItem() { return SetItemShn::Get(); }
    SetItemEffectShn& setItemEffect() { return SetItemEffectShn::Get(); }
    UpgradeInfoShn& upgradeInfo() { return UpgradeInfoShn::Get(); }

private:
    UpgradeEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_UPGRADEENGINE_H
