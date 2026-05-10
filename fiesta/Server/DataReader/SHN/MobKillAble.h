// Server/DataReader/SHN/MobKillAble.h
// Auto-generated: one-file-per-SHN split for MobKillAble.shn
#ifndef FIESTA_DATAREADER_SHN_MOBKILLABLE_H
#define FIESTA_DATAREADER_SHN_MOBKILLABLE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobKillAbleRow {
    uint32           uiMobKillInx;
    uint8            uiCheat;
    uint8            uiMob;
    uint8            uiUser;
};

class MobKillAbleShn {
public:
    static MobKillAbleShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobKillAbleRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobKillAbleRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MOBKILLABLE_H
