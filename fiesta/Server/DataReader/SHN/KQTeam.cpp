// Server/DataReader/SHN/KQTeam.cpp
// Auto-generated: one-file-per-SHN split for KQTeam.shn
#include "KQTeam.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

KQTeamShn& KQTeamShn::Get() { static KQTeamShn s; return s; }

void KQTeamShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("KQTeam");
    if (!t) { SHINELOG_WARN("KQTeam.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        KQTeamRow rec;
        rec.iID = (int16)ShnGetI32(*t, _r, "ID");
        rec.uiMaxMemberGap = (uint8)ShnGetU32(*t, _r, "MaxMemberGap");
        rec.uiIsTeamPVP = (uint8)ShnGetU32(*t, _r, "IsTeamPVP");
        rec.iKQTeamDivideType = (int16)ShnGetI32(*t, _r, "KQTeamDivideType");
        rec.uiRegenXRed = ShnGetU32(*t, _r, "RegenXRed");
        rec.uiRegenYRed = ShnGetU32(*t, _r, "RegenYRed");
        rec.uiRegenXBlue = ShnGetU32(*t, _r, "RegenXBlue");
        rec.uiRegenYBlue = ShnGetU32(*t, _r, "RegenYBlue");
        m_kById[rec.iID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("KQTeam.shn: %u rows", (uint32)m_kRows.size());
}

const KQTeamRow* KQTeamShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
