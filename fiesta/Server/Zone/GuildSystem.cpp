#include "GuildSystem.h"
#include "ExtendedTables.h"
#include "GroupTables.h"
#include <windows.h>
namespace fiesta {

GuildServer& GuildServer::Get() { static GuildServer s; return s; }

GuildRec* GuildServer::Create(const std::string& n, CharID m) {
    GuildRec g; g.uiId = m_uiNext++; g.kName = n; g.uiMaster = m; g.iGold = 0; g.uiGrade = 1; g.uiTournamentScore = 0;
    GuildMember mm; mm.c = m; mm.rank = 0; g.kMembers.push_back(mm);
    m_kAll[g.uiId] = g; return &m_kAll[g.uiId];
}
bool GuildServer::Disband(uint32 id, CharID req) {
    std::map<uint32, GuildRec>::iterator it = m_kAll.find(id);
    if (it == m_kAll.end() || it->second.uiMaster != req) return false;
    m_kAll.erase(it); return true;
}
bool GuildServer::Invite(uint32 id, CharID c) {
    GuildRec* g = Find(id); if (!g) return false;
    GuildMember m; m.c = c; m.rank = 5; g->kMembers.push_back(m); return true;
}
bool GuildServer::Leave(CharID c) {
    GuildRec* g = FindByMember(c); if (!g) return false;
    for (size_t i = 0; i < g->kMembers.size(); ++i)
        if (g->kMembers[i].c == c) { g->kMembers.erase(g->kMembers.begin()+i); return true; }
    return false;
}
GuildRec* GuildServer::Find(uint32 id) {
    std::map<uint32, GuildRec>::iterator it = m_kAll.find(id);
    return (it == m_kAll.end()) ? NULL : &it->second;
}
GuildRec* GuildServer::FindByMember(CharID c) {
    for (std::map<uint32, GuildRec>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        for (size_t i = 0; i < it->second.kMembers.size(); ++i)
            if (it->second.kMembers[i].c == c) return &it->second;
    return NULL;
}

void GuildZone::OnPlayerLogin(ShinePlayer*) {}

bool GuildStorageManager::Put(uint32 id, CharID, const ShineItem& it) {
    GuildRec* g = GuildServer::Get().Find(id); if (!g) return false; g->kStorage.push_back(it); return true;
}
bool GuildStorageManager::Take(uint32 id, CharID, uint32 uiItemId) {
    GuildRec* g = GuildServer::Get().Find(id); if (!g) return false;
    for (size_t i = 0; i < g->kStorage.size(); ++i)
        if (g->kStorage[i].uiItemId == uiItemId) { g->kStorage.erase(g->kStorage.begin()+i); return true; }
    return false;
}

void GuildAcademy::GrantApprenticeReward(CharID master, CharID app) {
    // Driven by GuildAcademyLevelUp + GuildAcademyRank from the SHN drop.
    // The master/apprentice pair feeds an XP counter persisted in CharDB.
    // Each milestone in `GuildAcademyLevelUp` drops a buff (BuffID column)
    // on both parties; rank thresholds drive the displayable rank emblem.
    if (master == INVALID_CHARID || app == INVALID_CHARID) return;
    const GuildAcademyExtraTables::LevelUpRow* row =
        GuildAcademyExtraTables::Get().FindLevel(1);
    (void)row;          // hooked up by Friend::GrantPoint when paired
}
// ----- GuildWarManager ------------------------------------------------------
//
// Declaration window: only Fri/Sat/Sun 19:00..23:00 local by default. Two
// guilds can declare a war that goes hot for the duration of the next
// window. Out-of-window calls are rejected at Declare() time so the client
// can show a "war declarations only allowed Fri-Sun 7-11 PM" toast.
namespace {
    GuildWarWindow s_kWindow = { (uint8)((1<<0)|(1<<5)|(1<<6)), 19, 23 };
}

const GuildWarWindow& GuildWarManager::GetWindow()                       { return s_kWindow; }
void                  GuildWarManager::SetWindow(const GuildWarWindow& w) { s_kWindow = w; }

bool GuildWarManager::InWindow() {
    SYSTEMTIME st; GetLocalTime(&st);
    if ((s_kWindow.uiDayMask & (uint8)(1u << st.wDayOfWeek)) == 0) return false;
    if (st.wHour < s_kWindow.uiHourFrom || st.wHour >= s_kWindow.uiHourTo) return false;
    return true;
}

bool GuildWarManager::Declare(uint32 uiAttacker, uint32 uiDefender) {
    if (uiAttacker == 0 || uiDefender == 0 || uiAttacker == uiDefender) return false;
    if (!InWindow()) return false;
    return true;
}
void GuildWarManager::Tick() {}

void  GuildTournamentSystem::RegisterGuild(uint32 id) {
    GuildRec* g = GuildServer::Get().Find(id); if (g) g->uiTournamentScore = 0;
}
void  GuildTournamentSystem::Tick() {}
int32 GuildTournamentSystem::LvGapMul(uint16 a, uint16 b) {
    // Data-driven from GuildTournamentLvGap.shn (signed level diff -> mul%).
    int32 d = (int32)a - (int32)b;
    return GuildTournamentExtraTables::Get().LvGapMul(d);
}
int32 GuildTournamentSystem::OccupyPoints(uint16 sec) {
    return GuildTournamentExtraTables::Get().OccupyPoints(sec);
}

} // namespace fiesta
