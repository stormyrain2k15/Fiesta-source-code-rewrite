// Server/Zone/ZoneServer.h
// Zone server: holds Field/Layer/Map registry, all ShineObject lifecycles,
// every gameplay subsystem singleton.
#ifndef FIESTA_ZONE_ZONESERVER_H
#define FIESTA_ZONE_ZONESERVER_H
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/CToken.h"
#include "ShineObject.h"
#include <map>

namespace fiesta {

class ClientSession : public IOCPSession {
public:
    ClientSession();
    virtual void OnConnect();
    virtual void OnDisconnect();
    virtual void OnPacket(const GPacket& rPkt);

    ShinePlayer* GetPlayer() { return m_pkPlayer; }
    AccountID    GetAccount() const { return m_uiAccount; }
private:
    AccountID    m_uiAccount;
    ShinePlayer* m_pkPlayer;
    bool         m_bAuthed;
};

class ZoneServer {
public:
    static ZoneServer& Get();

    bool   Init(uint16 uiZoneId);
    void   Shutdown();
    void   Tick();

    uint16 ZoneId() const { return m_uiZoneId; }
    Handle NewObjectHandle();

    void   AttachPlayer (ShinePlayer* p);
    void   DetachPlayer (ShinePlayer* p);
    ShinePlayer* FindPlayerByCharID(CharID c);
    const std::map<Handle, ShinePlayer*>& Players() const { return m_kPlayers; }
private:
    ZoneServer();
    uint16  m_uiZoneId;
    Handle  m_uiNextHandle;
    std::map<Handle, ShinePlayer*> m_kPlayers;
    CRITICAL_SECTION m_kCs;
};

} // namespace fiesta
#endif
