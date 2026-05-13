// Server/Zone/GambleHouse/SlotMachine/GBSMCardRate.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace shine {
GBSMCardRateTable& GBSMCardRateTable::Get() { static GBSMCardRateTable s; return s; }
bool GBSMCardRateTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSMCard, GBSM_CardRate
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMCardRate");
    if (!t) { SHINELOG_WARN("GBSMCardRate.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint32 card = ShnGetU32(*t, i, "GBSMCard");
        uint16 rate = (uint16)ShnGetU32(*t, i, "GBSM_CardRate");
        m_kRows[card] = rate;
    }
    SHINELOG_INFO("GBSMCardRate: %u symbols", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
uint16 GBSMCardRateTable::WeightFor(uint32 uiCard) const {
    std::map<uint32, uint16>::const_iterator it = m_kRows.find(uiCard);
    return (it == m_kRows.end()) ? 0 : it->second;
}
} // namespace shine
