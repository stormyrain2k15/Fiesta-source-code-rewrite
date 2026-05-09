// Server/WorldManager/WorldManagerServer.cpp
#include "WorldManagerServer.h"
#include "../Shared/ShineLogSystem.h"
#include "../Shared/GTimer.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"
#include <string.h>

namespace fiesta {

void PartyFinderServer::Tick() {}
void RankingServer    ::Tick() {}
void ChatStealServer  ::Tick() {}

WorldManagerServer& WorldManagerServer::Get() { static WorldManagerServer s; return s; }

WorldManagerServer::WorldManagerServer()
    : m_pkPartyFinder(new PartyFinderServer()),
      m_pkRanking    (new RankingServer    ()),
      m_pkChatSteal  (new ChatStealServer  ()) {
    InitializeCriticalSection(&m_kCs);
}

void WorldManagerServer::RegisterZone(uint16 zid, const std::string& ip, uint16 port) {
    EnterCriticalSection(&m_kCs);
    ZoneInfo z; z.uiZoneId = zid; z.kIp = ip; z.uiPort = port; z.bAlive = true; z.uiLastHeartbeatMs = GTimer::NowMillis();
    m_kZones[zid] = z;
    LeaveCriticalSection(&m_kCs);
    SHINELOG_INFO("WM: Zone%02u registered at %s:%u", zid, ip.c_str(), port);
}

const ZoneInfo* WorldManagerServer::PickZoneForChar(CharID cid) const {
    if (m_kZones.empty()) return NULL;
    // Provisional: round-robin by char id. Replace with map-based dispatch in Zone enter.
    uint16 zid = (uint16)(cid % (CharID)m_kZones.size());
    std::map<uint16, ZoneInfo>::const_iterator it = m_kZones.find(zid);
    return (it == m_kZones.end()) ? NULL : &it->second;
}

void WorldManagerServer::OnTokenIssuedFromLogin(const PendingTokenAuth& tok) {
    EnterCriticalSection(&m_kCs);
    m_kPendingTokens.push_back(tok);
    LeaveCriticalSection(&m_kCs);
}

bool WorldManagerServer::ConsumeToken(AccountID aid, const uint8 aSecret[16]) {
    bool bOk = false;
    EnterCriticalSection(&m_kCs);
    for (size_t i = 0; i < m_kPendingTokens.size(); ++i) {
        if (m_kPendingTokens[i].aid == aid &&
            memcmp(m_kPendingTokens[i].aSecret, aSecret, 16) == 0) {
            m_kPendingTokens.erase(m_kPendingTokens.begin() + i);
            bOk = true; break;
        }
    }
    LeaveCriticalSection(&m_kCs);
    return bOk;
}

// --------------- WMClientSession ---------------
void WMClientSession::OnConnect()    { m_uiAccount = 0; m_uiActiveChar = 0; SHINELOG_INFO("WM: client connected"); }
void WMClientSession::OnDisconnect() { SHINELOG_INFO("WM: client disconnect aid=%u", m_uiAccount); }

void WMClientSession::OnPacket(const GPacket& rPkt) {
    switch (rPkt.GetOpcode()) {
        case NC_CHAR_LOGIN_REQ: {
            // body = [account_id u32][16-byte token secret]
            PacketBuffer b = rPkt.Body();
            uint32 aid = 0; b.ReadU32(aid);
            uint8  sec[16]; b.ReadBytes(sec, 16);
            if (!WorldManagerServer::Get().ConsumeToken(aid, sec)) {
                SHINELOG_WARN("WM: bad token aid=%u", aid);
                Close(); return;
            }
            m_uiAccount = aid;
            PacketBuffer ack; ack.WriteU8(1);
            SendPacket(this, NC_CHAR_LOGIN_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_CHAR_LOGOUT_CMD:
            Close(); break;
        default:
            SHINELOG_DEBUG("WM-client recv NC=0x%04X", rPkt.GetOpcode());
            break;
    }
}

// --------------- WMZoneSession ---------------
void WMZoneSession::OnConnect()    { m_uiZoneId = 0xFFFF; }
void WMZoneSession::OnDisconnect() { SHINELOG_INFO("WM: Zone%02u disconnected", m_uiZoneId); }

void WMZoneSession::OnPacket(const GPacket& rPkt) {
    switch (rPkt.GetOpcode()) {
        case NC_INTER_ZONE_REGISTER_REQ: {
            PacketBuffer b = rPkt.Body();
            uint16 zid = 0; b.ReadU16(zid);
            std::string ip; b.ReadString(ip);
            uint16 port = 0; b.ReadU16(port);
            m_uiZoneId = zid;
            WorldManagerServer::Get().RegisterZone(zid, ip, port);
            PacketBuffer ack; ack.WriteU8(1); SendPacket(this, NC_INTER_HELLO_ACK, ack.Data(), ack.Size());
            break;
        }
        case NC_INTER_ZONE_HEARTBEAT_CMD:
            // touch alive
            break;
        default: break;
    }
}

// --------------- WMLoginSession ---------------
void WMLoginSession::OnPacket(const GPacket& rPkt) {
    if (rPkt.GetOpcode() == NC_INTER_AUTH_TOKEN_PUSH) {
        PendingTokenAuth tok;
        PacketBuffer b = rPkt.Body();
        uint32 aid = 0; uint16 wid = 0;
        b.ReadU32(aid); b.ReadU16(wid);
        b.ReadBytes(tok.aSecret, 16);
        tok.aid = aid; tok.uiWorldId = wid; tok.uiIssuedMs = GTimer::NowMillis();
        WorldManagerServer::Get().OnTokenIssuedFromLogin(tok);
    }
}

// --------------- WMCharDBSession ---------------
void WMCharDBSession::OnPacket(const GPacket& rPkt) {
    SHINELOG_DEBUG("WM<-CharDB NC=0x%04X", rPkt.GetOpcode());
}

// --------------- WMOPToolSession ---------------
void WMOPToolSession::OnPacket(const GPacket& rPkt) {
    if (rPkt.GetOpcode() == NC_OPTOOL_AUTH_REQ) {
        PacketBuffer ack; ack.WriteU8(1); SendPacket(this, NC_OPTOOL_AUTH_ACK, ack.Data(), ack.Size());
    }
}

} // namespace fiesta
