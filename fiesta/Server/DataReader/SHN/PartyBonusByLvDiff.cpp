// Server/DataReader/SHN/PartyBonusByLvDiff.cpp
// Auto-generated: one-file-per-SHN split for PartyBonusByLvDiff.shn
#include "PartyBonusByLvDiff.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

PartyBonusByLvDiffShn& PartyBonusByLvDiffShn::Get() { static PartyBonusByLvDiffShn s; return s; }

void PartyBonusByLvDiffShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PartyBonusByLvDiff");
    if (!t) { SHINELOG_WARN("PartyBonusByLvDiff.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PartyBonusByLvDiffRow rec;
        rec.uiPB_LvDiff = (uint16)ShnGetU32(*t, _r, "PB_LvDiff");
        rec.uiPB_BonusRatio = (uint16)ShnGetU32(*t, _r, "PB_BonusRatio");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PartyBonusByLvDiff.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
