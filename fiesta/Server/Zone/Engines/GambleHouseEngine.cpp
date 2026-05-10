// Server/Zone/Engines/GambleHouseEngine.cpp
#include "GambleHouseEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GambleHouseEngine& GambleHouseEngine::Get() { static GambleHouseEngine s; return s; }

void GambleHouseEngine::Bind() {
    DiceDividindShn::Get().Load();
    DiceGameShn::Get().Load();
    DiceRateShn::Get().Load();
    GBBanTimeShn::Get().Load();
    GBDiceDividindShn::Get().Load();
    GBDiceGameShn::Get().Load();
    GBDiceRateShn::Get().Load();
    GBEventCodeShn::Get().Load();
    GBExchangeMaxCoinShn::Get().Load();
    GBHouseShn::Get().Load();
    GBJoinGameMemberShn::Get().Load();
    GBRewardShn::Get().Load();
    GBSMAllShn::Get().Load();
    GBSMBetCoinShn::Get().Load();
    GBSMCardRateShn::Get().Load();
    GBSMCenterShn::Get().Load();
    GBSMGroupShn::Get().Load();
    GBSMJPRateShn::Get().Load();
    GBSMLineShn::Get().Load();
    GBSMNPCShn::Get().Load();
    GBTaxRateShn::Get().Load();
    LCGroupRateShn::Get().Load();
    LCRewardShn::Get().Load();
    SHINELOG_INFO("GambleHouseEngine::Bind done");
}

} // namespace fiesta
