// Server/Zone/Engines/MountEngine.h
// Auto-generated: MountEngine — aggregates 9 SHN loader(s).
#ifndef SHINE_ZONE_ENGINES_MOUNTENGINE_H
#define SHINE_ZONE_ENGINES_MOUNTENGINE_H
#include "../../DataReader/SHN/MoverAbility.h"
#include "../../DataReader/SHN/MoverHG.h"
#include "../../DataReader/SHN/MoverItem.h"
#include "../../DataReader/SHN/MoverMain.h"
#include "../../DataReader/SHN/MoverUpgradeEffect.h"
#include "../../DataReader/SHN/RareMoverEachRate.h"
#include "../../DataReader/SHN/RareMoverRate.h"
#include "../../DataReader/SHN/RareMoverSubRate.h"
#include "../../DataReader/SHN/Riding.h"
#include <vector>
#include <string>

namespace shine {

class MountEngine {
public:
    static MountEngine& Get();
    void Bind();

    // Per-SHN accessors
    MoverAbilityShn& moverAbility() { return MoverAbilityShn::Get(); }
    MoverHGShn& moverHG() { return MoverHGShn::Get(); }
    MoverItemShn& moverItem() { return MoverItemShn::Get(); }
    MoverMainShn& moverMain() { return MoverMainShn::Get(); }
    MoverUpgradeEffectShn& moverUpgradeEffect() { return MoverUpgradeEffectShn::Get(); }
    RareMoverEachRateShn& rareMoverEachRate() { return RareMoverEachRateShn::Get(); }
    RareMoverRateShn& rareMoverRate() { return RareMoverRateShn::Get(); }
    RareMoverSubRateShn& rareMoverSubRate() { return RareMoverSubRateShn::Get(); }
    RidingShn& riding() { return RidingShn::Get(); }

private:
    MountEngine() {}
};

} // namespace shine
#endif // SHINE_ZONE_ENGINES_MOUNTENGINE_H
