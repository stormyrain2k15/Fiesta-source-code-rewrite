// Server/DataReader/SHN/PartyBonusByMember.cpp
// Auto-generated: one-file-per-SHN split for PartyBonusByMember.shn
#include "PartyBonusByMember.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PartyBonusByMemberShn& PartyBonusByMemberShn::Get() { static PartyBonusByMemberShn s; return s; }

void PartyBonusByMemberShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PartyBonusByMember");
    if (!t) { SHINELOG_WARN("PartyBonusByMember.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PartyBonusByMemberRow rec;
        rec.uiPB_PartyMember = (uint8)ShnGetU32(*t, _r, "PB_PartyMember");
        rec.uiPB_BonusRatio = (uint16)ShnGetU32(*t, _r, "PB_BonusRatio");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PartyBonusByMember.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
