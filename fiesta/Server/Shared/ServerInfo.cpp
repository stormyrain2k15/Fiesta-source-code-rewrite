// Server/Shared/ServerInfo.cpp
#include "ServerInfo.h"
#include "ShineLogSystem.h"
#include <stdlib.h>

namespace fiesta {

std::string ServerInfo::s_kEmpty;

namespace {
    const ServerInfo* g_pCurrent = NULL;
}

const ServerInfo* ServerInfo::GetCurrent()              { return g_pCurrent; }
void              ServerInfo::SetCurrent(const ServerInfo* p) { g_pCurrent = p; }

ServerInfo::ServerInfo() : m_uiWorld(0) {}

bool ServerInfo::Load(const char* szPath) {
    if (!szPath || !*szPath) return false;
    if (!m_kCfg.Load(szPath)) return false;
    Snapshot();
    SHINELOG_INFO("ServerInfo: nation=%s world=%u(%s) services=%u odbc=%u",
                  m_kNation.c_str(), m_uiWorld, m_kWorldName.c_str(),
                  (uint32)m_kSvcs.size(), (uint32)m_kOdbc.size());
    return true;
}

static const std::string& StrAt(const CfgRecord& r, size_t i, const std::string& d) {
    return (i < r.size() && r[i].eType == CFT_STRING) ? r[i].kStr : d;
}
static int64 IntAt(const CfgRecord& r, size_t i) {
    return (i < r.size() && r[i].eType == CFT_INTEGER) ? r[i].iNum : 0;
}

void ServerInfo::Snapshot() {
    m_kSvcs.clear(); m_kOdbc.clear();

    if (const CfgRecord* p = m_kCfg.First("NATION_NAME")) m_kNation = StrAt(*p, 0, "");

    if (const CfgRecord* p = m_kCfg.First("WORLD_NAME")) {
        m_uiWorld    = (uint16)IntAt(*p, 0);
        m_kWorldName = StrAt(*p, 1, "");
        m_kWorldRoot = StrAt(*p, 2, "");
    }

    const std::vector<CfgRecord>& svc = m_kCfg.Records("SERVER_INFO");
    m_kSvcs.reserve(svc.size());
    for (size_t i = 0; i < svc.size(); ++i) {
        const CfgRecord& r = svc[i];
        ServiceEndpoint e;
        e.kTag       = StrAt(r, 0, "");
        e.eKind      = (ServiceKind)IntAt(r, 1);
        e.uiWorld    = (uint16)IntAt(r, 2);
        e.uiZone     = (uint16)IntAt(r, 3);
        e.iBindClass = (int32) IntAt(r, 4);
        e.kIp        = StrAt(r, 5, "127.0.0.1");
        e.uiPort     = (uint16)IntAt(r, 6);
        e.iMaxA      = (int32) IntAt(r, 7);
        e.iMaxB      = (int32) IntAt(r, 8);
        m_kSvcs.push_back(e);
    }

    const std::vector<CfgRecord>& od = m_kCfg.Records("ODBC_INFO");
    m_kOdbc.reserve(od.size());
    for (size_t i = 0; i < od.size(); ++i) {
        const CfgRecord& r = od[i];
        OdbcEntry o;
        o.kDbName  = StrAt(r, 0, "");
        o.iDbKind  = (int32)IntAt(r, 1);
        // r[2] = 0 reserved
        o.kConnStr = StrAt(r, 3, "");
        o.kPrelude = StrAt(r, 4, "");
        m_kOdbc.push_back(o);
    }
}

const ServiceEndpoint* ServerInfo::FindFirst(ServiceKind eKind, int iZone, int iBindClass) const {
    for (size_t i = 0; i < m_kSvcs.size(); ++i) {
        const ServiceEndpoint& e = m_kSvcs[i];
        if (e.eKind != eKind) continue;
        if (iZone >= 0 && (int)e.uiZone != iZone) continue;
        if (iBindClass >= 0 && e.iBindClass != iBindClass) continue;
        return &e;
    }
    return NULL;
}

const OdbcEntry* ServerInfo::FindOdbc(const std::string& rName) const {
    for (size_t i = 0; i < m_kOdbc.size(); ++i)
        if (m_kOdbc[i].kDbName == rName) return &m_kOdbc[i];
    return NULL;
}

// Back-compat for old code that did GetInt("Login.Port", 9010) etc.
// Maps a few well-known keys onto the new typed API.
const std::string& ServerInfo::GetString(const std::string& rKey, const std::string& rDef) const {
    if (rKey == "Data.Root") { return m_kWorldRoot.empty() ? (s_kEmpty = rDef) : m_kWorldRoot; }
    s_kEmpty = rDef; return s_kEmpty;
}
int ServerInfo::GetInt(const std::string& rKey, int nDef) const {
    if (rKey == "Login.Port"        || rKey == "Login.PublicPort") {
        const ServiceEndpoint* p = FindFirst(SK_Login, -1, 20); return p ? p->uiPort : nDef;
    }
    if (rKey == "WM.ClientPort") {
        const ServiceEndpoint* p = FindFirst(SK_WorldManager, -1, 20); return p ? p->uiPort : nDef;
    }
    if (rKey == "Zone.ClientPort") {
        const ServiceEndpoint* p = FindFirst(SK_Zone, -1, 20); return p ? p->uiPort : nDef;
    }
    if (rKey == "Account.Port") {
        const ServiceEndpoint* p = FindFirst(SK_AccountDB); return p ? p->uiPort : nDef;
    }
    if (rKey == "AccountLog.Port") {
        const ServiceEndpoint* p = FindFirst(SK_AccountLogDB); return p ? p->uiPort : nDef;
    }
    if (rKey == "Character.Port") {
        const ServiceEndpoint* p = FindFirst(SK_CharacterDB); return p ? p->uiPort : nDef;
    }
    if (rKey == "GameLog.Port") {
        const ServiceEndpoint* p = FindFirst(SK_GameLogDB); return p ? p->uiPort : nDef;
    }
    if (rKey == "Zone.Id") return 0;
    return nDef;
}
uint16 ServerInfo::GetU16(const std::string& rKey, uint16 uDef) const {
    return (uint16)GetInt(rKey, (int)uDef);
}

} // namespace fiesta
