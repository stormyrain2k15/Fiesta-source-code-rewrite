// Server/Shared/ServerInfo.h
// typed wrapper over ConfigParser exposing the original-format records:
//   NATION_NAME, WORLD_NAME, SERVER_INFO, ODBC_INFO, MY_SERVER.
// SERVER_INFO record fields (positional per the project's #DEFINE):
//   0 : <STRING>  service tag       e.g. "PG_Login", "PG_W00_Z00"
//   1 : <INTEGER> service kind      0=AccDB 1=ALDB 2=CDB 3=GLDB 4=Login 5=WM 6=Zone
//   2 : <INTEGER> world id          0..N
//   3 : <INTEGER> zone id           0..N (or 0 for non-zone services)
//   4 : <INTEGER> bind class        20=public, 5/6/8=loopback variants
//   5 : <STRING>  ip                "127.0.0.1" / public ip
//   6 : <INTEGER> port              tcp port
//   7 : <INTEGER> max clients       (interpretation per service)
//   8 : <INTEGER> max records / cap (interpretation per service)
// ODBC_INFO record fields:
//   0 : <STRING>  db logical name   "Account", "World00_Character", ...
//   1 : <INTEGER> db kind id        editor-internal grouping
//   2 : <INTEGER> 0
//   3 : <STRING>  ODBC connect str  "DRIVER={SQL Server};SERVER=..;UID=..;PWD=.."
//   4 : <STRING>  prelude SQL       "USE <db>; SET LOCK_TIMEOUT 5000"
#ifndef FIESTA_SHARED_SERVERINFO_H
#define FIESTA_SHARED_SERVERINFO_H
#include "ShineTypes.h"
#include "ConfigParser.h"

namespace fiesta {

enum ServiceKind {
    SK_AccountDB     = 0,
    SK_AccountLogDB  = 1,
    SK_CharacterDB   = 2,
    SK_GameLogDB     = 3,
    SK_Login         = 4,
    SK_WorldManager  = 5,
    SK_Zone          = 6
};

struct ServiceEndpoint {
    std::string  kTag;
    ServiceKind  eKind;
    uint16       uiWorld;
    uint16       uiZone;
    int32        iBindClass;
    std::string  kIp;
    uint16       uiPort;
    int32        iMaxA;
    int32        iMaxB;
};

struct OdbcEntry {
    std::string  kDbName;
    int32        iDbKind;
    std::string  kConnStr;
    std::string  kPrelude;
};

class ServerInfo {
public:
    ServerInfo();

    // Loads ServerInfo.txt (or LoginServerInfo.txt etc.) and resolves #include directives.
    bool Load(const char* szPath);

    const std::string&  Nation()  const { return m_kNation; }
    uint16              WorldId() const { return m_uiWorld; }
    const std::string&  WorldName() const { return m_kWorldName; }
    const std::string&  WorldRoot() const { return m_kWorldRoot; }

    const std::vector<ServiceEndpoint>& Services() const { return m_kSvcs; }
    const std::vector<OdbcEntry>&       OdbcEntries() const { return m_kOdbc; }

    // Convenience: first matching SERVER_INFO of (kind, [world,] [zone,] [bind]).
    const ServiceEndpoint* FindFirst(ServiceKind eKind, int iZone = -1, int iBindClass = -1) const;

    // Convenience: ODBC connection string by db logical name.
    const OdbcEntry* FindOdbc(const std::string& rDbName) const;

    // Back-compat key=value getters (used by simpler service mains).
    int    GetInt   (const std::string& rKey, int    nDef = 0) const;
    uint16 GetU16   (const std::string& rKey, uint16 uDef = 0) const;
    const std::string& GetString(const std::string& rKey, const std::string& rDef = "") const;

    // Process-wide singleton handle. Set by service mains via
    // SetCurrent() right after Load() succeeds; consumed by classes
    // (LoginClientSession, ZoneHandlers, etc.) that need to look up a
    // service endpoint without being passed a ServerInfo* through
    // every constructor. Returns NULL until the service main calls
    // SetCurrent(this).
    static const ServerInfo* GetCurrent();
    static void              SetCurrent(const ServerInfo* p);
private:
    void   Snapshot();
    ConfigParser  m_kCfg;
    std::string   m_kNation;
    uint16        m_uiWorld;
    std::string   m_kWorldName;
    std::string   m_kWorldRoot;
    std::vector<ServiceEndpoint>  m_kSvcs;
    std::vector<OdbcEntry>        m_kOdbc;
    static std::string s_kEmpty;
};

} // namespace fiesta
#endif
