// Server/WorldManager/PF_GameLogDB.cpp
// Outbound proxy to the GameLog DB exe. Most kill/drop/trade writes flow
// directly Zone->GameLogDB; this WM-side facade carries cross-zone gamelog
// inserts that originate on the OPTool fanout (give-item / take-item) and
// the KQServer reward distribution path so the GameLog DB can audit a
// single canonical record per world-event.
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"

namespace shine {

class PF_GameLogDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("WM<-GameLogDB NC=0x%04X", rPkt.GetOpcode());
    }
};

class PF_GameLogDB {
public:
    static PF_GameLogDB& Get() { static PF_GameLogDB s; return s; }

    bool Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort) {
        return m_kConn.Connect(pkIOCP, rIp, uiPort, new PF_GameLogDBSession());
    }

    bool LogGiveItem(uint32 cidOper, CharID cidTarget, uint32 uiItemID, uint16 uiCount) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body;
        body.WriteU8(1);
        body.WriteU32(cidOper);
        body.WriteU32(cidTarget);
        body.WriteU32(uiItemID);
        body.WriteU16(uiCount);
        GPacket pk(NC_INTER_GAMELOG_QUERY);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

    bool LogTakeItem(uint32 cidOper, uint64 uiItemKey) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body;
        body.WriteU8(2);
        body.WriteU32(cidOper);
        body.WriteU64(uiItemKey);
        GPacket pk(NC_INTER_GAMELOG_QUERY);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

    bool LogKQReward(uint32 uiKQID, CharID cid, uint32 uiItemID, uint16 uiCount) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body;
        body.WriteU8(3);
        body.WriteU32(uiKQID);
        body.WriteU32(cid);
        body.WriteU32(uiItemID);
        body.WriteU16(uiCount);
        GPacket pk(NC_INTER_GAMELOG_QUERY);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

private:
    PF_GameLogDB() {}
    Socket_Connector m_kConn;
};

} // namespace shine
