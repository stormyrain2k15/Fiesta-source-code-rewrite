// Server/DataReader/SHN/MoverHG.cpp
// Auto-generated: one-file-per-SHN split for MoverHG.shn
#include "MoverHG.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MoverHGShn& MoverHGShn::Get() { static MoverHGShn s; return s; }

void MoverHGShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MoverHG");
    if (!t) { SHINELOG_WARN("MoverHG.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MoverHGRow rec;
        rec.kMoverIDX = ShnGetStr(*t, _r, "MoverIDX");
        rec.kFeedType = ShnGetStr(*t, _r, "FeedType");
        rec.uiRestoreAmount = (uint16)ShnGetU32(*t, _r, "RestoreAmount");
        rec.uiMaxHG = (uint16)ShnGetU32(*t, _r, "MaxHG");
        rec.uiCreateHG = (uint16)ShnGetU32(*t, _r, "CreateHG");
        rec.uiTick = (uint16)ShnGetU32(*t, _r, "Tick");
        rec.uiConsumeHG = (uint16)ShnGetU32(*t, _r, "ConsumeHG");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MoverHG.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
