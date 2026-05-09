// Server/WorldManager/ChatStealServer.cpp
// World-wide chat fanout authority. RouteShout / RouteWorldYell broadcast
// to every connected zone via NC_INTER_BROADCAST_CMD; per-zone chat
// dispatchers then re-emit the proper NC_CHAT_*_CMD to local clients.
// Chat block / spam tracking is also held here so cross-zone whispers can
// be rejected without bouncing through the local zone's mute table.
#include "WMServices.h"
#include "WorldManagerServer.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include "../Shared/PacketBuffer.h"
#include "../Shared/GTimer.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

ChatStealServer& ChatStealServer::Get() { static ChatStealServer s; return s; }

static void Fan(uint8 uiKind, CharID from, const std::string& rChannel,
                const std::string& rText) {
    PacketBuffer body;
    body.WriteU8(uiKind);          // 0 = shout, 1 = world yell
    body.WriteU32(from);
    body.WriteString(rChannel);
    body.WriteString(rText);
    WorldManagerServer::Get().BroadcastToZones(NC_INTER_BROADCAST_CMD,
                                               body.Data(), body.Size());
}

void ChatStealServer::RouteShout(CharID from, const std::string& rChannel,
                                 const std::string& rText) {
    if (IsBlocked(from)) return;
    Fan(0, from, rChannel, rText);
}

void ChatStealServer::RouteWorldYell(CharID from, const std::string& rText) {
    if (IsBlocked(from)) return;
    Fan(1, from, std::string(), rText);
}

bool ChatStealServer::IsBlocked(CharID c) const {
    std::map<CharID, uint64>::const_iterator it = m_kBlockUntil.find(c);
    if (it == m_kBlockUntil.end()) return false;
    return GTimer::NowMillis() < it->second;
}

void ChatStealServer::Block(CharID c, uint32 uiDurationSecs) {
    m_kBlockUntil[c] = GTimer::NowMillis() + (uint64)uiDurationSecs * 1000ULL;
    SHINELOG_INFO("ChatBlock cid=%u for %u sec", c, uiDurationSecs);
}

void ChatStealServer::UnblockExpired() {
    uint64 uiNow = GTimer::NowMillis();
    for (std::map<CharID, uint64>::iterator it = m_kBlockUntil.begin();
         it != m_kBlockUntil.end(); ) {
        if (uiNow >= it->second) {
            std::map<CharID, uint64>::iterator del = it++;
            m_kBlockUntil.erase(del);
        } else ++it;
    }
}

// WMServices ticker entry for chat. Called from WMServicesTickAll().
} // namespace fiesta
