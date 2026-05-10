// Server/Zone/Engines/CollectEngine.cpp
// Auto-generated: CollectEngine — calls Load() on each constituent SHN.
#include "CollectEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CollectEngine& CollectEngine::Get() { static CollectEngine s; return s; }

void CollectEngine::Bind() {
    CollectCardGroupDescShn::Get().Load();
    CollectCardMobGroupShn::Get().Load();
    CollectCardRewardShn::Get().Load();
    CollectCardStarRateShn::Get().Load();
    CollectCardTitleShn::Get().Load();
    WeaponTitleDataShn::Get().Load();
    SHINELOG_INFO("CollectEngine::Bind done");
}

} // namespace fiesta
