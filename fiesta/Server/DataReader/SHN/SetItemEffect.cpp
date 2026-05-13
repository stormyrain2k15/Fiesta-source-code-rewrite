// Server/DataReader/SHN/SetItemEffect.cpp
// Auto-generated: one-file-per-SHN split for SetItemEffect.shn
#include "SetItemEffect.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

SetItemEffectShn& SetItemEffectShn::Get() { static SetItemEffectShn s; return s; }

void SetItemEffectShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("SetItemEffect");
    if (!t) { SHINELOG_WARN("SetItemEffect.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        SetItemEffectRow rec;
        rec.kEffect = ShnGetStr(*t, _r, "Effect");
        rec.kDesc = ShnGetStr(*t, _r, "Desc");
        rec.uiUseSubject = (uint8)ShnGetU32(*t, _r, "UseSubject");
        rec.kSkillGroup = ShnGetStr(*t, _r, "SkillGroup");
        rec.kFrom = ShnGetStr(*t, _r, "From");
        rec.kTo = ShnGetStr(*t, _r, "To");
        rec.uiIndex = ShnGetU32(*t, _r, "Index");
        rec.uiArgument = ShnGetU32(*t, _r, "Argument");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("SetItemEffect.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
