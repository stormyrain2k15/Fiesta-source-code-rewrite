// Server/Zone/ExpeditionSystem.cpp
#include "ExpeditionSystem.h"
#include "ZoneServer.h"
#include "AbState.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

static const size_t MAX_SUB_PARTIES = 6;-- ExpeditionPartyWin

ExpeditionSystem& ExpeditionSystem::Get() { static ExpeditionSystem s; return s; }

ExpeditionRec* ExpeditionSystem::Create(CharID uiLeader, eExpeditionLoot eLoot) {
    Party* pLeaderParty = PartyContainer::Get().GetByMember(uiLeader);
    if (!pLeaderParty || pLeaderParty->uiLeader != uiLeader) return NULL;
    if (m_kPartyIdx.find(pLeaderParty->uiPartyId) != m_kPartyIdx.end()) return NULL;

    ExpeditionRec rec;
    rec.uiExpId        = m_uiNext++;
    rec.uiLeader       = uiLeader;
    rec.eLoot          = eLoot;
    rec.uiTurnCursor   = 0;
    rec.uiMasterBuffID = 0;
    rec.kPartyIds.push_back(pLeaderParty->uiPartyId);
    m_kAll[rec.uiExpId]                  = rec;
    m_kPartyIdx[pLeaderParty->uiPartyId] = rec.uiExpId;
    SHINELOG_INFO("Expedition %u created by char %u (loot mode %d)",
                  rec.uiExpId, uiLeader, (int)eLoot);
    return &m_kAll[rec.uiExpId];
}

ExpeditionRec* ExpeditionSystem::Find(uint32 uiExpId) {
    std::map<uint32, ExpeditionRec>::iterator it = m_kAll.find(uiExpId);
    return (it == m_kAll.end()) ? NULL : &it->second;
}

ExpeditionRec* ExpeditionSystem::FindByMember(CharID c) {
    Party* pP = PartyContainer::Get().GetByMember(c);
    if (!pP) return NULL;
    std::map<uint32, uint32>::iterator it = m_kPartyIdx.find(pP->uiPartyId);
    if (it == m_kPartyIdx.end()) return NULL;
    return Find(it->second);
}

bool ExpeditionSystem::JoinParty(uint32 uiExpId, uint32 uiPartyId) {
    ExpeditionRec* pE = Find(uiExpId); if (!pE)                              return false;
    if (pE->kPartyIds.size() >= MAX_SUB_PARTIES)                              return false;
    if (m_kPartyIdx.find(uiPartyId) != m_kPartyIdx.end())                     return false;
    pE->kPartyIds.push_back(uiPartyId);
    m_kPartyIdx[uiPartyId] = uiExpId;
    return true;
}

bool ExpeditionSystem::LeaveParty(uint32 uiExpId, uint32 uiPartyId) {
    ExpeditionRec* pE = Find(uiExpId); if (!pE)                              return false;
    for (size_t i = 0; i < pE->kPartyIds.size(); ++i) {
        if (pE->kPartyIds[i] == uiPartyId) {
            pE->kPartyIds.erase(pE->kPartyIds.begin() + i);
            m_kPartyIdx.erase(uiPartyId);
            // If leader's party left, disband.
            Party* pL = PartyContainer::Get().GetByMember(pE->uiLeader);
            if (!pL || pL->uiPartyId == uiPartyId) {
                Disband(uiExpId, pE->uiLeader);
            }
            return true;
        }
    }
    return false;
}

bool ExpeditionSystem::Disband(uint32 uiExpId, CharID uiRequester) {
    ExpeditionRec* pE = Find(uiExpId); if (!pE)                              return false;
    if (pE->uiLeader != uiRequester)                                         return false;
    for (size_t i = 0; i < pE->kPartyIds.size(); ++i)
        m_kPartyIdx.erase(pE->kPartyIds[i]);
    m_kAll.erase(uiExpId);
    return true;
}

CharID ExpeditionSystem::RouteLoot(uint32 uiExpId) {
    ExpeditionRec* pE = Find(uiExpId); if (!pE)                              return INVALID_CHARID;
    if (pE->eLoot == EL_MASTER)                                              return pE->uiLeader;
    // Build flattened member list across all sub-parties.
    std::vector<CharID> all;
    for (size_t i = 0; i < pE->kPartyIds.size(); ++i) {
        // Iterate parties via PartyContainer; we don't expose a pretty
        // accessor but every sub-party leader's party can be fetched.
        // For now we iterate the leaders' parties directly.
        // (Other party member resolution is the caller's job in the
        // original engine -- this method ships the *winner* CharID.)
    }
    if (pE->eLoot == EL_FREE)                                                return INVALID_CHARID;
    // EL_TURN: bump cursor, return cursor-th party leader for now.
    if (pE->kPartyIds.empty())                                               return INVALID_CHARID;
    pE->uiTurnCursor = (pE->uiTurnCursor + 1) % (uint32)pE->kPartyIds.size();
    return pE->uiLeader;     // simplistic; runtime composes member list.
}

void ExpeditionSystem::ApplyMasterBuff(uint32 uiExpId, uint32 uiBuffID) {
    ExpeditionRec* pE = Find(uiExpId); if (!pE)                              return;
    pE->uiMasterBuffID = uiBuffID;
    SHINELOG_INFO("Expedition %u master buff = %u", uiExpId, uiBuffID);
    // Buff propagation: AbState system applies the buffID to every member of
    // every sub-party. The hook lives in AbState.cpp; this just records
    // the desired ID.
}

void ExpeditionSystem::Tick() {
    // Per-expedition refresh: re-apply the master buff to every member of
    // every sub-party every tick. Keep-time is 90s (~1 min) so a missed
    // tick still leaves the buff pinned. AbState handles dedupe / stack
    // resolution against any pre-existing identical abstate.
    static const uint32 EXP_BUFF_KEEP_MS = 90000;
    std::map<uint32, ExpeditionRec>::iterator it;
    for (it = m_kAll.begin(); it != m_kAll.end(); ++it) {
        const ExpeditionRec& e = it->second;
        if (e.uiMasterBuffID == 0) continue;
        for (size_t pi = 0; pi < e.kPartyIds.size(); ++pi) {
            const Party* pkP = PartyContainer::Get().Get(e.kPartyIds[pi]);
            if (!pkP) continue;
            for (size_t mi = 0; mi < pkP->kMembers.size(); ++mi) {
                ShinePlayer* pkM =
                    ZoneServer::Get().FindPlayerByCharID(pkP->kMembers[mi]);
                if (pkM)
                    pkM->AbState().Apply(e.uiMasterBuffID, EXP_BUFF_KEEP_MS);
            }
        }
    }
}

} // namespace shine
