// Client/Network/WMSession.cpp
#include "WMSession.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

namespace shine {

WMSession::WMSession()
    : m_eState(WMS_IDLE), m_cbSuccess(NULL), m_cbFail(NULL), m_pkCbCtx(NULL) {
    ZeroMemory(&m_kHandoff, sizeof(m_kHandoff));
}

void WMSession::SetCallbacks(WMSuccessCallback onSuccess, WMFailCallback onFail, void* pkCtx) {
    m_cbSuccess = onSuccess;
    m_cbFail    = onFail;
    m_pkCbCtx   = pkCtx;
}

bool WMSession::Connect(const WMHandoff& rHandoff) {
    m_kHandoff = rHandoff;
    return m_kNet.Connect(rHandoff.kIp, rHandoff.uiPort, this);
}

void WMSession::Disconnect() {
    m_kNet.Disconnect();
    m_eState = WMS_IDLE;
}

// ── INetPacketSink ────────────────────────────────────────────────────────────

void WMSession::OnConnected() {
    SHINELOG_INFO("WMSession: connected");
    SendAuthReq();
}

void WMSession::OnDisconnected() {
    SHINELOG_WARN("WMSession: disconnected (state=%d)", (int)m_eState);
    if (m_eState != WMS_DONE && m_cbFail)
        m_cbFail(m_pkCbCtx, "Disconnected from WorldManager");
    m_eState = WMS_IDLE;
}

void WMSession::OnPacket(const GPacket& rPkt) {
    uint16 nc = (uint16)rPkt.GetOpcode();
    switch (nc) {
    case NC_CHAR_LOGIN_ACK:         HandleAuthAck(rPkt);     break;
    case NC_WM_ZONE_ASSIGN_CMD:     HandleZoneAssign(rPkt);  break;
    default:
        SHINELOG_DEBUG("WMSession: unhandled NC=0x%04X", nc);
        break;
    }
}

// ── Sends ─────────────────────────────────────────────────────────────────────

void WMSession::SendAuthReq() {
    // NC_CHAR_LOGIN_REQ: [accountId(4)][token(16)]
    PacketBuffer body;
    body.WriteU32(m_kHandoff.uiAccountId);
    body.WriteBytes(m_kHandoff.aToken, 16);
    m_kNet.SendPacket(NC_CHAR_LOGIN_REQ, body.Data(), body.Size());
    m_eState = WMS_WAIT_AUTH_ACK;
    SHINELOG_INFO("WMSession: sent auth req acct=%u", m_kHandoff.uiAccountId);
}

void WMSession::SendCharSelectReq() {
    // NC_WM_CHARSELECT_REQ: [charId(4)]
    // WM uses this to assign the player to the correct zone
    PacketBuffer body;
    body.WriteU32(m_kHandoff.uiCharId);
    m_kNet.SendPacket(NC_WM_CHARSELECT_REQ, body.Data(), body.Size());
    m_eState = WMS_WAIT_ZONE_ASSIGN;
    SHINELOG_INFO("WMSession: sent charselect charId=%u", m_kHandoff.uiCharId);
}

// ── Handlers ──────────────────────────────────────────────────────────────────

void WMSession::HandleAuthAck(const GPacket& rPkt) {
    // NC_CHAR_LOGIN_ACK: [ok(1)]
    PacketBuffer body = rPkt.Body();
    uint8 bOk = 0;
    body.ReadU8(bOk);

    if (!bOk) {
        SHINELOG_ERROR("WMSession: auth rejected");
        if (m_cbFail) m_cbFail(m_pkCbCtx, "WorldManager rejected auth token");
        return;
    }

    SHINELOG_INFO("WMSession: auth accepted");
    SendCharSelectReq();
}

void WMSession::HandleZoneAssign(const GPacket& rPkt) {
    // NC_WM_ZONE_ASSIGN_CMD: [zoneIp(str)][zonePort(2)][zoneToken(16)]
    PacketBuffer body = rPkt.Body();

    ZoneHandoff zh;
    zh.uiAccountId = m_kHandoff.uiAccountId;
    zh.uiCharId    = m_kHandoff.uiCharId;
    body.ReadString(zh.kIp);
    body.ReadU16(zh.uiPort);
    body.ReadBytes(zh.aToken, 16);

    SHINELOG_INFO("WMSession: zone assigned %s:%u", zh.kIp.c_str(), zh.uiPort);
    m_eState = WMS_DONE;

    if (m_cbSuccess) m_cbSuccess(m_pkCbCtx, zh);
    // Caller connects ZoneSession then calls Disconnect() on us
}

} // namespace shine
