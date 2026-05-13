// Server/Zone/GambleHouse/SlotMachine/GBSMAll.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace shine {
GBSMAllTable& GBSMAllTable::Get() { static GBSMAllTable s; return s; }
bool GBSMAllTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSM_GroupID, GBSM_Num,
    // GBSM_RatioAll, GBSM_IsJP
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMAll");
    if (!t) { SHINELOG_WARN("GBSMAll.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        LegacyGBSMAllRow r;
        r.uiGroupID  = (uint8) ShnGetU32(*t, i, "GBSM_GroupID");
        r.uiNum      = (uint8) ShnGetU32(*t, i, "GBSM_Num");
        r.uiRatioAll = (uint16)ShnGetU32(*t, i, "GBSM_RatioAll");
        r.bIsJP      = (uint8) ShnGetU32(*t, i, "GBSM_IsJP");
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("GBSMAll: %u rows", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
} // namespace shine
