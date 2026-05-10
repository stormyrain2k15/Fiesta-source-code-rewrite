// Server/DataReader/SHN/GuildTournamentRequire.h
// Auto-generated: one-file-per-SHN split for GuildTournamentRequire.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDTOURNAMENTREQUIRE_H
#define FIESTA_DATAREADER_SHN_GUILDTOURNAMENTREQUIRE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildTournamentRequireRow {
    uint8            uiMinLv;
    uint16           uiMinMem;
    uint32           uiJoinMoney;
};

class GuildTournamentRequireShn {
public:
    static GuildTournamentRequireShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildTournamentRequireRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildTournamentRequireRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDTOURNAMENTREQUIRE_H
