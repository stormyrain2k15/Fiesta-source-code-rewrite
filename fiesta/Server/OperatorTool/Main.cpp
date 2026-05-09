// Server/OperatorTool/Main.cpp
// 39 -- OperatorTool / optool admin path. Auths against the OperatorTool DB
// (tOperator / tMenuAuth) via SQLP_Operator and exposes ban/kick commands
// over IOCP.
// EVIDENCE: PDB_CONFIRMED  symbol: OPToolSession, OperatorTool, AmpersandCommand
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../DataServer/Common/Database.h"
#include "../DataServer/Common/SQLP.h"

namespace fiesta {

static Database*     g_pkOpDB    = NULL;
static SQLP_Operator* g_pkSQLPOp = NULL;

class OPToolSession : public IOCPSession {
public:
    OPToolSession() : m_iOperLevel(0) {}
    virtual void OnPacket(const GPacket& rPkt) {
        switch (rPkt.GetOpcode()) {
            case NC_OPTOOL_AUTH_REQ: {
                // Auth packet body: ASCII userid '\0' password '\0'
                std::string id, pw;
                {
                    const PacketBuffer& kBody = rPkt.Body();
                    const uint8* p = kBody.Data(); size_t n = kBody.Size(), k = 0;
                    while (k < n && p[k] != 0) { id.push_back((char)p[k]); ++k; } if (k < n) ++k;
                    while (k < n && p[k] != 0) { pw.push_back((char)p[k]); ++k; }
                }
                bool bOK = g_pkSQLPOp && g_pkSQLPOp->Logon(id, pw, m_iOperLevel);
                PacketBuffer ack; ack.WriteU8(bOK ? 1 : 0); ack.WriteU8((uint8)m_iOperLevel);
                SendPacket(this, NC_OPTOOL_AUTH_ACK, ack.Data(), ack.Size());
                if (bOK) SHINELOG_INFO("optool: '%s' authed level=%d", id.c_str(), m_iOperLevel);
                else     SHINELOG_WARN("optool: '%s' auth FAIL",      id.c_str());
                break;
            }
            case NC_OPTOOL_BAN_CMD:  SHINELOG_WARN("optool: BAN  (level=%d)", m_iOperLevel); break;
            case NC_OPTOOL_KICK_CMD: SHINELOG_WARN("optool: KICK (level=%d)", m_iOperLevel); break;
            default: break;
        }
    }
private:
    int m_iOperLevel;
};
static IOCPSession* MakeOPToolSession() { return new OPToolSession(); }

class OperatorToolService : public WinService {
public:
    OperatorToolService() : WinService("FiestaOperatorTool") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        g_pkOpDB = new Database();
        g_pkOpDB->Connect(m_kInfo.GetString("OPTool.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=OperatorTool;Trusted_Connection=yes;"));
        g_pkSQLPOp = new SQLP_Operator(g_pkOpDB);
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("OPTool.Port", 30000),
                                 &MakeOPToolSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        delete g_pkSQLPOp; g_pkSQLPOp = NULL;
        if (g_pkOpDB) { g_pkOpDB->Disconnect(); delete g_pkOpDB; g_pkOpDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::OperatorToolService s; return s.Run(argc, argv); }
