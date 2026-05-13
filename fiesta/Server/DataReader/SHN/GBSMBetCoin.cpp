// Server/DataReader/SHN/GBSMBetCoin.cpp
// Auto-generated: one-file-per-SHN split for GBSMBetCoin.shn
#include "GBSMBetCoin.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBSMBetCoinShn& GBSMBetCoinShn::Get() { static GBSMBetCoinShn s; return s; }

void GBSMBetCoinShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMBetCoin");
    if (!t) { SHINELOG_WARN("GBSMBetCoin.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBSMBetCoinRow rec;
        rec.uiGBSMBet = ShnGetU32(*t, _r, "GBSMBet");
        rec.uiGBSMCoin = (uint16)ShnGetU32(*t, _r, "GBSMCoin");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBSMBetCoin.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
