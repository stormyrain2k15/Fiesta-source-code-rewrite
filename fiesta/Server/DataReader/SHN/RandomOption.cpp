// Server/DataReader/SHN/RandomOption.cpp
// Auto-generated: one-file-per-SHN split for RandomOption.shn
#include "RandomOption.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

RandomOptionShn& RandomOptionShn::Get() { static RandomOptionShn s; return s; }

void RandomOptionShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("RandomOption");
    if (!t) { SHINELOG_WARN("RandomOption.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        RandomOptionRow rec;
        rec.kDropItemIndex = ShnGetStr(*t, _r, "DropItemIndex");
        rec.uiRandomOptionType = ShnGetU32(*t, _r, "RandomOptionType");
        rec.uiMin = ShnGetU32(*t, _r, "Min");
        rec.uiMax = ShnGetU32(*t, _r, "Max");
        rec.uiTypeDropRate = ShnGetU32(*t, _r, "TypeDropRate");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("RandomOption.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
