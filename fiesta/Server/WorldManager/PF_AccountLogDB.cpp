// Server/WorldManager/PF_AccountLogDB.cpp
// Outbound proxy to the AccountLog DB exe. Login/logout audit appends and
// /report ticket forwarding. Heavy log writes flow Login->AccountLogDB
// directly; this WM-side proxy carries cross-zone reports that originate
// from the OPTool fanout (e.g. ban/kick auditing) so the AccountLog DB
// gets a single canonical record per action.
#include "../Shared/Socket_Connector.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GPacket.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"

namespace shine {

class PF_AccountLogDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("WM<-AccountLogDB NC=0x%04X", rPkt.GetOpcode());
    }
};

class PF_AccountLogDB {
public:
    static PF_AccountLogDB& Get() { static PF_AccountLogDB s; return s; }

    bool Connect(IOCPManager* pkIOCP, const std::string& rIp, uint16 uiPort) {
        return m_kConn.Connect(pkIOCP, rIp, uiPort, new PF_AccountLogDBSession());
    }

    bool AppendLogin (AccountID a, uint8 uiWorldNo) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body; body.WriteU8(1); body.WriteU32(a); body.WriteU8(uiWorldNo);
        GPacket pk(NC_INTER_ACCTLOG_QUERY);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }
    bool AppendLogout(AccountID a, uint8 uiWorldNo) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body; body.WriteU8(2); body.WriteU32(a); body.WriteU8(uiWorldNo);
        GPacket pk(NC_INTER_ACCTLOG_QUERY);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

    bool ReportFile(const std::string& rKind, const std::string& rPayload) {
        if (!m_kConn.IsConnected()) return false;
        PacketBuffer body; body.WriteU8(3); body.WriteString(rKind); body.WriteString(rPayload);
        GPacket pk(NC_INTER_ACCTLOG_QUERY);
        pk.Body().WriteBytes(body.Data(), body.Size());
        return m_kConn.SendPacket(pk);
    }

private:
    PF_AccountLogDB() {}
    Socket_Connector m_kConn;
};

} // namespace shine
