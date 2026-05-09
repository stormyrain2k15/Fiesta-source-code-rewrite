// Server/GamigoZR/Main.cpp
// 00 -- GamigoZR launcher / stub path. Brokers EU regional auth.
// EVIDENCE: PDB_CONFIRMED  symbol: GamigoZR
#include "../Shared/WinService.h"
#include "../Shared/ServerInfo.h"
#include "../Shared/IOCPManager.h"
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

class GamigoZRSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("GamigoZR recv NC=0x%04X", rPkt.GetOpcode());
    }
};
static IOCPSession* MakeGamigoZRSession() { return new GamigoZRSession(); }

class GamigoZRService : public WinService {
public:
    GamigoZRService() : WinService("FiestaGamigoZR") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("GamigoZR.Port", 29000), &MakeGamigoZRSession);
    }
    virtual void OnStop() { m_kAcceptor.Stop(); m_kIOCP.Stop(); }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::GamigoZRService s; return s.Run(argc, argv); }
