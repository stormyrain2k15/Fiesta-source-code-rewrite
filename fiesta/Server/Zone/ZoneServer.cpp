// Server/Zone/ZoneServer.cpp
#include "ZoneServer.h"
#include "ItemUpgrade.h"
#include "LiveOpsBoosts.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../Common/ProtocolParser.h"

namespace fiesta {

ProtocolParser& GetZoneParser() { static ProtocolParser s; return s; }

ZoneServer& ZoneServer::Get() { static ZoneServer s; return s; }

ZoneServer::ZoneServer() : m_uiZoneId(0), m_uiNextHandle(1) {
    InitializeCriticalSection(&m_kCs);
}

bool ZoneServer::Init(uint16 zid) {
    m_uiZoneId = zid;
    SHINELOG_INFO("Zone%02u init", zid);
    return true;
}

void ZoneServer::Shutdown() {
    EnterCriticalSection(&m_kCs);
    for (std::map<Handle, ShinePlayer*>::iterator it = m_kPlayers.begin(); it != m_kPlayers.end(); ++it)
        delete it->second;
    m_kPlayers.clear();
    m_kObjects.clear();
    LeaveCriticalSection(&m_kCs);
}

void ZoneServer::Tick() {
    // 1Hz hook -- per-system tick fan-out happens here in pass-2 (BattleTick, AbStateTick, etc.).
    LiveOpsBoosts::Get().Tick();
}

Handle ZoneServer::NewObjectHandle() {
    EnterCriticalSection(&m_kCs);
    Handle h = m_uiNextHandle++;
    LeaveCriticalSection(&m_kCs);
    return h;
}

void ZoneServer::AttachPlayer(ShinePlayer* p) {
    if (!p) return;
    EnterCriticalSection(&m_kCs);
    if (p->GetHandle() == INVALID_HANDLE) p->SetHandle(m_uiNextHandle++);
    m_kPlayers[p->GetHandle()] = p;
    m_kObjects[p->GetHandle()] = p;
    LeaveCriticalSection(&m_kCs);
}

void ZoneServer::DetachPlayer(ShinePlayer* p) {
    if (!p) return;
    EnterCriticalSection(&m_kCs);
    m_kPlayers.erase(p->GetHandle());
    m_kObjects.erase(p->GetHandle());
    LeaveCriticalSection(&m_kCs);
}

void ZoneServer::RegisterObject(ShineObject* p) {
    if (!p) return;
    EnterCriticalSection(&m_kCs);
    if (p->GetHandle() == INVALID_HANDLE) p->SetHandle(m_uiNextHandle++);
    m_kObjects[p->GetHandle()] = p;
    LeaveCriticalSection(&m_kCs);
}

void ZoneServer::UnregisterObject(ShineObject* p) {
    if (!p) return;
    EnterCriticalSection(&m_kCs);
    m_kObjects.erase(p->GetHandle());
    LeaveCriticalSection(&m_kCs);
}

ShineObject* ZoneServer::FindObject(Handle h) {
    EnterCriticalSection(&m_kCs);
    std::map<Handle, ShineObject*>::iterator it = m_kObjects.find(h);
    ShineObject* pkOut = (it == m_kObjects.end()) ? NULL : it->second;
    LeaveCriticalSection(&m_kCs);
    return pkOut;
}

ShineMob* ZoneServer::FindMob(Handle h) {
    ShineObject* pk = FindObject(h);
    return (pk && pk->GetType() == OT_MOB) ? static_cast<ShineMob*>(pk) : NULL;
}

ShineNPC* ZoneServer::FindNPC(Handle h) {
    ShineObject* pk = FindObject(h);
    return (pk && pk->GetType() == OT_NPC) ? static_cast<ShineNPC*>(pk) : NULL;
}

ShinePet* ZoneServer::FindPet(Handle h) {
    ShineObject* pk = FindObject(h);
    return (pk && pk->GetType() == OT_PET) ? static_cast<ShinePet*>(pk) : NULL;
}

void ZoneServer::SnapshotObjects(std::vector<ShineObject*>& rOut) const {
    rOut.clear();
    rOut.reserve(m_kObjects.size());
    EnterCriticalSection(const_cast<CRITICAL_SECTION*>(&m_kCs));
    for (std::map<Handle, ShineObject*>::const_iterator it = m_kObjects.begin();
         it != m_kObjects.end(); ++it) {
        rOut.push_back(it->second);
    }
    LeaveCriticalSection(const_cast<CRITICAL_SECTION*>(&m_kCs));
}

ShinePlayer* ZoneServer::FindPlayerByCharID(CharID c) {
    EnterCriticalSection(&m_kCs);
    ShinePlayer* pkOut = NULL;
    for (std::map<Handle, ShinePlayer*>::iterator it = m_kPlayers.begin(); it != m_kPlayers.end(); ++it)
        if (it->second->GetCharID() == c) { pkOut = it->second; break; }
    LeaveCriticalSection(&m_kCs);
    return pkOut;
}

// ---------------- ClientSession ----------------
ClientSession::ClientSession() : m_uiAccount(0), m_pkPlayer(NULL), m_bAuthed(false) {}

void ClientSession::OnConnect() {
    SHINELOG_INFO("Zone client connected");
}

void ClientSession::OnDisconnect() {
    if (m_pkPlayer) {
        ItemUpgrade::CloseSession(m_pkPlayer);
        ZoneServer::Get().DetachPlayer(m_pkPlayer);
        delete m_pkPlayer; m_pkPlayer = NULL;
    }
}

void ClientSession::OnPacket(const GPacket& rPkt) {
    GetZoneParser().Dispatch(this, rPkt);
}

} // namespace fiesta
