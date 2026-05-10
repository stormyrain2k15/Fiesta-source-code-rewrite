// Server/DataReader/SHN/GBJoinGameMember.cpp
// Auto-generated: one-file-per-SHN split for GBJoinGameMember.shn
#include "GBJoinGameMember.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBJoinGameMemberShn& GBJoinGameMemberShn::Get() { static GBJoinGameMemberShn s; return s; }

void GBJoinGameMemberShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBJoinGameMember");
    if (!t) { SHINELOG_WARN("GBJoinGameMember.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBJoinGameMemberRow rec;
        rec.uiGameType = ShnGetU32(*t, _r, "GameType");
        rec.uiMinJoinMember = (uint8)ShnGetU32(*t, _r, "MinJoinMember");
        rec.uiMaxJoinMember = (uint8)ShnGetU32(*t, _r, "MaxJoinMember");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBJoinGameMember.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
