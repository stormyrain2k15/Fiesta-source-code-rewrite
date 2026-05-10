// Server/DataReader/SHN/StateItem.cpp
// Auto-generated: one-file-per-SHN split for StateItem.shn
#include "StateItem.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

StateItemShn& StateItemShn::Get() { static StateItemShn s; return s; }

void StateItemShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("StateItem");
    if (!t) { SHINELOG_WARN("StateItem.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        StateItemRow rec;
        rec.kItemIDX = ShnGetStr(*t, _r, "ItemIDX");
        rec.kAbStateInx = ShnGetStr(*t, _r, "AbStateInx");
        rec.uiStaStrength = ShnGetU32(*t, _r, "StaStrength");
        rec.uiStaSucRate = (uint16)ShnGetU32(*t, _r, "StaSucRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("StateItem.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
