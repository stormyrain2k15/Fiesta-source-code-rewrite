// Server/Zone/Engines/CollectEngine.h
// Auto-generated: CollectEngine — aggregates 6 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_COLLECTENGINE_H
#define FIESTA_ZONE_ENGINES_COLLECTENGINE_H
#include "../../DataReader/SHN/CollectCardGroupDesc.h"
#include "../../DataReader/SHN/CollectCardMobGroup.h"
#include "../../DataReader/SHN/CollectCardReward.h"
#include "../../DataReader/SHN/CollectCardStarRate.h"
#include "../../DataReader/SHN/CollectCardTitle.h"
#include "../../DataReader/SHN/WeaponTitleData.h"
#include <vector>
#include <string>

namespace fiesta {

class CollectEngine {
public:
    static CollectEngine& Get();
    void Bind();

    // Per-SHN accessors
    CollectCardGroupDescShn& collectCardGroupDesc() { return CollectCardGroupDescShn::Get(); }
    CollectCardMobGroupShn& collectCardMobGroup() { return CollectCardMobGroupShn::Get(); }
    CollectCardRewardShn& collectCardReward() { return CollectCardRewardShn::Get(); }
    CollectCardStarRateShn& collectCardStarRate() { return CollectCardStarRateShn::Get(); }
    CollectCardTitleShn& collectCardTitle() { return CollectCardTitleShn::Get(); }
    WeaponTitleDataShn& weaponTitleData() { return WeaponTitleDataShn::Get(); }

private:
    CollectEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_COLLECTENGINE_H
