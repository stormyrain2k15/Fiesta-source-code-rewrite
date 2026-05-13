// Server/DataReader/SHN/GuildAcademyRank.h
// Auto-generated: one-file-per-SHN split for GuildAcademyRank.shn
#ifndef SHINE_DATAREADER_SHN_GUILDACADEMYRANK_H
#define SHINE_DATAREADER_SHN_GUILDACADEMYRANK_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GuildAcademyRankRow {
    int8             iRank;
    int8             iUnkCol1;
    uint32           uiFame;
    std::string      kBuffName;
};

class GuildAcademyRankShn {
public:
    static GuildAcademyRankShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildAcademyRankRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildAcademyRankRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GUILDACADEMYRANK_H
