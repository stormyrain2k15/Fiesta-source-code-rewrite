// Server/WorldManager/WorldManagerServer.h
// world manager: routes between Login, Zone, optool, and the social/party
// servers. Cross-zone content systems (Friend / ChatSteal / PartyFinder /
// Ranking / Prison / HolyPromise / Gamble / EventAttendance / DailyQuest /
// NpcSchedule / GMEvent / MatchInstanceDungeon / KQ / DataFile) live in
// WMServices.h and are ticked from WMServicesTickAll() each frame.
#ifndef FIESTA_WMSERVER_H
#define FIESTA_WMSERVER_H
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/CToken.h"
#include "WMServices.h"
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
    void TouchZoneHeartbeat(uint16 uiZoneId);
    const ZoneInfo* PickZoneForChar(CharID cid) const;

    // Live WMZoneSession registry. The WMZoneSession::OnPacket handler pushes
    // its IOCPSession in here on register, and yanks it on disconnect. Used
    // by the OPTool fanout to broadcast a packet to every connected zone, or
    // to target a single zone by ID.
    void RegisterZoneSession  (uint16 uiZoneId, IOCPSession* pkSess);
    void UnregisterZoneSession(IOCPSession* pkSess);
    void BroadcastToZones     (NCOpcode uiOp, const void* p, size_t n);
    bool SendToZone           (uint16 uiZoneId, NCOpcode uiOp, const void* p, size_t n);

    // Token bridge (Login -> WM) (defined in WMLoginSession.cpp).
    void OnTokenIssuedFromLogin(const PendingTokenAuth& rTok);
    bool ConsumeToken(AccountID aid, const uint8 aSecret[16]);

private:
    WorldManagerServer();
    std::map<uint16, ZoneInfo>          m_kZones;
    std::map<uint16, IOCPSession*>      m_kZoneSessions;   // zid -> WMZoneSession*
    std::vector<PendingTokenAuth>       m_kPendingTokens;
    CRITICAL_SECTION                    m_kCs;
};

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

// Admin / dev-tool acceptor on the WM. The OperatorTool exe itself does not
// ship in this tree (the original was a thin Win32 admin panel). The WM still
// listens for an external admin tool to connect -- but only over loopback,
// and only with a successful `p_Operator_Logon` against the OperatorTool DB.
// Once authed, the panel can drive every cross-DB / cross-zone admin action:
//   ban / kick / jail / unjail / sysmsg broadcast / give-item / take-item /
//   read-only DB query (any of Account / AccountLog / World00_Character /
//   World00_GameLog / OperatorTool / StatisticsData / Options).
// Nothing in the engine depends on the OPTool being connected; the session is
// purely opt-in.
class WMOPToolSession : public IOCPSession {
public:
    WMOPToolSession() : m_iOperLevel(0), m_bAuthed(false) {}
    virtual void OnPacket(const GPacket& rPkt);

    int  GetLevel() const { return m_iOperLevel; }
    bool IsAuthed() const { return m_bAuthed;   }
private:
    int  m_iOperLevel;
    bool m_bAuthed;
};

} // namespace fiesta
#endif
