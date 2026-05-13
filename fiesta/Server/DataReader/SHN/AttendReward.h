// Server/DataReader/SHN/AttendReward.h
// Auto-generated: one-file-per-SHN split for AttendReward.shn
#ifndef SHINE_DATAREADER_SHN_ATTENDREWARD_H
#define SHINE_DATAREADER_SHN_ATTENDREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct AttendRewardRow {
    uint8            uiAR_ID;
    uint32           uiAR_Type;
    uint8            uiAR_Count;
    std::string      kAR_ItemInx;
};

class AttendRewardShn {
public:
    static AttendRewardShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AttendRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<AttendRewardRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ATTENDREWARD_H
