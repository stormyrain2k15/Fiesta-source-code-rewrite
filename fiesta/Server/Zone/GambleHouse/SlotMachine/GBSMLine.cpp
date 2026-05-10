// Server/Zone/GambleHouse/SlotMachine/GBSMLine.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace fiesta {
GBSMLineTable& GBSMLineTable::Get() { static GBSMLineTable s; return s; }
bool GBSMLineTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSM_GroupID, GBSM_Num, GBSM_RatioLine
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMLine");
    if (!t) { SHINELOG_WARN("GBSMLine.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        GBSMLineRow r;
        r.uiGroupID   = (uint8) ShnGetU32(*t, i, "GBSM_GroupID");
        r.uiNum       = (uint8) ShnGetU32(*t, i, "GBSM_Num");
        r.uiRatioLine = (uint16)ShnGetU32(*t, i, "GBSM_RatioLine");
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("GBSMLine: %u line-tiers", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
} // namespace fiesta
