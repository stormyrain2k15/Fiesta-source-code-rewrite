// Server/Zone/GambleHouse/SlotMachine/GBSMGroup.cpp
// FEATURE: casino-slot
#include "GBSMTables.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"
namespace shine {
GBSMGroupTable& GBSMGroupTable::Get() { static GBSMGroupTable s; return s; }
bool GBSMGroupTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-slot -- column read: GBSM_GroupID, GBSMCard
    const ShnFile* t = ShnRegistry::Get().GetTable("GBSMGroup");
    if (!t) { SHINELOG_WARN("GBSMGroup.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint8  gid  = (uint8)ShnGetU32(*t, i, "GBSM_GroupID");
        uint32 card =        ShnGetU32(*t, i, "GBSMCard");
        m_kRows.push_back(std::make_pair(gid, card));
    }
    SHINELOG_INFO("GBSMGroup: %u (group,card) pairs", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
bool GBSMGroupTable::IsMember(uint8 uiGroupID, uint32 uiCard) const {
    for (size_t i = 0; i < m_kRows.size(); ++i) {
        if (m_kRows[i].first == uiGroupID && m_kRows[i].second == uiCard) return true;
    }
    return false;
}
void GBSMGroupTable::GroupsFor(uint32 uiCard, std::vector<uint8>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kRows.size(); ++i) {
        if (m_kRows[i].second == uiCard) rOut.push_back(m_kRows[i].first);
    }
}
} // namespace shine
