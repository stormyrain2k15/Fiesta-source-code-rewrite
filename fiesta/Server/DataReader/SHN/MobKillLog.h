// Server/DataReader/SHN/MobKillLog.h
// Auto-generated: one-file-per-SHN split for MobKillLog.shn
#ifndef SHINE_DATAREADER_SHN_MOBKILLLOG_H
#define SHINE_DATAREADER_SHN_MOBKILLLOG_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MobKillLogRow {
    uint32           uiMobID;
};

class MobKillLogShn {
public:
    static MobKillLogShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobKillLogRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobKillLogRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOBKILLLOG_H
