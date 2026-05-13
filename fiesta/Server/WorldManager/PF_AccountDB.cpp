// Server/WorldManager/PF_AccountDB.cpp
// Outbound proxy to the Account DB exe. Carries WM-side queries that need
// the Account schema (teenager/age check, web-DB key lookup, return-check
// for inactive-account bonuses). Each public method enqueues an inter-
// server packet on the connector and returns immediately; the response
// flows back through PF_AccountDBSession::OnPacket below.
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"

namespace shine {

class PF_AccountDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("WM<-AccountDB NC=0x%04X", rPkt.GetOpcode());
    }
};

class PF_AccountDB {
public:
    static PF_AccountDB& Get() { static PF_AccountDB s; return s; }

    bool Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort) {
        return m_kConn.Connect(pkIOCP, rIp, uiPort, new PF_AccountDBSession());
    }

    // Teenager / age check. Returns true iff the request was sent; the
    // actual flag arrives async on the response channel.
    bool QueryTeenager(AccountID aid) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body; body.WriteU8(1); body.WriteU32(aid);
        GPacket pk(NC_INTER_AUTH_TOKEN_PUSH);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

    bool QueryWebDBKey(AccountID aid) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body; body.WriteU8(2); body.WriteU32(aid);
        GPacket pk(NC_INTER_AUTH_TOKEN_PUSH);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

    // Returning-player check (inactive >= 30 days).
    bool QueryReturnCheck(AccountID aid) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body; body.WriteU8(3); body.WriteU32(aid);
        GPacket pk(NC_INTER_AUTH_TOKEN_PUSH);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

private:
    PF_AccountDB() {}
    Socket_Connector m_kConn;
};

} // namespace shine
