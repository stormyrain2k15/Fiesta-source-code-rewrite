// Server/Zone/MarriageSystem.cpp
#include "MarriageSystem.h"
#include "MiscTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

static const uint64 SUMMON_COOLDOWN_MS = 1800000ULL;     // 30 min, EV_VERIFY

MarriageServer& MarriageServer::Get() { static MarriageServer s; return s; }

MarriageRecord* MarriageServer::FindByCharID(CharID c) {
    std::map<CharID, uint32>::iterator it = m_kIndex.find(c);
    if (it == m_kIndex.end()) return NULL;
    std::map<uint32, MarriageRecord>::iterator jt = m_kAll.find(it->second);
    return (jt == m_kAll.end()) ? NULL : &jt->second;
}

bool MarriageServer::Propose(CharID a, CharID b) {
    if (a == b)             return false;
    if (FindByCharID(a))    return false;
    if (FindByCharID(b))    return false;
    MarriageRecord r;
    r.uiRecID        = m_uiNext++;
    r.uiA = a; r.uiB = b;
    r.eState         = MS_PROPOSED;
    r.uiCreatedMs    = 0; r.uiCeremonyMs = 0;
    r.uiPromiseCount = 0; r.uiSummonCdMs = 0;
    m_kAll[r.uiRecID]= r;
    m_kIndex[a]      = r.uiRecID;
    m_kIndex[b]      = r.uiRecID;
    SHINELOG_INFO("Marriage proposed: %u -> %u (rec %u)", a, b, r.uiRecID);
    return true;
}

bool MarriageServer::Accept(CharID a) {
    MarriageRecord* p = FindByCharID(a); if (!p)                    return false;
    if (p->eState != MS_PROPOSED)                                   return false;
    if (p->uiB != a)                                                return false;
    p->eState = MS_ENGAGED;
    return true;
}
bool MarriageServer::Reject(CharID a) {
    MarriageRecord* p = FindByCharID(a); if (!p)                    return false;
    if (p->eState != MS_PROPOSED)                                   return false;
    if (p->uiB != a)                                                return false;
    m_kIndex.erase(p->uiA); m_kIndex.erase(p->uiB);
    m_kAll.erase(p->uiRecID);
    return true;
}

bool MarriageServer::Wed(CharID a, CharID b, uint64 uiNowMs) {
    MarriageRecord* p = FindByCharID(a); if (!p)                    return false;
    if (p->uiA != a || p->uiB != b)                                 return false;
    if (p->eState != MS_ENGAGED)                                    return false;
    p->eState       = MS_MARRIED;
    p->uiCeremonyMs = uiNowMs;
    SHINELOG_INFO("Marriage solemnised: %u <-> %u (rec %u)", a, b, p->uiRecID);
    return true;
}

bool MarriageServer::Divorce(CharID a) {
    MarriageRecord* p = FindByCharID(a); if (!p)                    return false;
    p->eState = MS_DIVORCED;
    m_kIndex.erase(p->uiA); m_kIndex.erase(p->uiB);
    m_kAll.erase(p->uiRecID);
    return true;
}

bool MarriageServer::Summon(CharID self, uint64 uiNowMs) {
    MarriageRecord* p = FindByCharID(self); if (!p)                 return false;
    if (p->eState != MS_MARRIED)                                    return false;
    if (uiNowMs < p->uiSummonCdMs)                                  return false;
    p->uiSummonCdMs = uiNowMs + SUMMON_COOLDOWN_MS;
    return true;
}

void MarriageServer::GrantPromiseTick(CharID self, uint64 /*uiNowMs*/) {
    MarriageRecord* p = FindByCharID(self); if (!p)                 return;
    if (p->eState != MS_MARRIED)                                    return;
    if (p->uiA != self)                                             return;     // count from A only
    p->uiPromiseCount++;
    int32 reward = HPRewardTable::Get().ForCount(p->uiPromiseCount);
    if (reward > 0) {
        SHINELOG_INFO("Marriage promise #%u: rec %u, reward handle %d",
                      p->uiPromiseCount, p->uiRecID, reward);
        // Issuance happens via Inventory::Add against HPReward row -> ShineRewardTable.
    }
}

} // namespace fiesta
