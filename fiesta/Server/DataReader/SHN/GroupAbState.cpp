// Server/DataReader/SHN/GroupAbState.cpp
// Auto-generated: one-file-per-SHN split for GroupAbState.shn
#include "GroupAbState.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GroupAbStateShn& GroupAbStateShn::Get() { static GroupAbStateShn s; return s; }

void GroupAbStateShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GroupAbState");
    if (!t) { SHINELOG_WARN("GroupAbState.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GroupAbStateRow rec;
        rec.kGroupAbStateIDX = ShnGetStr(*t, _r, "GroupAbStateIDX");
        rec.kAbstateIDX = ShnGetStr(*t, _r, "AbstateIDX");
        rec.uiStrength = (uint8)ShnGetU32(*t, _r, "Strength");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GroupAbState.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
