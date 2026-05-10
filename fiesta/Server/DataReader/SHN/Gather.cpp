// Server/DataReader/SHN/Gather.cpp
// Auto-generated: one-file-per-SHN split for Gather.shn
#include "Gather.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GatherShn& GatherShn::Get() { static GatherShn s; return s; }

void GatherShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("Gather");
    if (!t) { SHINELOG_WARN("Gather.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GatherRow rec;
        rec.uiGatherID = (uint16)ShnGetU32(*t, _r, "GatherID");
        rec.kIndex = ShnGetStr(*t, _r, "Index");
        rec.uiType = ShnGetU32(*t, _r, "Type");
        rec.kNeededTool0 = ShnGetStr(*t, _r, "NeededTool0");
        rec.kNeededTool1 = ShnGetStr(*t, _r, "NeededTool1");
        rec.kNeededTool2 = ShnGetStr(*t, _r, "NeededTool2");
        rec.kEqipItemView = ShnGetStr(*t, _r, "EqipItemView");
        rec.uiAniNumber = ShnGetU32(*t, _r, "AniNumber");
        rec.uiGauge = ShnGetU32(*t, _r, "Gauge");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("Gather.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
