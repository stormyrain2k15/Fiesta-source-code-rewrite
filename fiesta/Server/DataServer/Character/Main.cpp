// Server/DataServer/Character/Main.cpp
// 03 -- Character DB service exe entry.
// EVIDENCE: PDB_CONFIRMED  symbol: WMCharDBSession, GameDBSession
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

class CharDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("CharDB recv NC=0x%04X", rPkt.GetOpcode());
        // CharDataLoad / CharSave / CharNew / CharDel ack/req paths route here.
    }
};
static IOCPSession* MakeCharDBSession() { return new CharDBSession(); }

class CharacterDBService : public WinService {
public:
    CharacterDBService() : WinService("FiestaCharacterDB"), m_pkDB(NULL) {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        m_pkDB = new Database();
        m_pkDB->Connect(m_kInfo.GetString("Character.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=World00_Character;Trusted_Connection=yes;"));
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("Character.Port", 27602), &MakeCharDBSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        if (m_pkDB) { m_pkDB->Disconnect(); delete m_pkDB; m_pkDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor; Database* m_pkDB;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::CharacterDBService s; return s.Run(argc, argv); }
