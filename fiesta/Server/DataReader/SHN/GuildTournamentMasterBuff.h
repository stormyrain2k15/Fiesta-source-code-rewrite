// Server/DataReader/SHN/GuildTournamentMasterBuff.h
// Auto-generated: one-file-per-SHN split for GuildTournamentMasterBuff.shn
#ifndef SHINE_DATAREADER_SHN_GUILDTOURNAMENTMASTERBUFF_H
#define SHINE_DATAREADER_SHN_GUILDTOURNAMENTMASTERBUFF_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GuildTournamentMasterBuffRow {
    uint16           uiMAP_TYPE;
    std::string      kStateName;
};

class GuildTournamentMasterBuffShn {
public:
    static GuildTournamentMasterBuffShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildTournamentMasterBuffRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildTournamentMasterBuffRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GUILDTOURNAMENTMASTERBUFF_H
