// Server/Zone/Tables/KQTables.cpp
// FEATURE: world-creation -- moved from GroupTables.cpp under
// the one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
#include "BindMacros.h"
#include "../GroupTables.h"

namespace shine {

KQTables& KQTables::Get() { static KQTables s; return s; }
void KQTables::Bind() {
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuest")) {
        ITER_ROWS(t) {
            LegacyKingdomQuestRow rec;
            rec.uiID       = ShnGetU32(*t, _r, "ID");
            rec.kName      = ShnGetStr(*t, _r, "Name");
            rec.uiMinLevel = ShnGetU32(*t, _r, "MinLevel");
            rec.uiMaxLevel = ShnGetU32(*t, _r, "MaxLevel");
            rec.uiTeamSize = ShnGetU32(*t, _r, "TeamSize");
            m_kKQById[rec.uiID] = m_kKQ.size();
            m_kKQ.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestRew")) {
        ITER_ROWS(t) {
            LegacyKingdomQuestRewRow rec;
            rec.uiID    = ShnGetU32(*t, _r, "ID");
            rec.uiRank  = ShnGetU32(*t, _r, "Rank");
            rec.kReward = ShnGetStr(*t, _r, "Reward");
            rec.uiQty   = ShnGetU32(*t, _r, "Qty");
            m_kRew.push_back(rec);
        }
    }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestMap")) {
        ITER_ROWS(t) {
            LegacyKingdomQuestMapRow rec;
            rec.uiID     = ShnGetU32(*t, _r, "ID");
            rec.kMapName = ShnGetStr(*t, _r, "MapName");
            rec.uiX      = ShnGetU32(*t, _r, "X");
            rec.uiY      = ShnGetU32(*t, _r, "Y");
            m_kMapById[rec.uiID] = m_kMap.size();
            m_kMap.push_back(rec);
        }
    }
}
const LegacyKingdomQuestRow*    KQTables::FindKQ (uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kKQById.find(uiID);
    return (it == m_kKQById.end()) ? NULL : &m_kKQ[it->second]; }
const LegacyKingdomQuestRewRow* KQTables::FindRew(uint32 uiID, uint32 uiRank) const {
    for (size_t i = 0; i < m_kRew.size(); ++i)
        if (m_kRew[i].uiID == uiID && m_kRew[i].uiRank == uiRank)
            return &m_kRew[i];
    return NULL;
}
const LegacyKingdomQuestMapRow* KQTables::FindMap(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kMapById.find(uiID);
    return (it == m_kMapById.end()) ? NULL : &m_kMap[it->second]; }

// =============================================================================
//  One-call binder
// =============================================================================
void BindAllGroupTables() {
    ItemTables::Get()        .Bind();
    MobTables::Get()         .Bind();
    SkillTables::Get()       .Bind();
    MapTables::Get()         .Bind();
    AbStateTables::Get()     .Bind();
    PresentationTables::Get().Bind();
    PupTables::Get()         .Bind();
    MountTables::Get()       .Bind();
    MiniHouseTables::Get()   .Bind();
    GuildTables2::Get()      .Bind();
    CollectTables::Get()     .Bind();

} // namespace shine
