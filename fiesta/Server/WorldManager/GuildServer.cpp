// Server/WorldManager/GuildServer.cpp
// Routes every guild mutation to the CharDB exe via the CharDBClient
// already bound on WM. Each public method here is a thin wrapper that
// (a) updates in-process state and (b) issues the matching stored proc.
#include "GuildServer.h"
#include "WMCharDBClient.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

GuildServer& GuildServer::Get() { static GuildServer s; return s; }

bool GuildServer::Create(CharID cMaster, const std::string& rName, uint32& uiGuildNoOut) {
    uiGuildNoOut = 0;
    bool ok = WMCharDBClient::Get().GuildCreate(cMaster, rName, uiGuildNoOut);
    if (ok) SHINELOG_INFO("Guild created '%s' master=%u no=%u", rName.c_str(), cMaster, uiGuildNoOut);
    return ok;
}
bool GuildServer::Dissolve (uint32 g)        { return WMCharDBClient::Get().GuildDissolve(g); }
bool GuildServer::AddMember(uint32 g, CharID c, uint8 r) { return WMCharDBClient::Get().GuildAddMember(g, c, r); }
bool GuildServer::DelMember(uint32 g, CharID c)          { return WMCharDBClient::Get().GuildDelMember(g, c); }
bool GuildServer::SetRank  (uint32 g, CharID c, uint8 r) { return WMCharDBClient::Get().GuildSetRank(g, c, r); }
bool GuildServer::AddFunds (uint32 g, int64 d)           { return WMCharDBClient::Get().GuildAddFunds(g, d); }
bool GuildServer::SetEmblem(uint32 g, uint32 k)          { return WMCharDBClient::Get().GuildSetEmblem(g, k); }

bool GuildServer::DeclareWar(uint32 a, uint32 d) {
    War w; w.uiAtk = a; w.uiDef = d; w.uiAtkKills = w.uiDefKills = 0;
    w.uiEndsAtMs = GTimer::NowMillis() + 30 * 60 * 1000ULL;   // 30-minute war window
    m_kWars.push_back(w);
    return WMCharDBClient::Get().GuildWarBegin(a, d);
}
void GuildServer::ScoreKill(uint32 a, uint32 d, CharID killer, CharID killed) {
    for (size_t i = 0; i < m_kWars.size(); ++i) {
        War& w = m_kWars[i];
        if ((w.uiAtk == a && w.uiDef == d) || (w.uiAtk == d && w.uiDef == a)) {
            if (w.uiAtk == a) ++w.uiAtkKills; else ++w.uiDefKills;
            WMCharDBClient::Get().GuildWarKill(a, d, killer, killed);
            return;
        }
    }
}
bool GuildServer::CloseWar(uint32 a, uint32 d, uint32& uiWinnerOut) {
    for (size_t i = 0; i < m_kWars.size(); ++i) {
        War& w = m_kWars[i];
        if ((w.uiAtk == a && w.uiDef == d) || (w.uiAtk == d && w.uiDef == a)) {
            uiWinnerOut = (w.uiAtkKills >= w.uiDefKills) ? w.uiAtk : w.uiDef;
            m_kWars.erase(m_kWars.begin() + i);
            return WMCharDBClient::Get().GuildWarEnd(a, d, uiWinnerOut);
        }
    }
    uiWinnerOut = 0;
    return false;
}

bool GuildServer::AcademyJoin (uint32 g, CharID c) { return WMCharDBClient::Get().GuildAcademyJoin (g, c); }
bool GuildServer::AcademyLeave(uint32 g, CharID c) { return WMCharDBClient::Get().GuildAcademyLeave(g, c); }
bool GuildServer::TournamentSet(uint32 t, uint32 g, int32 s) {
    return WMCharDBClient::Get().GuildTournamentSet(t, g, s);
}

void GuildServer::Tick() {
    uint64 now = GTimer::NowMillis();
    for (size_t i = 0; i < m_kWars.size(); ) {
        if (now >= m_kWars[i].uiEndsAtMs) {
            uint32 winner = (m_kWars[i].uiAtkKills >= m_kWars[i].uiDefKills)
                          ? m_kWars[i].uiAtk : m_kWars[i].uiDef;
            WMCharDBClient::Get().GuildWarEnd(m_kWars[i].uiAtk, m_kWars[i].uiDef, winner);
            m_kWars.erase(m_kWars.begin() + i);
        } else ++i;
    }
}

} // namespace shine
