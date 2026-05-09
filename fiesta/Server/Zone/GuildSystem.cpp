#include "GuildSystem.h"
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

void GuildAcademy::GrantApprenticeReward(CharID, CharID) {}
bool GuildWarManager::Declare(uint32, uint32) { return true; }
void GuildWarManager::Tick() {}

void  GuildTournamentSystem::RegisterGuild(uint32 id) {
    GuildRec* g = GuildServer::Get().Find(id); if (g) g->uiTournamentScore = 0;
}
void  GuildTournamentSystem::Tick() {}
int32 GuildTournamentSystem::LvGapMul(uint16 a, uint16 b) {
    int32 g = (int32)a - (int32)b;
    if (g >  10) return 80;   // EV_VERIFY
    if (g < -10) return 120;
    return 100;
}
int32 GuildTournamentSystem::OccupyPoints(uint16 sec) { return (int32)sec; } // EV_VERIFY

} // namespace fiesta
