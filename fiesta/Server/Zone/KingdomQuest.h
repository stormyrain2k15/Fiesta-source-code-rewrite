// Server/Zone/KingdomQuest.h
// KQ. symbol: KingdomQuest, KQServer, KQRewardDataBox,
//                                          KQContribute, KQTeam, KQIsVote, KQVoteMajorityRate
#ifndef FIESTA_ZONE_KINGDOMQUEST_H
#define FIESTA_ZONE_KINGDOMQUEST_H
#include "ShineObject.h"
#include <map>
#include <vector>

namespace fiesta {

enum KQState { KQS_IDLE = 0, KQS_RECRUIT, KQS_RUNNING, KQS_VOTE, KQS_END };

class KQServer {
public:
    static KQServer& Get();
    void Tick();
    bool Queue(ShinePlayer* pk);
    bool Leave(ShinePlayer* pk);
    void CastVote(ShinePlayer* pk, uint16 uiOption);
    KQState GetState() const { return m_eState; }
private:
    KQServer();
    KQState m_eState;
    uint64  m_uiStateEnterMs;
    std::vector<CharID> m_kQueue;
    std::map<uint16, uint32> m_kVoteCounts;
};

class KQContribute { public: static void Add(CharID c, uint32 uiPts); static uint32 Get(CharID c); };
class KQRewardDataBox { public: static int32 GoldFor(uint32 uiContribution); };
class KingdomQuest { public: static void OnDeath(ShinePlayer* pk); };

} // namespace fiesta
#endif
