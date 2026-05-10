// Server/DataReader/SHN/AttendSchedule.h
// Auto-generated: one-file-per-SHN split for AttendSchedule.shn
#ifndef FIESTA_DATAREADER_SHN_ATTENDSCHEDULE_H
#define FIESTA_DATAREADER_SHN_ATTENDSCHEDULE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct AttendScheduleRow {
    uint16           uiAS_StartYear;
    uint8            uiAS_StartMonth;
    uint8            uiAS_StartDay;
    uint8            uiAS_StartHour;
    uint8            uiAS_StartMinute;
    uint16           uiAS_JoinKeepTime;
    uint16           uiAS_CheckTerm;
};

class AttendScheduleShn {
public:
    static AttendScheduleShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AttendScheduleRow>& Rows() const { return m_kRows; }
private:
    std::vector<AttendScheduleRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ATTENDSCHEDULE_H
