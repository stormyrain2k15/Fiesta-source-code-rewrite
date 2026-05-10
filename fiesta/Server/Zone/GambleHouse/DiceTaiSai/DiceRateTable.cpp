// Server/Zone/GambleHouse/DiceTaiSai/DiceRateTable.cpp
// FEATURE: casino-dice
#include "DiceRateTable.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"

namespace fiesta {

DiceRateTable& DiceRateTable::Get() { static DiceRateTable s; return s; }

bool DiceRateTable::Bind() {
    m_kRates.clear();
    m_uiTotal = 0;
    // FEATURE: casino-dice -- column read: Rate
    const ShnFile* t = ShnRegistry::Get().GetTable("DiceRate");
    if (!t) { SHINELOG_WARN("DiceRate.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint32 r = ShnGetU32(*t, i, "Rate");
        m_kRates.push_back(r);
        m_uiTotal += r;
    }
    SHINELOG_INFO("DiceRate: %u rows total=%u",
                  (uint32)m_kRates.size(), m_uiTotal);
    return !m_kRates.empty();
}

int32 DiceRateTable::PickIndex(uint32 uiRoll) const {
    uint32 acc = 0;
    for (size_t i = 0; i < m_kRates.size(); ++i) {
        acc += m_kRates[i];
        if (uiRoll < acc) return (int32)i;
    }
    return -1;
}

} // namespace fiesta
