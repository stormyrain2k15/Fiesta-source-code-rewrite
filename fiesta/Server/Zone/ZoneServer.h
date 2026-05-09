// Server/Zone/ZoneServer.h
// Zone server: holds Field/Layer/Map registry, all ShineObject lifecycles,
// every gameplay subsystem singleton.
#ifndef FIESTA_ZONE_ZONESERVER_H
#define FIESTA_ZONE_ZONESERVER_H
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/CToken.h"
#include "ShineObject.h"
#include <map>
#include <vector>

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

    // -------------------------------------------------------------------
    //  Unified ShineObject registry (PASS3-005).
    //  Players, mobs, NPCs, pets, and item-drops all live in
    //  m_kObjects keyed by their Handle. AttachPlayer mirrors into it
    //  automatically; mob/npc spawn paths call Register/Unregister
    //  directly. FindObject returns the base pointer; the caller can
    //  test GetType() before downcasting (or use FindMob/FindNPC).
    // -------------------------------------------------------------------
    void          RegisterObject  (ShineObject* p);
    void          UnregisterObject(ShineObject* p);
    ShineObject*  FindObject(Handle h);
    ShineMob*     FindMob   (Handle h);
    ShineNPC*     FindNPC   (Handle h);
    ShinePet*     FindPet   (Handle h);
    const std::map<Handle, ShineObject*>& Objects() const { return m_kObjects; }
    // Snapshot the entire object table into a flat vector; safe to walk
    // outside the critical section. Used by Lua bindings + AOE scans.
    void          SnapshotObjects(std::vector<ShineObject*>& rOut) const;
private:
    ZoneServer();
    uint16  m_uiZoneId;
    Handle  m_uiNextHandle;
    std::map<Handle, ShinePlayer*> m_kPlayers;
    std::map<Handle, ShineObject*> m_kObjects;
    CRITICAL_SECTION m_kCs;
};

} // namespace fiesta
#endif
