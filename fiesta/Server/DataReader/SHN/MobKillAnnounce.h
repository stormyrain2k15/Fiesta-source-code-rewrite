// Server/DataReader/SHN/MobKillAnnounce.h
// Auto-generated: one-file-per-SHN split for MobKillAnnounce.shn
#ifndef SHINE_DATAREADER_SHN_MOBKILLANNOUNCE_H
#define SHINE_DATAREADER_SHN_MOBKILLANNOUNCE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MobKillAnnounceRow {
    uint32           uiMobID;
    uint32           uiTextIndex;
};

class MobKillAnnounceShn {
public:
    static MobKillAnnounceShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobKillAnnounceRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobKillAnnounceRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOBKILLANNOUNCE_H
