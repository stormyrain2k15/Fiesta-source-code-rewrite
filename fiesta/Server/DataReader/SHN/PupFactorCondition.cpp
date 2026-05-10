// Server/DataReader/SHN/PupFactorCondition.cpp
// Auto-generated: one-file-per-SHN split for PupFactorCondition.shn
#include "PupFactorCondition.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PupFactorConditionShn& PupFactorConditionShn::Get() { static PupFactorConditionShn s; return s; }

void PupFactorConditionShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PupFactorCondition");
    if (!t) { SHINELOG_WARN("PupFactorCondition.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PupFactorConditionRow rec;
        rec.uiPupMindType = ShnGetU32(*t, _r, "PupMindType");
        rec.uiPupFactorConditionType = ShnGetU32(*t, _r, "PupFactorConditionType");
        rec.uiPupFactorType = ShnGetU32(*t, _r, "PupFactorType");
        rec.uiIsMinus = (uint8)ShnGetU32(*t, _r, "IsMinus");
        rec.uiValue = ShnGetU32(*t, _r, "Value");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PupFactorCondition.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
