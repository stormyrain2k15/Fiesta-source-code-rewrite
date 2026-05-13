// Server/Zone/GameLogClient.h
//
// Zone-side outbound IOCP client to the GameLog DB exe. Routes combat
// kill / drop / trade events into World00_GameLog.
#ifndef SHINE_ZONE_GAMELOG_CLIENT_H
#define SHINE_ZONE_GAMELOG_CLIENT_H
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/ShineTypes.h"
#include <string>

namespace shine {

class GameLogClient {
public:
    static GameLogClient& Get();

    bool Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort);
    void Disconnect();
    bool IsConnected() const { return m_kConn.IsConnected(); }

    // Inter-server opcodes (NC_INTER_GAMELOG_QUERY body):
    //   op = 1 LogKill   { CharID killer, MobID mob,        uint16 mapId }
    //   op = 2 LogDrop   { CharID owner,  uint32 itemId,    uint16 count }
    //   op = 3 LogTrade  { CharID from,   CharID to,
    //                       uint32 itemKeyLo, uint32 itemKeyHi, uint16 count }
    //   op = 4 Report    { string kind, string payload }
    void LogKill (CharID cKiller, MobID mob, uint16 uiMapID);
    void LogDrop (CharID cOwner,  uint32 uiItemID, uint16 uiCount);
    void LogTrade(CharID cFrom,   CharID cTo, uint64 uiItemKey, uint16 uiCount);
    void Report  (const std::string& rKind, const std::string& rPayload);

private:
    GameLogClient() {}
    Socket_Connector m_kConn;
};

} // namespace shine
#endif
