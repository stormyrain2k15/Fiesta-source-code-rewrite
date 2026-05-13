// Server/Zone/Engines/UpgradeEngine.cpp
// Auto-generated: UpgradeEngine — calls Load() on each constituent SHN.
#include "UpgradeEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

UpgradeEngine& UpgradeEngine::Get() { static UpgradeEngine s; return s; }

void UpgradeEngine::Bind() {
    AccUpGradeInfoShn::Get().Load();
    AccUpgradeShn::Get().Load();
    BRAccUpgradeShn::Get().Load();
    BRAccUpgradeInfoShn::Get().Load();
    ColorInfoShn::Get().Load();
    EnchantSocketRateShn::Get().Load();
    GatherShn::Get().Load();
    JobEquipInfoShn::Get().Load();
    RandomOptionShn::Get().Load();
    RandomOptionCountShn::Get().Load();
    SetItemShn::Get().Load();
    SetItemEffectShn::Get().Load();
    UpgradeInfoShn::Get().Load();
    SHINELOG_INFO("UpgradeEngine::Bind done");
}

} // namespace shine
