// Server/Zone/GambleHouse/SlotMachine/GBSMBetCoin.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace fiesta {
GBSMBetCoinTable& GBSMBetCoinTable::Get() { static GBSMBetCoinTable s; return s; }
bool GBSMBetCoinTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSMBet, GBSMCoin
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMBetCoin");
    if (!t) { SHINELOG_WARN("GBSMBetCoin.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint32 bet  = ShnGetU32(*t, i, "GBSMBet");
        uint16 coin = (uint16)ShnGetU32(*t, i, "GBSMCoin");
        m_kRows[bet] = coin;
    }
    SHINELOG_INFO("GBSMBetCoin: %u tiers", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
uint16 GBSMBetCoinTable::ChipCost(uint32 uiBet) const {
    std::map<uint32, uint16>::const_iterator it = m_kRows.find(uiBet);
    return (it == m_kRows.end()) ? 0 : it->second;
}
} // namespace fiesta
