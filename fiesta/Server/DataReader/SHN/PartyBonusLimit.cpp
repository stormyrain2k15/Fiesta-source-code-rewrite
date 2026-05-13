// Server/DataReader/SHN/PartyBonusLimit.cpp
// Auto-generated: one-file-per-SHN split for PartyBonusLimit.shn
#include "PartyBonusLimit.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

PartyBonusLimitShn& PartyBonusLimitShn::Get() { static PartyBonusLimitShn s; return s; }

void PartyBonusLimitShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PartyBonusLimit");
    if (!t) { SHINELOG_WARN("PartyBonusLimit.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PartyBonusLimitRow rec;
        rec.uiPSE_ChrLv = (uint8)ShnGetU32(*t, _r, "PSE_ChrLv");
        rec.uiPSE_ExpLimit = ShnGetU32(*t, _r, "PSE_ExpLimit");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PartyBonusLimit.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
