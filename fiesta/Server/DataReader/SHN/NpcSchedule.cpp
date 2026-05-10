// Server/DataReader/SHN/NpcSchedule.cpp
// Auto-generated: one-file-per-SHN split for NpcSchedule.shn
#include "NpcSchedule.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

NpcScheduleShn& NpcScheduleShn::Get() { static NpcScheduleShn s; return s; }

void NpcScheduleShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("NpcSchedule");
    if (!t) { SHINELOG_WARN("NpcSchedule.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        NpcScheduleRow rec;
        rec.kMob_Inx = ShnGetStr(*t, _r, "Mob_Inx");
        rec.uiNS_Year = (uint16)ShnGetU32(*t, _r, "NS_Year");
        rec.uiNS_Month = (uint8)ShnGetU32(*t, _r, "NS_Month");
        rec.uiNS_Day = (uint8)ShnGetU32(*t, _r, "NS_Day");
        rec.uiNS_Hour = (uint8)ShnGetU32(*t, _r, "NS_Hour");
        rec.uiNS_Minute = (uint8)ShnGetU32(*t, _r, "NS_Minute");
        rec.uiNS_CycleHour = (uint16)ShnGetU32(*t, _r, "NS_CycleHour");
        rec.uiNS_LifeHour = (uint8)ShnGetU32(*t, _r, "NS_LifeHour");
        rec.uiNS_IsMsg = (uint8)ShnGetU32(*t, _r, "NS_IsMsg");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("NpcSchedule.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
