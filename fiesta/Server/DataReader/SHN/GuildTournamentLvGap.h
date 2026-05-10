// Server/DataReader/SHN/GuildTournamentLvGap.h
// Auto-generated: one-file-per-SHN split for GuildTournamentLvGap.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDTOURNAMENTLVGAP_H
#define FIESTA_DATAREADER_SHN_GUILDTOURNAMENTLVGAP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildTournamentLvGapRow {
    uint16           uiMAP_TYPE;
    float            fLvGap;
    uint16           uiPointRate;
};

class GuildTournamentLvGapShn {
public:
    static GuildTournamentLvGapShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildTournamentLvGapRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildTournamentLvGapRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDTOURNAMENTLVGAP_H
