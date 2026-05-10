// Server/Zone/Engines/PartyEngine.cpp
// Auto-generated: PartyEngine — calls Load() on each constituent SHN.
#include "PartyEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PartyEngine& PartyEngine::Get() { static PartyEngine s; return s; }

void PartyEngine::Bind() {
    FriendPointRewardShn::Get().Load();
    HolyPromiseRewardShn::Get().Load();
    PartyBonusByLvDiffShn::Get().Load();
    PartyBonusByMemberShn::Get().Load();
    PartyBonusLimitShn::Get().Load();
    SHINELOG_INFO("PartyEngine::Bind done");
}

} // namespace fiesta
