// Server/DataReader/SHN/PupCaseDesc.cpp
// Auto-generated: one-file-per-SHN split for PupCaseDesc.shn
#include "PupCaseDesc.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

PupCaseDescShn& PupCaseDescShn::Get() { static PupCaseDescShn s; return s; }

void PupCaseDescShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PupCaseDesc");
    if (!t) { SHINELOG_WARN("PupCaseDesc.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PupCaseDescRow rec;
        rec.uiPupPriorityType = ShnGetU32(*t, _r, "PupPriorityType");
        rec.uiPupCaseType = ShnGetU32(*t, _r, "PupCaseType");
        rec.kPupIDX = ShnGetStr(*t, _r, "PupIDX");
        rec.uiPupAIType = ShnGetU32(*t, _r, "PupAIType");
        rec.kSM_Inx = ShnGetStr(*t, _r, "SM_Inx");
        rec.uiActionEffectID = ShnGetU32(*t, _r, "ActionEffectID");
        rec.kSoundFile = ShnGetStr(*t, _r, "SoundFile");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PupCaseDesc.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
