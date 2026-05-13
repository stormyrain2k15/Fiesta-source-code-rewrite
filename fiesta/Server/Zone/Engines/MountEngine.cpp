// Server/Zone/Engines/MountEngine.cpp
// Auto-generated: MountEngine — calls Load() on each constituent SHN.
#include "MountEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MountEngine& MountEngine::Get() { static MountEngine s; return s; }

void MountEngine::Bind() {
    MoverAbilityShn::Get().Load();
    MoverHGShn::Get().Load();
    MoverItemShn::Get().Load();
    MoverMainShn::Get().Load();
    MoverUpgradeEffectShn::Get().Load();
    RareMoverEachRateShn::Get().Load();
    RareMoverRateShn::Get().Load();
    RareMoverSubRateShn::Get().Load();
    RidingShn::Get().Load();
    SHINELOG_INFO("MountEngine::Bind done");
}

} // namespace shine
