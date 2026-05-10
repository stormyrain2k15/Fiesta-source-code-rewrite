// Server/DataReader/SHN/GuildTournamentOccupy.h
// Auto-generated: one-file-per-SHN split for GuildTournamentOccupy.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDTOURNAMENTOCCUPY_H
#define FIESTA_DATAREADER_SHN_GUILDTOURNAMENTOCCUPY_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildTournamentOccupyRow {
    uint16           uiMAP_TYPE;
    uint16           uiOccupyTime;
    uint16           uiScore;
};

class GuildTournamentOccupyShn {
public:
    static GuildTournamentOccupyShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildTournamentOccupyRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildTournamentOccupyRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDTOURNAMENTOCCUPY_H
