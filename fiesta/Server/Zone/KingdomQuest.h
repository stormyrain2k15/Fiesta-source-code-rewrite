// Server/Zone/KingdomQuest.h
// KQ. symbol: KingdomQuest, KQServer, KQRewardDataBox,
//                                          KQContribute, KQTeam, KQIsVote, KQVoteMajorityRate
#ifndef SHINE_ZONE_KINGDOMQUEST_H
#define SHINE_ZONE_KINGDOMQUEST_H
#include "ShineObject.h"
#include <map>
#include <vector>

namespace shine {

enum KQState { KQS_IDLE = 0, KQS_RECRUIT, KQS_RUNNING, KQS_VOTE, KQS_END };

class KQServer {
public:
    static KQServer& Get();
    void Tick();
    bool Queue(ShinePlayer* pk);
    bool Leave(ShinePlayer* pk);
    void CastVote(ShinePlayer* pk, uint16 uiOption);
    KQState GetState() const { return m_eState; }
    // Force the state machine into KQS_END right now; the standard
    // tick advances KQS_END -> KQS_IDLE after kKQEndTimeoutMs. Used by
    // PineScript's cEndOfKingdomQuest after a victory/wipe condition.
    // Safe no-op when the machine is already idle.
    void ForceEnd();
private:
    KQServer();
    KQState m_eState;
    uint64  m_uiStateEnterMs;
    std::vector<CharID> m_kQueue;
    std::map<uint16, uint32> m_kVoteCounts;
};

class KQContribute { public: static void Add(CharID c, uint32 uiPts); static uint32 Get(CharID c); };
class KQRewardDataBox { public: static int32 GoldFor(uint32 uiContribution); };
class KingdomQuest {
public:
    static void OnDeath(ShinePlayer* pk);
    // Force-end any KQ instance running on the named map. Called from
    // PineScript via cEndOfKingdomQuest after a victory / wipe condition
    // is reached. Routes to KQServer when one is active; the no-active
    // case is a no-op and returns false.
    static bool End(const char* szMapInx);
};

} // namespace shine
#endif
