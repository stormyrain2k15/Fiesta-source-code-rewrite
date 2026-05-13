// Server/DataReader/SHN/ReactionType.cpp
// Auto-generated: one-file-per-SHN split for ReactionType.shn
#include "ReactionType.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ReactionTypeShn& ReactionTypeShn::Get() { static ReactionTypeShn s; return s; }

void ReactionTypeShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ReactionType");
    if (!t) { SHINELOG_WARN("ReactionType.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ReactionTypeRow rec;
        rec.uiRAType = ShnGetU32(*t, _r, "RAType");
        rec.kMobInx = ShnGetStr(*t, _r, "MobInx");
        rec.uiEcode = ShnGetU32(*t, _r, "Ecode");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ReactionType.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
