// Server/Zone/GambleHouse/SlotMachine/GBSMNPC.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace fiesta {
GBSMNPCTable& GBSMNPCTable::Get() { static GBSMNPCTable s; return s; }
bool GBSMNPCTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSM_MobInx, GBSMBet
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMNPC");
    if (!t) { SHINELOG_WARN("GBSMNPC.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        std::string inx = ShnGetStr(*t, i, "GBSM_MobInx");
        uint32      bet = ShnGetU32(*t, i, "GBSMBet");
        if (inx.empty()) continue;
        m_kRows[inx] = bet;
    }
    SHINELOG_INFO("GBSMNPC: %u machines", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
int32 GBSMNPCTable::BetTierFor(const std::string& rInx) const {
    std::map<std::string, uint32>::const_iterator it = m_kRows.find(rInx);
    return (it == m_kRows.end()) ? -1 : (int32)it->second;
}
} // namespace fiesta
