// Server/DataReader/SHN/GBBanTime.cpp
// Auto-generated: one-file-per-SHN split for GBBanTime.shn
#include "GBBanTime.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBBanTimeShn& GBBanTimeShn::Get() { static GBBanTimeShn s; return s; }

void GBBanTimeShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBBanTime");
    if (!t) { SHINELOG_WARN("GBBanTime.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBBanTimeRow rec;
        rec.uiGameType = ShnGetU32(*t, _r, "GameType");
        rec.uiGB_Ban = ShnGetU32(*t, _r, "GB_Ban");
        rec.uiGB_BanTime = ShnGetU32(*t, _r, "GB_BanTime");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBBanTime.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
