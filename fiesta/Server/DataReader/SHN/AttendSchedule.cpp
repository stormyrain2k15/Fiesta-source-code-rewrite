// Server/DataReader/SHN/AttendSchedule.cpp
// Auto-generated: one-file-per-SHN split for AttendSchedule.shn
#include "AttendSchedule.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

AttendScheduleShn& AttendScheduleShn::Get() { static AttendScheduleShn s; return s; }

void AttendScheduleShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("AttendSchedule");
    if (!t) { SHINELOG_WARN("AttendSchedule.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        AttendScheduleRow rec;
        rec.uiAS_StartYear = (uint16)ShnGetU32(*t, _r, "AS_StartYear");
        rec.uiAS_StartMonth = (uint8)ShnGetU32(*t, _r, "AS_StartMonth");
        rec.uiAS_StartDay = (uint8)ShnGetU32(*t, _r, "AS_StartDay");
        rec.uiAS_StartHour = (uint8)ShnGetU32(*t, _r, "AS_StartHour");
        rec.uiAS_StartMinute = (uint8)ShnGetU32(*t, _r, "AS_StartMinute");
        rec.uiAS_JoinKeepTime = (uint16)ShnGetU32(*t, _r, "AS_JoinKeepTime");
        rec.uiAS_CheckTerm = (uint16)ShnGetU32(*t, _r, "AS_CheckTerm");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AttendSchedule.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
