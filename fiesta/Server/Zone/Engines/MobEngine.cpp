// Server/Zone/Engines/MobEngine.cpp
// Auto-generated: MobEngine — calls Load() on each constituent SHN.
#include "MobEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobEngine& MobEngine::Get() { static MobEngine s; return s; }

void MobEngine::Bind() {
    MobAbStateDropSettingShn::Get().Load();
    MobAutoActionShn::Get().Load();
    MobConditionServerShn::Get().Load();
    MobKillAbleShn::Get().Load();
    MobKillAnnounceShn::Get().Load();
    MobKillLogShn::Get().Load();
    MobLifeTimeShn::Get().Load();
    MobNoFadeInShn::Get().Load();
    MobRegenAniShn::Get().Load();
    MobResistShn::Get().Load();
    MobSpeciesShn::Get().Load();
    MobWeaponShn::Get().Load();
    NpcDialogDataShn::Get().Load();
    ReactionTypeShn::Get().Load();
    SHINELOG_INFO("MobEngine::Bind done");
}

} // namespace shine
