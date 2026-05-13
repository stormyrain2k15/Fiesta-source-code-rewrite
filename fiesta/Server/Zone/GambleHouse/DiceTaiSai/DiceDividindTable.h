// Server/Zone/GambleHouse/DiceTaiSai/DiceDividindTable.h
// FEATURE: casino-dice -- payout curve.
// DiceDividind.shn (16 cols of SHORT_15, single global row): a 16-tier
// payout ladder for the dice game. Tier 0 = base divide rate (per-100),
// tiers 1..15 = bet-amount-bracket multipliers. The bowl's payout at
// resolve time is `bet * multiplier(tier) / 100`. The exact tier is
// chosen by the size of the bet; smaller bets win bigger multipliers
// (regressive prize structure -- canon casino math).
//
// The misspelling "Dividind" is straight from the NA2016 file -- do
// not "fix" it; the SHN parser is name-keyed.
#ifndef SHINE_ZONE_GAMBLEHOUSE_DICETAISAI_DICEDIVIDINDTABLE_H
#define SHINE_ZONE_GAMBLEHOUSE_DICETAISAI_DICEDIVIDINDTABLE_H
#include "../../../../Shared/ShineTypes.h"

namespace shine {

class DiceDividindTable {
public:
    static DiceDividindTable& Get();
    bool   Bind();
    int32  Tier(uint8 uiTier) const;       // 0..15; clamped on miss
    bool   IsLoaded() const { return m_bLoaded; }

private:
    DiceDividindTable();
    int32 m_aTiers[16];
    bool  m_bLoaded;
};

} // namespace shine
#endif
