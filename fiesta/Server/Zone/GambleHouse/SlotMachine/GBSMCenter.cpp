// Server/Zone/GambleHouse/SlotMachine/GBSMCenter.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace shine {
GBSMCenterTable& GBSMCenterTable::Get() { static GBSMCenterTable s; return s; }
bool GBSMCenterTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSM_GroupID, GBSM_RatioCenter
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMCenter");
    if (!t) { SHINELOG_WARN("GBSMCenter.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint8  gid = (uint8) ShnGetU32(*t, i, "GBSM_GroupID");
        uint16 rc  = (uint16)ShnGetU32(*t, i, "GBSM_RatioCenter");
        m_kRows[gid] = rc;
    }
    SHINELOG_INFO("GBSMCenter: %u groups", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
uint16 GBSMCenterTable::RatioFor(uint8 uiGroupID) const {
    std::map<uint8, uint16>::const_iterator it = m_kRows.find(uiGroupID);
    return (it == m_kRows.end()) ? 0 : it->second;
}
} // namespace shine
