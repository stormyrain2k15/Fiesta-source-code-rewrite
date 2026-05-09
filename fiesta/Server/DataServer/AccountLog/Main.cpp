// Server/DataServer/AccountLog/Main.cpp
// AccountLog DB service exe entry. Append-only login history, IP-block
// table, friend-event audit. Routed via NC_INTER_ACCTLOG_QUERY.
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../../Shared/PacketBuffer.h"
#include "../../Shared/GPacket.h"
#include "../../Common/NETCOMMAND.h"
#include "../../Common/SendPacket.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

static Database*         g_pkAcctLogDB = NULL;
static SQLP_AccountLog*  g_pkSQLPAcctL = NULL;

enum AcctLogOp {
    AL_OP_APPEND_LOGIN     = 1,    // (acctId, worldNo)
    AL_OP_APPEND_LOGOUT    = 2,    // (acctId, worldNo)
    AL_OP_IP_BLOCK_IS_BLOCK= 3,    // (string ip)              -> (uint8 ok, uint8 blocked)
    AL_OP_EVENT_FRIEND_GET = 4     // (acctId)                 -> rows
};

class AccountLogSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        if (rPkt.GetOpcode() != NC_INTER_ACCTLOG_QUERY) return;
        PacketBuffer body = rPkt.Body();
        uint8 op = 0; body.ReadU8(op);
        switch (op) {
            case AL_OP_APPEND_LOGIN: {
                uint32 acct = 0; uint8 world = 0;
                body.ReadU32(acct); body.ReadU8(world);
                if (g_pkSQLPAcctL) g_pkSQLPAcctL->AppendLogin(acct, world);
                break;
            }
            case AL_OP_APPEND_LOGOUT: {
                uint32 acct = 0; uint8 world = 0;
                body.ReadU32(acct); body.ReadU8(world);
                if (g_pkSQLPAcctL) g_pkSQLPAcctL->AppendLogout(acct, world);
                break;
            }
            case AL_OP_IP_BLOCK_IS_BLOCK: {
                std::string ip; body.ReadString(ip);
                uint8 blocked = 0;
                if (g_pkSQLPAcctL) blocked = g_pkSQLPAcctL->IPBlockIsBlock(ip) ? 1 : 0;
                PacketBuffer ack;
                ack.WriteU8(op); ack.WriteU8(1); ack.WriteU8(blocked);
                SendPacket(this, NC_INTER_ACCTLOG_RESPONSE, ack.Data(), ack.Size());
                break;
            }
            case AL_OP_EVENT_FRIEND_GET: {
                uint32 acct = 0; body.ReadU32(acct);
                std::vector<DBRecord> rows;
                bool ok = g_pkSQLPAcctL && g_pkSQLPAcctL->EventFriendGet(acct, rows);
                PacketBuffer ack;
                ack.WriteU8(op); ack.WriteU8(ok ? 1 : 0);
                ack.WriteU16((uint16)rows.size());
                for (size_t i = 0; i < rows.size(); ++i) {
                    ack.WriteU16((uint16)rows[i].kCols.size());
                    for (size_t j = 0; j < rows[i].kCols.size(); ++j)
                        ack.WriteString(rows[i].kCols[j]);
                }
                SendPacket(this, NC_INTER_ACCTLOG_RESPONSE, ack.Data(), ack.Size());
                break;
            }
            default:
                SHINELOG_WARN("AccountLog: unknown op=%u", (uint32)op);
                break;
        }
    }
};
static IOCPSession* MakeAccountLogSession() { return new AccountLogSession(); }

class AccountLogService : public WinService {
public:
    AccountLogService() : WinService("FiestaAccountLog") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        g_pkAcctLogDB = new Database();
        const OdbcEntry* od = m_kInfo.FindOdbc("AccountLog");
        g_pkAcctLogDB->Connect(od ? od->kConnStr :
            "Driver={SQL Server Native Client 11.0};Server=.;Database=AccountLog;Trusted_Connection=yes;");
        g_pkSQLPAcctL = new SQLP_AccountLog(g_pkAcctLogDB);
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("AccountLog.Port", 27601),
                                 &MakeAccountLogSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        delete g_pkSQLPAcctL; g_pkSQLPAcctL = NULL;
        if (g_pkAcctLogDB) { g_pkAcctLogDB->Disconnect(); delete g_pkAcctLogDB; g_pkAcctLogDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::AccountLogService s; return s.Run(argc, argv); }
