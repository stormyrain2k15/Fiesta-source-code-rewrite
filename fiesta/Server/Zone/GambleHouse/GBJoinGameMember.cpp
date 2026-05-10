// Server/Zone/GambleHouse/GBJoinGameMember.cpp
// FEATURE: casino-seatcap
#include "GBAuxTables.h"
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"
namespace fiesta {
GBJoinGameMemberTable& GBJoinGameMemberTable::Get() {
    static GBJoinGameMemberTable s; return s;
}
bool GBJoinGameMemberTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-seatcap -- column read: GameType, MinJoinMember,
    // MaxJoinMember
    const ShnFile* t = ShnRegistry::Get().GetTable("GBJoinGameMember");
    if (!t) { SHINELOG_WARN("GBJoinGameMember.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint32   gt = ShnGetU32(*t, i, "GameType");
        GBJoinRow r;
        r.uiMin = (uint8)ShnGetU32(*t, i, "MinJoinMember");
        r.uiMax = (uint8)ShnGetU32(*t, i, "MaxJoinMember");
        m_kRows[gt] = r;
    }
    SHINELOG_INFO("GBJoinGameMember: %u games", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
bool GBJoinGameMemberTable::Find(uint32 uiGT, GBJoinRow& rOut) const {
    std::map<uint32, GBJoinRow>::const_iterator it = m_kRows.find(uiGT);
    if (it == m_kRows.end()) return false;
    rOut = it->second; return true;
}
} // namespace fiesta
