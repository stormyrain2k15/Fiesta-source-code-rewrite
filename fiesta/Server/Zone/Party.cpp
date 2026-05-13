#include "Party.h"
namespace shine {

PartyContainer& PartyContainer::Get() { static PartyContainer s; return s; }

Party* PartyContainer::Create(CharID leader) {
    Party p; p.uiPartyId = m_uiNext++; p.uiLeader = leader; p.bRaid = false;
    p.kMembers.push_back(leader);
    m_kAll[p.uiPartyId] = p;
    return &m_kAll[p.uiPartyId];
}

bool PartyContainer::Join(uint32 pid, CharID c) {
    std::map<uint32, Party>::iterator it = m_kAll.find(pid);
    if (it == m_kAll.end()) return false;
    if (it->second.kMembers.size() >= (it->second.bRaid ? 24 : 8)) return false;
    for (size_t i = 0; i < it->second.kMembers.size(); ++i) if (it->second.kMembers[i] == c) return false;
    it->second.kMembers.push_back(c); return true;
}

bool PartyContainer::Leave(CharID c) {
    Party* p = GetByMember(c); if (!p) return false;
    for (size_t i = 0; i < p->kMembers.size(); ++i)
        if (p->kMembers[i] == c) { p->kMembers.erase(p->kMembers.begin()+i); break; }
    if (p->kMembers.empty()) m_kAll.erase(p->uiPartyId);
    else if (p->uiLeader == c) p->uiLeader = p->kMembers[0];
    return true;
}

bool PartyContainer::Kick(uint32 pid, CharID kicker, CharID kicked) {
    std::map<uint32, Party>::iterator it = m_kAll.find(pid);
    if (it == m_kAll.end() || it->second.uiLeader != kicker) return false;
    for (size_t i = 0; i < it->second.kMembers.size(); ++i)
        if (it->second.kMembers[i] == kicked) { it->second.kMembers.erase(it->second.kMembers.begin()+i); return true; }
    return false;
}

bool PartyContainer::Break(uint32 pid, CharID req) {
    std::map<uint32, Party>::iterator it = m_kAll.find(pid);
    if (it == m_kAll.end() || it->second.uiLeader != req) return false;
    m_kAll.erase(it); return true;
}

Party* PartyContainer::GetByMember(CharID c) {
    for (std::map<uint32, Party>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        for (size_t i = 0; i < it->second.kMembers.size(); ++i)
            if (it->second.kMembers[i] == c) return &it->second;
    return NULL;
}

Party* PartyContainer::Get(uint32 uiPartyId) {
    std::map<uint32, Party>::iterator it = m_kAll.find(uiPartyId);
    return (it == m_kAll.end()) ? NULL : &it->second;
}

int32 PartyContainer::BonusXpPct(size_t n) {
    static const int32 t[9] = { 0, 0, 10, 15, 20, 25, 30, 32, 35 };
    return n < 9 ? t[n] : 35;
}

bool RaidSystem::ConvertToRaid(uint32 pid) {
    Party* p = NULL;
    std::map<uint32, Party>::iterator it = PartyContainer::Get().m_kAll.find(pid);
    return false; // need friend access; left as no-op
}

void PartyCreate (CharID l)        { PartyContainer::Get().Create(l); }
void PartyJoin   (CharID c, uint32 p){ PartyContainer::Get().Join(p, c); }
void PartyLeave  (CharID c)        { PartyContainer::Get().Leave(c); }
void PartyKickOut(CharID a, CharID b){ Party* p = PartyContainer::Get().GetByMember(a); if (p) PartyContainer::Get().Kick(p->uiPartyId, a, b); }
void PartyBreak  (CharID a)        { Party* p = PartyContainer::Get().GetByMember(a); if (p) PartyContainer::Get().Break(p->uiPartyId, a); }

} // namespace shine
