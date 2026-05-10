// Server/Zone/Engines/PartyEngine.h
// Auto-generated: PartyEngine — aggregates 5 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_PARTYENGINE_H
#define FIESTA_ZONE_ENGINES_PARTYENGINE_H
#include "../../DataReader/SHN/FriendPointReward.h"
#include "../../DataReader/SHN/HolyPromiseReward.h"
#include "../../DataReader/SHN/PartyBonusByLvDiff.h"
#include "../../DataReader/SHN/PartyBonusByMember.h"
#include "../../DataReader/SHN/PartyBonusLimit.h"
#include <vector>
#include <string>

namespace fiesta {

class PartyEngine {
public:
    static PartyEngine& Get();
    void Bind();

    // Per-SHN accessors
    FriendPointRewardShn& friendPointReward() { return FriendPointRewardShn::Get(); }
    HolyPromiseRewardShn& holyPromiseReward() { return HolyPromiseRewardShn::Get(); }
    PartyBonusByLvDiffShn& partyBonusByLvDiff() { return PartyBonusByLvDiffShn::Get(); }
    PartyBonusByMemberShn& partyBonusByMember() { return PartyBonusByMemberShn::Get(); }
    PartyBonusLimitShn& partyBonusLimit() { return PartyBonusLimitShn::Get(); }

private:
    PartyEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_PARTYENGINE_H
