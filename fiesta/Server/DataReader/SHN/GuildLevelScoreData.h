// Server/DataReader/SHN/GuildLevelScoreData.h
// Auto-generated: one-file-per-SHN split for GuildLevelScoreData.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDLEVELSCOREDATA_H
#define FIESTA_DATAREADER_SHN_GUILDLEVELSCOREDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildLevelScoreDataRow {
    uint16           uiCheckLevel;
    uint16           uiAddLevelScore;
};

class GuildLevelScoreDataShn {
public:
    static GuildLevelScoreDataShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildLevelScoreDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildLevelScoreDataRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDLEVELSCOREDATA_H
