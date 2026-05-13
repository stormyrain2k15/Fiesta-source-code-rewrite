// Server/Zone/GambleHouse/GBExchangeMaxCoinTable.cpp
// FEATURE: casino-exchange
#include "GBExchangeMaxCoinTable.h"
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBExchangeMaxCoinTable& GBExchangeMaxCoinTable::Get() {
    static GBExchangeMaxCoinTable s; return s;
}

bool GBExchangeMaxCoinTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-exchange -- column read: Level, ExchangeMaxCoin
    const ShnFile* t = ShnRegistry::Get().GetTable("GBExchangeMaxCoin");
    if (!t) { SHINELOG_WARN("GBExchangeMaxCoin.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint8  lv  = (uint8) ShnGetU32(*t, i, "Level");
        uint32 cap =        ShnGetU32(*t, i, "ExchangeMaxCoin");
        if (lv == 0) continue;
        m_kRows[lv] = cap;
    }
    SHINELOG_INFO("GBExchangeMaxCoin: %u tiers", (uint32)m_kRows.size());
    return !m_kRows.empty();
}

uint32 GBExchangeMaxCoinTable::Cap(uint8 uiLevel) const {
    std::map<uint8, uint32>::const_iterator it = m_kRows.find(uiLevel);
    return (it == m_kRows.end()) ? 0 : it->second;
}

} // namespace shine
