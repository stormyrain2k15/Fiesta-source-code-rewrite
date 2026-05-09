// Server/WorldManager/GuildServer.h
// Cross-zone guild authority: creation, dissolution, member roster, ranks,
// guild-war lifecycle, emblem state, academy, tournament, funds.
#ifndef FIESTA_WM_GUILD_SERVER_H
#define FIESTA_WM_GUILD_SERVER_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace fiesta {

class GuildServer {
public:
    static GuildServer& Get();

    // Lifecycle
    bool   Create   (CharID cMaster, const std::string& rName, uint32& uiGuildNoOut);
    bool   Dissolve (uint32 uiGuildNo);

    // Member management
    bool   AddMember(uint32 uiGuildNo, CharID c, uint8 uiRank);
    bool   DelMember(uint32 uiGuildNo, CharID c);
    bool   SetRank  (uint32 uiGuildNo, CharID c, uint8 uiRank);

    // Funds
    bool   AddFunds (uint32 uiGuildNo, int64 iDelta);

    // Emblem state (validates against the EmblemRule table; the emblem
    // bitmap itself is uploaded separately).
    bool   SetEmblem(uint32 uiGuildNo, uint32 uiEmblemKey);

    // War lifecycle
    bool   DeclareWar (uint32 uiAttacker, uint32 uiDefender);
    void   ScoreKill  (uint32 uiAttacker, uint32 uiDefender, CharID killer, CharID killed);
    bool   CloseWar   (uint32 uiAttacker, uint32 uiDefender, uint32& uiWinnerOut);

    // Academy: members are tracked by (uiGuildNo, charID) so academy buffs
    // can be broadcast to every zone via the per-zone push.
    bool   AcademyJoin (uint32 uiGuildNo, CharID c);
    bool   AcademyLeave(uint32 uiGuildNo, CharID c);

    // Tournament
    bool   TournamentSet(uint32 uiTournamentNo, uint32 uiGuildNo, int32 iStatus);

    // Tick called from WM main loop -- progresses war timers, academy
    // buff reapply, tournament scheduling.
    void   Tick();

private:
    GuildServer() {}
    struct War { uint32 uiAtk, uiDef; uint32 uiAtkKills, uiDefKills; uint64 uiEndsAtMs; };
    std::vector<War> m_kWars;
};

} // namespace fiesta
#endif
