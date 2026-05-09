// Server/Zone/InstanceDungeon.h / .cpp combined
// Instance Dungeon (MID).
//                                  MIDRewardDataBox, MatchInstanceDungeonServer
#ifndef FIESTA_ZONE_INSTANCEDUNGEON_H
#define FIESTA_ZONE_INSTANCEDUNGEON_H
#include "ShineObject.h"
#include <vector>
#include <map>

namespace fiesta {

struct MIDGroupRec { uint32 uiInstanceId; std::vector<CharID> kMembers; uint64 uiStartMs; uint64 uiEndMs; };

class MIDServer {
public:
    static MIDServer& Get();
    uint32 SpinUp(uint32 uiTemplateId, const std::vector<CharID>& rGroup);
    void   TearDown(uint32 uiInstanceId);
    void   Tick();
    bool   IsActive(uint32 uiInstanceId) const;
private:
    std::map<uint32, MIDGroupRec> m_kActive;
    uint32 m_uiNextId;
    MIDServer() : m_uiNextId(1) {}
};

class MIDRewardDataBox { public: static int32 GoldFor(uint32 uiTemplate, uint16 uiClearTimeSec); };
class MatchInstanceDungeonServer { public: static bool TryQueue(ShinePlayer* pk, uint32 uiTemplate); };
class InstanceDungeon { public: static void OnPlayerEnter(ShinePlayer* pk, uint32 uiInstanceId); };
class InstanceDungeon_util { public: static uint64 ProvisionalDurationMs(uint32 uiTemplate); };

} // namespace fiesta
#endif
