// Server/DataReader/SHN/GuildTournamentScore.h
// Auto-generated: one-file-per-SHN split for GuildTournamentScore.shn
#ifndef SHINE_DATAREADER_SHN_GUILDTOURNAMENTSCORE_H
#define SHINE_DATAREADER_SHN_GUILDTOURNAMENTSCORE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GuildTournamentScoreRow {
    uint16           uiMAP_TYPE;
    int16            iGradeScore;
    int16            iUnkCol2;
    int16            iUnkCol3;
    int16            iUnkCol4;
    int16            iUnkCol5;
    int16            iUnkCol6;
    int16            iUnkCol7;
};

class GuildTournamentScoreShn {
public:
    static GuildTournamentScoreShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildTournamentScoreRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildTournamentScoreRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GUILDTOURNAMENTSCORE_H
