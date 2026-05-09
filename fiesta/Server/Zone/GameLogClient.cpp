// Server/Zone/GameLogClient.cpp
#include "GameLogClient.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"

namespace fiesta {

class GameLogClientSession : public IOCPSession {
public:
    virtual void OnConnect()    { SHINELOG_INFO("Zone->GameLog connected"); }
    virtual void OnDisconnect() { SHINELOG_WARN("Zone->GameLog disconnected"); }
    virtual void OnPacket(const GPacket&) {}   // GameLog never replies
};

GameLogClient& GameLogClient::Get() { static GameLogClient s; return s; }

bool GameLogClient::Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort) {
    return m_kConn.Connect(pkIOCP, rIp, uiPort, new GameLogClientSession());
}
void GameLogClient::Disconnect() { m_kConn.Close(); }

void GameLogClient::LogKill(CharID cKiller, MobID mob, uint16 uiMapID) {
    if (!IsConnected()) return;
    PacketBuffer body;
    body.WriteU8 (1);
    body.WriteU32(cKiller);
    body.WriteU32(mob);
    body.WriteU16(uiMapID);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_GAMELOG_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    m_kConn.SendPacket(kPkt);
}
void GameLogClient::LogDrop(CharID cOwner, uint32 uiItemID, uint16 uiCount) {
    if (!IsConnected()) return;
    PacketBuffer body;
    body.WriteU8 (2);
    body.WriteU32(cOwner);
    body.WriteU32(uiItemID);
    body.WriteU16(uiCount);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_GAMELOG_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    m_kConn.SendPacket(kPkt);
}
void GameLogClient::LogTrade(CharID cFrom, CharID cTo, uint64 uiItemKey, uint16 uiCount) {
    if (!IsConnected()) return;
    PacketBuffer body;
    body.WriteU8 (3);
    body.WriteU32(cFrom);
    body.WriteU32(cTo);
    body.WriteU32((uint32)(uiItemKey & 0xFFFFFFFFu));
    body.WriteU32((uint32)(uiItemKey >> 32));
    body.WriteU16(uiCount);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_GAMELOG_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    m_kConn.SendPacket(kPkt);
}
void GameLogClient::Report(const std::string& rKind, const std::string& rPayload) {
    if (!IsConnected()) return;
    PacketBuffer body;
    body.WriteU8 (4);
    body.WriteString(rKind);
    body.WriteString(rPayload);
    GPacket kPkt; kPkt.SetOpcode(NC_INTER_GAMELOG_QUERY);
    kPkt.Body().WriteBytes(body.Data(), body.Size());
    m_kConn.SendPacket(kPkt);
}

} // namespace fiesta
