// Server/DataReader/SHN/PupPriority.cpp
// Auto-generated: one-file-per-SHN split for PupPriority.shn
#include "PupPriority.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

PupPriorityShn& PupPriorityShn::Get() { static PupPriorityShn s; return s; }

void PupPriorityShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PupPriority");
    if (!t) { SHINELOG_WARN("PupPriority.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PupPriorityRow rec;
        rec.uiPupPriorityType = ShnGetU32(*t, _r, "PupPriorityType");
        rec.uiPriorityNum = (uint8)ShnGetU32(*t, _r, "PriorityNum");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PupPriority.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
