// Server/DataReader/SHN/GuildAcademyLevelUp.h
// Auto-generated: one-file-per-SHN split for GuildAcademyLevelUp.shn
#ifndef SHINE_DATAREADER_SHN_GUILDACADEMYLEVELUP_H
#define SHINE_DATAREADER_SHN_GUILDACADEMYLEVELUP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GuildAcademyLevelUpRow {
    uint8            uiLevel;
    uint32           uiPoint;
    uint32           uiBuffTime;
};

class GuildAcademyLevelUpShn {
public:
    static GuildAcademyLevelUpShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildAcademyLevelUpRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildAcademyLevelUpRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GUILDACADEMYLEVELUP_H
