// Server/DataReader/SHN/GBHouse.cpp
// Auto-generated: one-file-per-SHN split for GBHouse.shn
#include "GBHouse.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GBHouseShn& GBHouseShn::Get() { static GBHouseShn s; return s; }

void GBHouseShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBHouse");
    if (!t) { SHINELOG_WARN("GBHouse.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBHouseRow rec;
        rec.uiGB_GameMoney = ShnGetU32(*t, _r, "GB_GameMoney");
        rec.uiGB_ExchangeTax = ShnGetU32(*t, _r, "GB_ExchangeTax");
        rec.uiGB_ResetTimeHour = (uint8)ShnGetU32(*t, _r, "GB_ResetTimeHour");
        rec.uiGB_ResetTimeMin = (uint8)ShnGetU32(*t, _r, "GB_ResetTimeMin");
        rec.uiGB_ResetTimeSec = (uint8)ShnGetU32(*t, _r, "GB_ResetTimeSec");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBHouse.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
