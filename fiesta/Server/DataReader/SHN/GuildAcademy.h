// Server/DataReader/SHN/GuildAcademy.h
// Auto-generated: one-file-per-SHN split for GuildAcademy.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDACADEMY_H
#define FIESTA_DATAREADER_SHN_GUILDACADEMY_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildAcademyRow {
    std::string      kBuffName;
    uint32           uiLeastJoinTime;
    int8             iRankAggregationTime;
    int8             iUnkCol3;
};

class GuildAcademyShn {
public:
    static GuildAcademyShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildAcademyRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildAcademyRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDACADEMY_H
