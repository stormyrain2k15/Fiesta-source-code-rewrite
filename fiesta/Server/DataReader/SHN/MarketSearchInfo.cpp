// Server/DataReader/SHN/MarketSearchInfo.cpp
// Auto-generated: one-file-per-SHN split for MarketSearchInfo.shn
#include "MarketSearchInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MarketSearchInfoShn& MarketSearchInfoShn::Get() { static MarketSearchInfoShn s; return s; }

void MarketSearchInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MarketSearchInfo");
    if (!t) { SHINELOG_WARN("MarketSearchInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MarketSearchInfoRow rec;
        rec.uiAuctionSubType = ShnGetU32(*t, _r, "AuctionSubType");
        rec.uiMarketSearchType = ShnGetU32(*t, _r, "MarketSearchType");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MarketSearchInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
