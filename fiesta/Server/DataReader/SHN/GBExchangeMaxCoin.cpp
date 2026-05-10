// Server/DataReader/SHN/GBExchangeMaxCoin.cpp
// Auto-generated: one-file-per-SHN split for GBExchangeMaxCoin.shn
#include "GBExchangeMaxCoin.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBExchangeMaxCoinShn& GBExchangeMaxCoinShn::Get() { static GBExchangeMaxCoinShn s; return s; }

void GBExchangeMaxCoinShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBExchangeMaxCoin");
    if (!t) { SHINELOG_WARN("GBExchangeMaxCoin.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBExchangeMaxCoinRow rec;
        rec.uiLevel = (uint8)ShnGetU32(*t, _r, "Level");
        rec.uiExchangeMaxCoin = ShnGetU32(*t, _r, "ExchangeMaxCoin");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBExchangeMaxCoin.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
