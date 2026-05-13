// Server/DataReader/SHN/GBEventCode.cpp
// Auto-generated: one-file-per-SHN split for GBEventCode.shn
#include "GBEventCode.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBEventCodeShn& GBEventCodeShn::Get() { static GBEventCodeShn s; return s; }

void GBEventCodeShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBEventCode");
    if (!t) { SHINELOG_WARN("GBEventCode.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBEventCodeRow rec;
        rec.uiGameType = ShnGetU32(*t, _r, "GameType");
        rec.uiGB_ANI = ShnGetU32(*t, _r, "GB_ANI");
        rec.uiGB_ECode = ShnGetU32(*t, _r, "GB_ECode");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBEventCode.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
