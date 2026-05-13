// Server/DataReader/SHN/GuildTournamentReward.h
// Auto-generated: one-file-per-SHN split for GuildTournamentReward.shn
#ifndef SHINE_DATAREADER_SHN_GUILDTOURNAMENTREWARD_H
#define SHINE_DATAREADER_SHN_GUILDTOURNAMENTREWARD_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GuildTournamentRewardRow {
    uint8            uiRank;
    uint8            uiRewardGroup;
    uint32           uiRewardType;
    uint32           uiValue1;
    uint32           uiValue2;
    uint32           uiValue3;
    uint32           uiIO_Str;
    uint32           uiIO_Con;
    uint32           uiIO_Dex;
    uint32           uiIO_Int;
    uint32           uiIO_Men;
};

class GuildTournamentRewardShn {
public:
    static GuildTournamentRewardShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GuildTournamentRewardRow>& Rows() const { return m_kRows; }
private:
    std::vector<GuildTournamentRewardRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GUILDTOURNAMENTREWARD_H
