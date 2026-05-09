// Server/Zone/GuildTournamentTable.cpp
// GT data tables -- score per grade, level-gap rate, occupy time, AbState
// per map type, master buff. From the multi-row GuildTournament*.shn set.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class GuildTournamentTable {
public:
    static GuildTournamentTable& Get() { static GuildTournamentTable s; return s; }
    bool Load() {
        return ShnRegistry::Get().GetTable("GuildTournament")           != NULL
            || ShnRegistry::Get().GetTable("GuildTournamentReward")     != NULL
            || ShnRegistry::Get().GetTable("GuildTournamentScore")      != NULL
            || ShnRegistry::Get().GetTable("GuildTournamentSkill")      != NULL
            || ShnRegistry::Get().GetTable("GuildTournamentMasterBuff") != NULL
            || ShnRegistry::Get().GetTable("GuildTournamentLvGap")      != NULL
            || ShnRegistry::Get().GetTable("GuildTournamentOccupy")     != NULL;
    }
};
} // namespace fiesta
