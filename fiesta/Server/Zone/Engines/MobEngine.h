// Server/Zone/Engines/MobEngine.h
// Auto-generated: MobEngine — aggregates 14 SHN loader(s).
#ifndef SHINE_ZONE_ENGINES_MOBENGINE_H
#define SHINE_ZONE_ENGINES_MOBENGINE_H
#include "../../DataReader/SHN/MobAbStateDropSetting.h"
#include "../../DataReader/SHN/MobAutoAction.h"
#include "../../DataReader/SHN/MobConditionServer.h"
#include "../../DataReader/SHN/MobKillAble.h"
#include "../../DataReader/SHN/MobKillAnnounce.h"
#include "../../DataReader/SHN/MobKillLog.h"
#include "../../DataReader/SHN/MobLifeTime.h"
#include "../../DataReader/SHN/MobNoFadeIn.h"
#include "../../DataReader/SHN/MobRegenAni.h"
#include "../../DataReader/SHN/MobResist.h"
#include "../../DataReader/SHN/MobSpecies.h"
#include "../../DataReader/SHN/MobWeapon.h"
#include "../../DataReader/SHN/NpcDialogData.h"
#include "../../DataReader/SHN/ReactionType.h"
#include <vector>
#include <string>

namespace shine {

class MobEngine {
public:
    static MobEngine& Get();
    void Bind();

    // Per-SHN accessors
    MobAbStateDropSettingShn& mobAbStateDropSetting() { return MobAbStateDropSettingShn::Get(); }
    MobAutoActionShn& mobAutoAction() { return MobAutoActionShn::Get(); }
    MobConditionServerShn& mobConditionServer() { return MobConditionServerShn::Get(); }
    MobKillAbleShn& mobKillAble() { return MobKillAbleShn::Get(); }
    MobKillAnnounceShn& mobKillAnnounce() { return MobKillAnnounceShn::Get(); }
    MobKillLogShn& mobKillLog() { return MobKillLogShn::Get(); }
    MobLifeTimeShn& mobLifeTime() { return MobLifeTimeShn::Get(); }
    MobNoFadeInShn& mobNoFadeIn() { return MobNoFadeInShn::Get(); }
    MobRegenAniShn& mobRegenAni() { return MobRegenAniShn::Get(); }
    MobResistShn& mobResist() { return MobResistShn::Get(); }
    MobSpeciesShn& mobSpecies() { return MobSpeciesShn::Get(); }
    MobWeaponShn& mobWeapon() { return MobWeaponShn::Get(); }
    NpcDialogDataShn& npcDialogData() { return NpcDialogDataShn::Get(); }
    ReactionTypeShn& reactionType() { return ReactionTypeShn::Get(); }

private:
    MobEngine() {}
};

} // namespace shine
#endif // SHINE_ZONE_ENGINES_MOBENGINE_H
