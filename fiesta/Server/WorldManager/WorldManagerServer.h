// Server/WorldManager/WorldManagerServer.h
// 05 -- world manager: routes between Login, Zone, optool, and the social/party servers.
// EVIDENCE: PDB_CONFIRMED  symbol: WorldManagerServer, WMClientSession, WMZoneSession,
//                                  WMLoginSession, WMCharDBSession, WMOPToolSession,
//                                  PartyFinderServer, RankingServer, ChatStealServer
#ifndef FIESTA_WMSERVER_H
#define FIESTA_WMSERVER_H
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/CToken.h"
#include <map>
#include <vector>

namespace fiesta {

struct ZoneInfo {
    uint16      uiZoneId;       // 0..4
    std::string kIp;
    uint16      uiPort;
    bool        bAlive;
    uint64      uiLastHeartbeatMs;
};

struct PendingTokenAuth {
    AccountID aid;
    uint32    uiWorldId;
    uint8     aSecret[16];
    uint64    uiIssuedMs;
};

class WMClientSession;
class WMZoneSession;

class WorldManagerServer {
public:
    static WorldManagerServer& Get();

    void RegisterZone(uint16 uiZoneId, const std::string& rIp, uint16 uiPort);
    const ZoneInfo* PickZoneForChar(CharID cid) const;

    // Token bridge (Login -> WM) (defined in WMLoginSession.cpp).
    void OnTokenIssuedFromLogin(const PendingTokenAuth& rTok);
    bool ConsumeToken(AccountID aid, const uint8 aSecret[16]);

    // Party finder (in WM). Per-player party state lives in Zone; PartyFinderServer in WM
    // brokers cross-zone party formation requests.
    class PartyFinderServer*  PartyFinder() { return m_pkPartyFinder; }
    class RankingServer*      Ranking()     { return m_pkRanking; }
    class ChatStealServer*    ChatSteal()   { return m_pkChatSteal; }
private:
    WorldManagerServer();
    std::map<uint16, ZoneInfo>          m_kZones;
    std::vector<PendingTokenAuth>       m_kPendingTokens;
    class PartyFinderServer*  m_pkPartyFinder;
    class RankingServer*      m_pkRanking;
    class ChatStealServer*    m_pkChatSteal;
    CRITICAL_SECTION                    m_kCs;
};

class PartyFinderServer { public: void Tick(); };
class RankingServer     { public: void Tick(); };
class ChatStealServer   { public: void Tick(); };

class WMClientSession : public IOCPSession {
public:
    virtual void OnConnect();
    virtual void OnDisconnect();
    virtual void OnPacket(const GPacket& rPkt);
private:
    AccountID m_uiAccount;
    CharID    m_uiActiveChar;
};

class WMZoneSession : public IOCPSession {
public:
    virtual void OnConnect();
    virtual void OnDisconnect();
    virtual void OnPacket(const GPacket& rPkt);
private:
    uint16 m_uiZoneId;
};

class WMLoginSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt);
};

class WMCharDBSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt);
};

class WMOPToolSession : public IOCPSession {
public:
    virtual void OnPacket(const GPacket& rPkt);
};

} // namespace fiesta
#endif
