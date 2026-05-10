// Server/Zone/GambleHouse/SlotMachine/GBSMJPRate.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace fiesta {
GBSMJPRateTable& GBSMJPRateTable::Get() { static GBSMJPRateTable s; return s; }
bool GBSMJPRateTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSM_MinCount, GBSM_MaxCount, GBSM_JPRate
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMJPRate");
    if (!t) { SHINELOG_WARN("GBSMJPRate.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        LegacyGBSMJPRateRow r;
        r.uiMin  = ShnGetU32(*t, i, "GBSM_MinCount");
        r.uiMax  = ShnGetU32(*t, i, "GBSM_MaxCount");
        r.uiRate = ShnGetU32(*t, i, "GBSM_JPRate");
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("GBSMJPRate: %u tiers", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
uint32 GBSMJPRateTable::RateForPool(uint32 uiPool) const {
    for (size_t i = 0; i < m_kRows.size(); ++i) {
        if (uiPool >= m_kRows[i].uiMin && uiPool <= m_kRows[i].uiMax)
            return m_kRows[i].uiRate;
    }
    return 0;
}
} // namespace fiesta
