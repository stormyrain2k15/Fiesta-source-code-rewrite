#include "KingdomQuest.h"
#include "BattleTunables.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"
namespace fiesta {

static std::map<CharID, uint32> s_kKQContrib;

// =============================================================================
//  KQ state-machine timings (ms).
//  Adjust these to retune KQ pacing without touching dispatch logic.
// =============================================================================
const uint32 kKQRecruitTimeoutMs   = 60000;            // 1 min
const uint32 kKQRunningTimeoutMs   = 30u * 60u * 1000u; // 30 min
const uint32 kKQVoteTimeoutMs      = 60000;            // 1 min
const uint32 kKQEndTimeoutMs       = 30000;            // 30 sec
const size_t kKQRecruitMinPlayers  = 6;

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
    if (m_eState == KQS_IDLE && m_kQueue.size() >= kKQRecruitMinPlayers)
    { m_eState = KQS_RECRUIT; m_uiStateEnterMs = now; }
    if (m_eState == KQS_RECRUIT && now - m_uiStateEnterMs > kKQRecruitTimeoutMs)
    { m_eState = KQS_RUNNING; m_uiStateEnterMs = now; }
    if (m_eState == KQS_RUNNING && now - m_uiStateEnterMs > kKQRunningTimeoutMs)
    { m_eState = KQS_VOTE; m_uiStateEnterMs = now; m_kVoteCounts.clear(); }
    if (m_eState == KQS_VOTE && now - m_uiStateEnterMs > kKQVoteTimeoutMs)
    { m_eState = KQS_END; m_uiStateEnterMs = now; }
    if (m_eState == KQS_END && now - m_uiStateEnterMs > kKQEndTimeoutMs)
    { m_eState = KQS_IDLE; m_kQueue.clear(); }
}

void KQServer::ForceEnd() {
    if (m_eState == KQS_IDLE) return;
    m_eState         = KQS_END;
    m_uiStateEnterMs = GTimer::NowMillis();
}

void   KQContribute::Add(CharID c, uint32 v) { s_kKQContrib[c] += v; }
uint32 KQContribute::Get(CharID c) { std::map<CharID,uint32>::iterator it = s_kKQContrib.find(c); return it==s_kKQContrib.end()?0:it->second; }

// Reward = base + perPoint * contribution, clamped to maxGold. Generic
// placeholder values; tune in BattleTunables.h kKQReward*.
int32  KQRewardDataBox::GoldFor(uint32 c) {
    int64 raw = (int64)kKQRewardBaseGold +
                (int64)kKQRewardGoldPerPoint * (int64)c;
    if (raw > (int64)kKQRewardMaxGold) raw = (int64)kKQRewardMaxGold;
    if (raw < 0) raw = 0;
    return (int32)raw;
}

void   KingdomQuest::OnDeath(ShinePlayer*) {}

// PineScript-driven KQ end. cEndOfKingdomQuest from the script forces
// the state machine into KQS_END; the standard tick advances
// KQS_END -> KQS_IDLE after kKQEndTimeoutMs and clears the queue.
// The map name is informational -- this implementation runs a single
// KQ instance per zone.
bool KingdomQuest::End(const char* szMapInx) {
    KQServer& kq = KQServer::Get();
    if (kq.GetState() == KQS_IDLE) return false;
    SHINELOG_INFO("KQ ForceEnd from script (map=%s)",
                  szMapInx ? szMapInx : "?");
    kq.ForceEnd();
    return true;
}

} // namespace fiesta
