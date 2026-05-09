#include "KingdomQuest.h"
#include "../Shared/GTimer.h"
namespace fiesta {

static std::map<CharID, uint32> s_kKQContrib;

KQServer& KQServer::Get() { static KQServer s; return s; }
KQServer::KQServer() : m_eState(KQS_IDLE), m_uiStateEnterMs(0) {}

bool KQServer::Queue(ShinePlayer* pk) {
    if (!pk) return false;
    for (size_t i = 0; i < m_kQueue.size(); ++i) if (m_kQueue[i] == pk->GetCharID()) return false;
    m_kQueue.push_back(pk->GetCharID());
    return true;
}
bool KQServer::Leave(ShinePlayer* pk) {
    if (!pk) return false;
    for (size_t i = 0; i < m_kQueue.size(); ++i)
        if (m_kQueue[i] == pk->GetCharID()) { m_kQueue.erase(m_kQueue.begin()+i); return true; }
    return false;
}
void KQServer::CastVote(ShinePlayer*, uint16 opt) { m_kVoteCounts[opt] += 1; }
void KQServer::Tick() {
    uint64 now = GTimer::NowMillis();
    if (m_eState == KQS_IDLE && m_kQueue.size() >= 6) { m_eState = KQS_RECRUIT; m_uiStateEnterMs = now; }
    if (m_eState == KQS_RECRUIT && now - m_uiStateEnterMs > 60000) { m_eState = KQS_RUNNING; m_uiStateEnterMs = now; }
    if (m_eState == KQS_RUNNING && now - m_uiStateEnterMs > 30ULL * 60000ULL) { m_eState = KQS_VOTE; m_uiStateEnterMs = now; m_kVoteCounts.clear(); }
    if (m_eState == KQS_VOTE && now - m_uiStateEnterMs > 60000) { m_eState = KQS_END; m_uiStateEnterMs = now; }
    if (m_eState == KQS_END && now - m_uiStateEnterMs > 30000) { m_eState = KQS_IDLE; m_kQueue.clear(); }
}

void   KQContribute::Add(CharID c, uint32 v) { s_kKQContrib[c] += v; }
uint32 KQContribute::Get(CharID c) { std::map<CharID,uint32>::iterator it = s_kKQContrib.find(c); return it==s_kKQContrib.end()?0:it->second; }
int32  KQRewardDataBox::GoldFor(uint32 c) { return (int32)c * 10; }
void   KingdomQuest::OnDeath(ShinePlayer*) {}

} // namespace fiesta
