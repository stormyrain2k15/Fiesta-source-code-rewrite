// Server/Zone/GambleHouse/GambleSystem.cpp
// FEATURE: casino -- orchestrator (compute only).
#include "GambleSystem.h"
#include "GBHouseTable.h"
#include "GBExchangeMaxCoinTable.h"
#include "GBTaxRateTable.h"
#include "GBAuxTables.h"
#include "DiceTaiSai/DiceGameTable.h"
#include "DiceTaiSai/DiceRateTable.h"
#include "DiceTaiSai/DiceDividindTable.h"
#include "SlotMachine/GBSMTables.h"
#include "../ShineObject.h"
#include "../Inventory.h"
#include "../../Shared/well512.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

static well512 s_kRng;

GambleSystem& GambleSystem::Get() { static GambleSystem s; return s; }

bool GambleSystem::BindAllCasinoTables() {
    // Order matters only loosely -- each binder looks up its own SHN.
    bool any = false;
    any |= GBHouseTable::Get()           .Bind();
    any |= GBExchangeMaxCoinTable::Get() .Bind();
    any |= GBTaxRateTable::Get()         .Bind();
    any |= GBRewardTable::Get()          .Bind();
    any |= GBBanTimeTable::Get()         .Bind();
    any |= GBJoinGameMemberTable::Get()  .Bind();
    any |= GBEventCodeTable::Get()       .Bind();
    any |= DiceGameTable::Get()          .Bind();
    any |= DiceRateTable::Get()          .Bind();
    any |= DiceDividindTable::Get()      .Bind();
    any |= GBSMAllTable::Get()           .Bind();
    any |= GBSMBetCoinTable::Get()       .Bind();
    any |= GBSMCardRateTable::Get()      .Bind();
    any |= GBSMCenterTable::Get()        .Bind();
    any |= GBSMGroupTable::Get()         .Bind();
    any |= GBSMJPRateTable::Get()        .Bind();
    any |= GBSMLineTable::Get()          .Bind();
    any |= GBSMNPCTable::Get()           .Bind();
    SHINELOG_INFO("GambleSystem: all casino tables bound (%s)",
                  any ? "ok" : "all-empty");
    return any;
}

// ---------------------------------------------------------------------------
// Currency exchange
// ---------------------------------------------------------------------------
uint32 GambleSystem::ExchangeGoldToCoin(ShinePlayer* pk, uint64 uiGold,
                                        uint8 uiExchLv) const
{
    if (!pk || uiGold == 0) return 0;
    const LegacyGBHouseRow& h = GBHouseTable::Get().Row();
    uint32 rate = h.uiGameMoney ? h.uiGameMoney : 10;
    // Chips = floor(gold / rate). Cap by GBExchangeMaxCoin.
    uint64 chips = uiGold / rate;
    uint32 cap   = GBExchangeMaxCoinTable::Get().Cap(uiExchLv);
    if (cap > 0 && chips > (uint64)cap) chips = cap;
    if (chips == 0) return 0;
    uint64 spend = chips * (uint64)rate;
    if ((int64)spend > pk->Inv().Money()) return 0;
    pk->Inv().AddMoney(-(int64)spend);
    SHINELOG_INFO("Casino exchange G->C: cid=%u %llu gold -> %u chips "
                  "(rate=%u cap=%u)",
                  pk->GetCharID(), spend, (uint32)chips, rate, cap);
    return (uint32)chips;
}

uint64 GambleSystem::ExchangeCoinToGold(ShinePlayer* pk, uint32 uiChips) const {
    if (!pk || uiChips == 0) return 0;
    const LegacyGBHouseRow& h = GBHouseTable::Get().Row();
    uint32 rate = h.uiGameMoney ? h.uiGameMoney : 10;
    uint64 gross = (uint64)uiChips * (uint64)rate;
    uint64 tax   = (gross * (uint64)h.uiExchangeTax) / 100ULL;
    uint64 net   = gross - tax;
    pk->Inv().AddMoney((int64)net);
    SHINELOG_INFO("Casino exchange C->G: cid=%u %u chips -> %llu gold "
                  "(tax=%u%% gross=%llu)",
                  pk->GetCharID(), uiChips, net, h.uiExchangeTax, gross);
    return net;
}

// ---------------------------------------------------------------------------
// Dice (Tai Sai bowl) resolve
// ---------------------------------------------------------------------------
int32 GambleSystem::ResolveDice(ShinePlayer* pk, uint16 uiBowlItem,
                                uint32 uiBet, uint8 uiTier) const
{
    if (!pk) return 0;
    const LegacyDiceGameRow* pkBowl = DiceGameTable::Get().FindByItem(uiBowlItem);
    if (!pkBowl) {
        SHINELOG_WARN("Dice: bowl item %u unknown", uiBowlItem);
        return 0;
    }
    if (pk->GetLevel() < pkBowl->uiUseMinLv) return 0;
    if (uiBet < pkBowl->uiMinGetMoney || uiBet > pkBowl->uiMaxBetMoney) return 0;

    // Roll three d6 (Tai Sai = sic-bo three-dice). Total used to pick
    // a tier within the DiceDividind ladder; small/big bets resolve at
    // the bowl's GetSysRate house edge.
    uint32 d1 = (uint32)(s_kRng.NextDouble() * 6.0) + 1;
    uint32 d2 = (uint32)(s_kRng.NextDouble() * 6.0) + 1;
    uint32 d3 = (uint32)(s_kRng.NextDouble() * 6.0) + 1;
    uint32 total = d1 + d2 + d3;

    // Win/lose against bowl's system rate (per-1000).
    int32 wlroll = (int32)(s_kRng.NextDouble() * 1000.0);
    bool  bWin   = (wlroll < (int32)pkBowl->uiGetSysRate);

    if (!bWin) {
        SHINELOG_INFO("Dice: cid=%u bowl=%u dice=%u/%u/%u (sum=%u) LOSE",
                      pk->GetCharID(), uiBowlItem, d1,d2,d3, total);
        return 0;
    }

    // Pick payout tier from DiceDividind based on bet bracket; bigger
    // bets land in lower-multiplier tiers, smaller bets win bigger
    // multipliers (regressive). Tier index = (uiTier % 16).
    int32 mult = DiceDividindTable::Get().Tier(uiTier % 16);
    int64 pay  = ((int64)uiBet * (int64)mult) / 100LL;
    if (pay < 0) pay = 0;
    SHINELOG_INFO("Dice: cid=%u bowl=%u dice=%u/%u/%u WIN tier=%u mult=%d pay=%lld",
                  pk->GetCharID(), uiBowlItem, d1,d2,d3,
                  uiTier % 16, mult, pay);
    return (int32)pay;
}

// ---------------------------------------------------------------------------
// Slot machine resolve
// ---------------------------------------------------------------------------
int32 GambleSystem::ResolveSlot(ShinePlayer* pk,
                                const std::string& rNpcInx,
                                uint32 uiPool) const
{
    if (!pk) return 0;
    int32 betTier = GBSMNPCTable::Get().BetTierFor(rNpcInx);
    if (betTier < 0) {
        SHINELOG_WARN("Slot: NPC inx '%s' not bound", rNpcInx.c_str());
        return 0;
    }
    uint16 chipCost = GBSMBetCoinTable::Get().ChipCost((uint32)betTier);
    if (chipCost == 0) return 0;
    // Bet payment is the caller's responsibility (chip-side ledger).

    // Roll three reels via weighted CardRate distribution.
    // PROVISIONAL_BODY: NA2016 ships a fixed 21-symbol reel strip;
    // we treat every card present in GBSMCardRate as a symbol on the
    // strip and weight-pick. Confirm against client reel art when
    // wired to the visualization.
    uint32 picks[3] = { 0, 0, 0 };
    std::vector<std::pair<uint32, uint32> > ladder; // (cumWeight, card)
    uint32 total = 0;
    // Probe card ids 1..32 (canon NA2016 ships 1..21; pad to 32 for safety).
    for (uint32 c = 1; c <= 32; ++c) {
        uint16 w = GBSMCardRateTable::Get().WeightFor(c);
        if (w == 0) continue;
        total += w;
        ladder.push_back(std::make_pair(total, c));
    }
    if (total == 0 || ladder.empty()) return 0;
    for (int i = 0; i < 3; ++i) {
        uint32 r = (uint32)(s_kRng.NextDouble() * (double)total);
        for (size_t j = 0; j < ladder.size(); ++j) {
            if (r < ladder[j].first) { picks[i] = ladder[j].second; break; }
        }
    }

    // Check if the three reels share any group.
    uint8  matchGroup = 0;
    bool   bMatch     = false;
    {
        std::vector<uint8> g0, g1, g2;
        GBSMGroupTable::Get().GroupsFor(picks[0], g0);
        GBSMGroupTable::Get().GroupsFor(picks[1], g1);
        GBSMGroupTable::Get().GroupsFor(picks[2], g2);
        for (size_t a = 0; a < g0.size() && !bMatch; ++a) {
            for (size_t b = 0; b < g1.size() && !bMatch; ++b) {
                if (g0[a] != g1[b]) continue;
                for (size_t c = 0; c < g2.size() && !bMatch; ++c) {
                    if (g1[b] != g2[c]) continue;
                    matchGroup = g0[a]; bMatch = true;
                }
            }
        }
    }

    if (!bMatch) {
        SHINELOG_INFO("Slot: cid=%u npc=%s reels=%u/%u/%u NO MATCH",
                      pk->GetCharID(), rNpcInx.c_str(),
                      picks[0], picks[1], picks[2]);
        return 0;
    }

    // Score the match. GBSMAll triple-match > GBSMCenter > GBSMLine.
    uint16 ratioLine   = 0;
    uint16 ratioCenter = GBSMCenterTable::Get().RatioFor(matchGroup);
    uint16 ratioAll    = 0;
    bool   bIsJP       = false;
    {
        const std::vector<LegacyGBSMAllRow>& a = GBSMAllTable::Get().Rows();
        for (size_t i = 0; i < a.size(); ++i) {
            if (a[i].uiGroupID != matchGroup) continue;
            if (a[i].uiRatioAll > ratioAll) {
                ratioAll = a[i].uiRatioAll;
                bIsJP    = (a[i].bIsJP != 0);
            }
        }
        const std::vector<LegacyGBSMLineRow>& l = GBSMLineTable::Get().Rows();
        for (size_t i = 0; i < l.size(); ++i) {
            if (l[i].uiGroupID != matchGroup) continue;
            if (l[i].uiRatioLine > ratioLine) ratioLine = l[i].uiRatioLine;
        }
    }
    uint16 ratio = ratioAll;
    if (ratio == 0) ratio = ratioCenter;
    if (ratio == 0) ratio = ratioLine;
    if (ratio == 0) return 0;

    // Apply JP bonus when the pool is hot.
    int32 pay = (int32)chipCost * (int32)ratio / 100;
    if (bIsJP) {
        uint32 jpRate = GBSMJPRateTable::Get().RateForPool(uiPool);
        if (jpRate > 0) {
            pay = (int32)((int64)pay * (int64)jpRate / 100LL);
        }
    }
    SHINELOG_INFO("Slot: cid=%u npc=%s reels=%u/%u/%u group=%u ratio=%u "
                  "(line=%u center=%u all=%u JP=%d) pay=%d chips",
                  pk->GetCharID(), rNpcInx.c_str(),
                  picks[0], picks[1], picks[2], matchGroup, ratio,
                  ratioLine, ratioCenter, ratioAll, bIsJP ? 1 : 0, pay);
    return pay;
}

} // namespace fiesta
