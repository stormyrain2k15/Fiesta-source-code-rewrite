// Server/WorldManager/FriendServer.cpp
// Friend list authority on the WM. The CharDB owns the persistent rows;
// FriendServer keeps an in-memory presence map so cross-zone online /
// offline / level-change / map-change deltas can be pushed without a DB
// round-trip every event.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "WMCharDBClient.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

FriendServer& FriendServer::Get() { static FriendServer s; return s; }

static void PushDelta(const std::set<CharID>& rTargets, NCOpcode uiOp,
                      CharID c, uint16 uiA, uint16 uiB) {
    if (rTargets.empty()) return;
    PacketBuffer body;
    body.WriteU16((uint16)rTargets.size());
    for (std::set<CharID>::const_iterator it = rTargets.begin(); it != rTargets.end(); ++it)
        body.WriteU32(*it);
    body.WriteU32(c); body.WriteU16(uiA); body.WriteU16(uiB);
    WorldManagerServer::Get().BroadcastToZones(uiOp, body.Data(), body.Size());
}

bool FriendServer::Add(CharID a, CharID b) {
    if (a == 0 || b == 0 || a == b) return false;
    m_kFriends[a].insert(b);
    m_kFriends[b].insert(a);
    bool ok = WMCharDBClient::Get().FriendAdd(a, b);
    SHINELOG_INFO("Friend add %u<->%u (db=%d)", a, b, ok ? 1 : 0);
    return ok;
}

bool FriendServer::Del(CharID a, CharID b) {
    std::map<CharID, std::set<CharID> >::iterator ia = m_kFriends.find(a);
    if (ia != m_kFriends.end()) ia->second.erase(b);
    std::map<CharID, std::set<CharID> >::iterator ib = m_kFriends.find(b);
    if (ib != m_kFriends.end()) ib->second.erase(a);
    return WMCharDBClient::Get().FriendDel(a, b);
}

void FriendServer::OnLogin(CharID c, uint16 uiMap, uint16 uiLevel) {
    Pres p; p.uiMap = uiMap; p.uiLevel = uiLevel; p.bOnline = true;
    m_kPresence[c] = p;
    std::map<CharID, std::set<CharID> >::iterator it = m_kFriends.find(c);
    if (it != m_kFriends.end()) PushDelta(it->second, NC_FRIEND_ONLINE_CMD, c, uiMap, uiLevel);
}

void FriendServer::OnLogout(CharID c) {
    std::map<CharID, Pres>::iterator p = m_kPresence.find(c);
    if (p != m_kPresence.end()) p->second.bOnline = false;
    std::map<CharID, std::set<CharID> >::iterator it = m_kFriends.find(c);
    if (it != m_kFriends.end()) PushDelta(it->second, NC_FRIEND_OFFLINE_CMD, c, 0, 0);
}

void FriendServer::OnLevelChange(CharID c, uint16 uiNewLevel) {
    std::map<CharID, Pres>::iterator p = m_kPresence.find(c);
    if (p != m_kPresence.end()) p->second.uiLevel = uiNewLevel;
    std::map<CharID, std::set<CharID> >::iterator it = m_kFriends.find(c);
    if (it != m_kFriends.end())
        PushDelta(it->second, NC_FRIEND_LIST_CMD, c,
                  (p != m_kPresence.end()) ? p->second.uiMap : 0, uiNewLevel);
}

void FriendServer::OnMapChange(CharID c, uint16 uiNewMap) {
    std::map<CharID, Pres>::iterator p = m_kPresence.find(c);
    if (p != m_kPresence.end()) p->second.uiMap = uiNewMap;
    std::map<CharID, std::set<CharID> >::iterator it = m_kFriends.find(c);
    if (it != m_kFriends.end())
        PushDelta(it->second, NC_FRIEND_LIST_CMD, c, uiNewMap,
                  (p != m_kPresence.end()) ? p->second.uiLevel : 0);
}

} // namespace fiesta
