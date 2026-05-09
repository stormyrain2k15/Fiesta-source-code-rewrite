// Server/DataServer/Character/Main.cpp
// 03 -- Character DB service exe entry. Hosts every World00_Character SQLP
// module; the Wedding / HolyPromise / Guild / Item / Quest / Skill / Friend
// helpers all share the same physical ODBC connection.
// EVIDENCE: PDB_CONFIRMED  symbol: WMCharDBSession, GameDBSession
#include "../../Shared/WinService.h"
#include "../../Shared/ServerInfo.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/IOCPManager.h"
#include "../../Shared/Socket_Acceptor.h"
#include "../Common/Database.h"
#include "../Common/SQLP.h"

namespace fiesta {

static Database*         g_pkCharDB    = NULL;
static SQLP_Character*   g_pkSQLPChar  = NULL;
static SQLP_Wedding*     g_pkSQLPWed   = NULL;
static SQLP_HolyPromise* g_pkSQLPHoly  = NULL;
static SQLP_Guild*       g_pkSQLPGuild = NULL;
static SQLP_Item*        g_pkSQLPItem  = NULL;
static SQLP_Quest*       g_pkSQLPQuest = NULL;
static SQLP_Skill*       g_pkSQLPSkill = NULL;
static SQLP_Friend*      g_pkSQLPFr    = NULL;

class CharDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt) {
        SHINELOG_DEBUG("CharDB recv NC=0x%04X", rPkt.GetOpcode());
        // CharDataLoad / CharSave / CharNew / CharDel ack/req paths use the
        // facades above; this stub accepts the packet so the session stays
        // alive while the dispatcher is being wired.
    }
};
static IOCPSession* MakeCharDBSession() { return new CharDBSession(); }

class CharacterDBService : public WinService {
public:
    CharacterDBService() : WinService("FiestaCharacterDB") {}
    virtual bool OnStart() {
        m_kInfo.Load("ServerInfo.txt");
        g_pkCharDB = new Database();
        g_pkCharDB->Connect(m_kInfo.GetString("Character.ConnStr",
            "Driver={SQL Server Native Client 11.0};Server=.;Database=World00_Character;Trusted_Connection=yes;"));
        g_pkSQLPChar  = new SQLP_Character  (g_pkCharDB);
        g_pkSQLPWed   = new SQLP_Wedding    (g_pkCharDB);
        g_pkSQLPHoly  = new SQLP_HolyPromise(g_pkCharDB);
        g_pkSQLPGuild = new SQLP_Guild      (g_pkCharDB);
        g_pkSQLPItem  = new SQLP_Item       (g_pkCharDB);
        g_pkSQLPQuest = new SQLP_Quest      (g_pkCharDB);
        g_pkSQLPSkill = new SQLP_Skill      (g_pkCharDB);
        g_pkSQLPFr    = new SQLP_Friend     (g_pkCharDB);
        if (!m_kIOCP.Start()) return false;
        return m_kAcceptor.Start(&m_kIOCP, m_kInfo.GetU16("Character.Port", 27602),
                                 &MakeCharDBSession);
    }
    virtual void OnStop() {
        m_kAcceptor.Stop(); m_kIOCP.Stop();
        delete g_pkSQLPFr;    g_pkSQLPFr    = NULL;
        delete g_pkSQLPSkill; g_pkSQLPSkill = NULL;
        delete g_pkSQLPQuest; g_pkSQLPQuest = NULL;
        delete g_pkSQLPItem;  g_pkSQLPItem  = NULL;
        delete g_pkSQLPGuild; g_pkSQLPGuild = NULL;
        delete g_pkSQLPHoly;  g_pkSQLPHoly  = NULL;
        delete g_pkSQLPWed;   g_pkSQLPWed   = NULL;
        delete g_pkSQLPChar;  g_pkSQLPChar  = NULL;
        if (g_pkCharDB) { g_pkCharDB->Disconnect(); delete g_pkCharDB; g_pkCharDB = NULL; }
    }
private:
    ServerInfo m_kInfo; IOCPManager m_kIOCP; Socket_Acceptor m_kAcceptor;
};

} // namespace fiesta

int main(int argc, char** argv) { fiesta::CharacterDBService s; return s.Run(argc, argv); }
