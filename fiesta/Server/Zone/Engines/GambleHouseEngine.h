// Server/Zone/Engines/GambleHouseEngine.h
// GambleHouseEngine — aggregates 23 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_GAMBLEHOUSEENGINE_H
#define FIESTA_ZONE_ENGINES_GAMBLEHOUSEENGINE_H
#include "../../DataReader/SHN/DiceDividind.h"
#include "../../DataReader/SHN/DiceGame.h"
#include "../../DataReader/SHN/DiceRate.h"
#include "../../DataReader/SHN/GBBanTime.h"
#include "../../DataReader/SHN/GBDiceDividind.h"
#include "../../DataReader/SHN/GBDiceGame.h"
#include "../../DataReader/SHN/GBDiceRate.h"
#include "../../DataReader/SHN/GBEventCode.h"
#include "../../DataReader/SHN/GBExchangeMaxCoin.h"
#include "../../DataReader/SHN/GBHouse.h"
#include "../../DataReader/SHN/GBJoinGameMember.h"
#include "../../DataReader/SHN/GBReward.h"
#include "../../DataReader/SHN/GBSMAll.h"
#include "../../DataReader/SHN/GBSMBetCoin.h"
#include "../../DataReader/SHN/GBSMCardRate.h"
#include "../../DataReader/SHN/GBSMCenter.h"
#include "../../DataReader/SHN/GBSMGroup.h"
#include "../../DataReader/SHN/GBSMJPRate.h"
#include "../../DataReader/SHN/GBSMLine.h"
#include "../../DataReader/SHN/GBSMNPC.h"
#include "../../DataReader/SHN/GBTaxRate.h"
#include "../../DataReader/SHN/LCGroupRate.h"
#include "../../DataReader/SHN/LCReward.h"
#include <vector>
#include <string>

namespace fiesta {

class GambleHouseEngine {
public:
    static GambleHouseEngine& Get();
    void Bind();

    // Per-SHN accessors
    DiceDividindShn& diceDividind() { return DiceDividindShn::Get(); }
    DiceGameShn& diceGame() { return DiceGameShn::Get(); }
    DiceRateShn& diceRate() { return DiceRateShn::Get(); }
    GBBanTimeShn& gBBanTime() { return GBBanTimeShn::Get(); }
    GBDiceDividindShn& gBDiceDividind() { return GBDiceDividindShn::Get(); }
    GBDiceGameShn& gBDiceGame() { return GBDiceGameShn::Get(); }
    GBDiceRateShn& gBDiceRate() { return GBDiceRateShn::Get(); }
    GBEventCodeShn& gBEventCode() { return GBEventCodeShn::Get(); }
    GBExchangeMaxCoinShn& gBExchangeMaxCoin() { return GBExchangeMaxCoinShn::Get(); }
    GBHouseShn& gBHouse() { return GBHouseShn::Get(); }
    GBJoinGameMemberShn& gBJoinGameMember() { return GBJoinGameMemberShn::Get(); }
    GBRewardShn& gBReward() { return GBRewardShn::Get(); }
    GBSMAllShn& gBSMAll() { return GBSMAllShn::Get(); }
    GBSMBetCoinShn& gBSMBetCoin() { return GBSMBetCoinShn::Get(); }
    GBSMCardRateShn& gBSMCardRate() { return GBSMCardRateShn::Get(); }
    GBSMCenterShn& gBSMCenter() { return GBSMCenterShn::Get(); }
    GBSMGroupShn& gBSMGroup() { return GBSMGroupShn::Get(); }
    GBSMJPRateShn& gBSMJPRate() { return GBSMJPRateShn::Get(); }
    GBSMLineShn& gBSMLine() { return GBSMLineShn::Get(); }
    GBSMNPCShn& gBSMNPC() { return GBSMNPCShn::Get(); }
    GBTaxRateShn& gBTaxRate() { return GBTaxRateShn::Get(); }
    LCGroupRateShn& lCGroupRate() { return LCGroupRateShn::Get(); }
    LCRewardShn& lCReward() { return LCRewardShn::Get(); }

private:
    GambleHouseEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_GAMBLEHOUSEENGINE_H
