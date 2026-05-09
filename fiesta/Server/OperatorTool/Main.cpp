// Server/OperatorTool/Main.cpp
// 39 -- OperatorTool / optool admin path.
// EVIDENCE: PDB_CONFIRMED  symbol: OPToolSession, OperatorTool, AmpersandCommand
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"

namespace fiesta {

class OPToolSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        switch (rPkt.GetOpcode()) {
            case NC_OPTOOL_AUTH_REQ: {
                PacketBuffer ack; ack.WriteU8(1);
                SendPacket(this, NC_OPTOOL_AUTH_ACK, ack.Data(), ack.Size()); break;
            }
            case NC_OPTOOL_BAN_CMD:  SHINELOG_WARN("optool: BAN");  break;
            case NC_OPTOOL_KICK_CMD: SHINELOG_WARN("optool: KICK"); break;
            default: break;
        }
    }
};
static IOCPSession* MakeOPToolSession() { return new OPToolSession(); }

class OperatorToolService : public WinService {
public:
    OperatorToolService() : WinService("FiestaOperatorTool") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("OPTool.Port", 30000), &MakeOPToolSession);
    }
    virtual void OnStop() { m_kAcceptor.Stop(); m_kIOCP.Stop(); }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::OperatorToolService s; return s.Run(argc, argv); }
