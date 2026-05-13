// Server/Zone/GambleHouse/DiceTaiSai/DiceRateTable.h
// FEATURE: casino-dice -- single-column odds modifier.
// DiceRate.shn (1 col, "Rate"): per-row weight. Sum across rows is the
// jackpot odds denominator. Same shape as a one-column lottery wheel;
// the canonical use is to compute the win/lose flag against the bowl's
// GetSysRate before consulting DiceDividind for the actual payout.
#ifndef SHINE_ZONE_GAMBLEHOUSE_DICETAISAI_DICERATETABLE_H
#define SHINE_ZONE_GAMBLEHOUSE_DICETAISAI_DICERATETABLE_H
#include "../../../../Shared/ShineTypes.h"
#include <vector>

namespace shine {

class DiceRateTable {
public:
    static DiceRateTable& Get();
    bool   Bind();
    uint32 Total() const { return m_uiTotal; }
    // Pick a row index from a uniform 0..Total()-1 roll.
    int32  PickIndex(uint32 uiRoll) const;
    size_t Count() const { return m_kRates.size(); }

private:
    DiceRateTable() : m_uiTotal(0) {}
    std::vector<uint32> m_kRates;
    uint32              m_uiTotal;
};

} // namespace shine
#endif
