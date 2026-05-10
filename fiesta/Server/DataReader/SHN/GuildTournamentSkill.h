// Server/DataReader/SHN/GuildTournamentSkill.h
// Auto-generated: one-file-per-SHN split for GuildTournamentSkill.shn
#ifndef FIESTA_DATAREADER_SHN_GUILDTOURNAMENTSKILL_H
#define FIESTA_DATAREADER_SHN_GUILDTOURNAMENTSKILL_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GuildTournamentSkillRow {
    uint16           uiMAP_TYPE;
    uint16           uiIndex;
    uint16           uiDeathPoint;
    std::string      kStaName;
    uint32           uiTargetType;
    uint32           uiDlyTime;
};

class GuildTournamentSkillShn {
public:
    static GuildTournamentSkillShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildTournamentSkillRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildTournamentSkillRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GUILDTOURNAMENTSKILL_H
