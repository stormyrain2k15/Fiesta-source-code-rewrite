// Server/Zone/MobHuntLog.cpp
// Per-character mob kill log -- writes to GameLogDB via GameLogClient
// for the "monster hunt log" UI in the client (collect-titles tracker).
#include "GameLogClient.h"
namespace shine {
class MobHuntLog {
public:
    static void OnKill(uint32 cid, uint32 uiNpcID) {
        GameLogClient::Get().LogKill((CharID)cid, (MobID)uiNpcID, 0);
    }
};
} // namespace shine
