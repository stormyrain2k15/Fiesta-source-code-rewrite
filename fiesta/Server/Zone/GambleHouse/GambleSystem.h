// Server/Zone/GambleHouse/GambleSystem.h
// FEATURE: casino -- orchestrator. Pure compute layer; owns no SHN
// table directly. Delegates ingest to the per-SHN binders in
// GBHouseTable / GBExchangeMaxCoinTable / GBTaxRateTable / GBAuxTables /
// DiceTaiSai/* / SlotMachine/*.
#ifndef FIESTA_ZONE_GAMBLEHOUSE_GAMBLESYSTEM_H
#define FIESTA_ZONE_GAMBLEHOUSE_GAMBLESYSTEM_H
#include "../../../Shared/ShineTypes.h"

namespace fiesta {
class ShinePlayer;

// Game-type ids (matching GBSM/GBReward/GBJoinGameMember/GBTaxRate
// rows). Confirmed by NA2016 sample data: 23 == dice-bowl,
// 26 == slot-machine. Other values are reserved.
const uint32 kGameType_Dice = 23;
const uint32 kGameType_Slot = 26;

class GambleSystem {
public:
    static GambleSystem& Get();

    // Boot-time wiring -- runs every per-SHN Bind() in the casino
    // family. Single call from Zone Main.cpp.
    bool BindAllCasinoTables();

    // -------- Currency exchange (gold <-> Lucky Coin chips) --------
    // Returns the chip count granted (may be capped by GBExchangeMaxCoin);
    // deducts gold from the player's inventory accordingly. 0 on fail.
    uint32 ExchangeGoldToCoin(ShinePlayer* pk, uint64 uiGold,
                              uint8 uiExchLevel) const;

    // Returns the gold count credited (post-tax). 0 on fail.
    uint64 ExchangeCoinToGold(ShinePlayer* pk, uint32 uiChips) const;

    // -------- Dice (Tai Sai bowl) --------
    // Resolve one full bet/roll/payout cycle. Returns the payout in
    // chips. Bet validation, min/max bracket and house edge consulted
    // from DiceGameTable + DiceRateTable + DiceDividindTable.
    int32 ResolveDice(ShinePlayer* pk, uint16 uiBowlItemID,
                      uint32 uiBetChips, uint8 uiBetTier) const;

    // -------- Slot machine --------
    // Spin one machine for the given player. Returns the chip payout
    // (0 = no win). The roll computes a 3-symbol reel result against
    // GBSMCardRate weights, scores it against GBSMLine/Center/All
    // group rules, and applies the GBSMJPRate bump if the JP pool
    // is hot.
    int32 ResolveSlot(ShinePlayer* pk,
                      const std::string& rNpcInx,
                      uint32 uiPoolSize) const;

private:
    GambleSystem() {}
};

} // namespace fiesta
#endif
