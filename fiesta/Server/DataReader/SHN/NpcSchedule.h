// Server/DataReader/SHN/NpcSchedule.h
// Auto-generated: one-file-per-SHN split for NpcSchedule.shn
#ifndef FIESTA_DATAREADER_SHN_NPCSCHEDULE_H
#define FIESTA_DATAREADER_SHN_NPCSCHEDULE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct NpcScheduleRow {
    std::string      kMob_Inx;
    uint16           uiNS_Year;
    uint8            uiNS_Month;
    uint8            uiNS_Day;
    uint8            uiNS_Hour;
    uint8            uiNS_Minute;
    uint16           uiNS_CycleHour;
    uint8            uiNS_LifeHour;
    uint8            uiNS_IsMsg;
};

class NpcScheduleShn {
public:
    static NpcScheduleShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<NpcScheduleRow>& Rows() const { return m_kRows; }
private:
    std::vector<NpcScheduleRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_NPCSCHEDULE_H
