// Client/Network/ZoneSession.cpp
#include "ZoneSession.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

namespace shine {

ZoneSession::ZoneSession()
    : m_eState(ZS_IDLE), m_cbReady(NULL), m_cbPacket(NULL),
      m_cbFail(NULL), m_pkCbCtx(NULL) {
    ZeroMemory(&m_kPlayer, sizeof(m_kPlayer));
    ZeroMemory(&m_kHandoff, sizeof(m_kHandoff));
}

void ZoneSession::SetCallbacks(ZoneReadyCallback onReady,
                                ZonePacketCallback onPacket,
                                ZoneFailCallback   onFail,
                                void* pkCtx) {
    m_cbReady   = onReady;
    m_cbPacket  = onPacket;
    m_cbFail    = onFail;
    m_pkCbCtx   = pkCtx;
}

bool ZoneSession::Connect(const ZoneHandoff& rHandoff) {
    m_kHandoff = rHandoff;
    return m_kNet.Connect(rHandoff.kIp, rHandoff.uiPort, this);
}

void ZoneSession::Disconnect() {
    m_kNet.Disconnect();
    m_eState = ZS_IDLE;
}

// ── INetPacketSink ────────────────────────────────────────────────────────────

void ZoneSession::OnConnected() {
    SHINELOG_INFO("ZoneSession: connected");
    SendLoginReq();
}

void ZoneSession::OnDisconnected() {
    SHINELOG_WARN("ZoneSession: disconnected (state=%d)", (int)m_eState);
    if (m_eState != ZS_IN_WORLD && m_cbFail)
        m_cbFail(m_pkCbCtx, "Disconnected from zone server");
    m_eState = ZS_IDLE;
}

void ZoneSession::OnPacket(const GPacket& rPkt) {
    uint16 nc = (uint16)rPkt.GetOpcode();

    switch (nc) {
    case NC_CHAR_LOGIN_ACK:     HandleLoginAck(rPkt);  break;
    case NC_CHAR_INFO_CMD:            HandleCharInfo(rPkt);   break;
    case NC_CHAR_CLIENT_BASE_CMD:   HandleCharBase(rPkt);   break;
    case NC_CHAR_CLIENT_SHAPE_CMD:  HandleCharShape(rPkt);  break;
    case NC_CHAR_NEW_ACK:           HandleCharNewAck(rPkt); break;
    case NC_CHAR_DEL_ACK:           HandleCharDelAck(rPkt); break;
    case NC_MAP_WORLDTICK_CMD:  HandleWorldTick(rPkt); break;
    default:
        // Pass everything else up to the game layer
        if (m_cbPacket) m_cbPacket(m_pkCbCtx, rPkt);
        break;
    }
}

// ── Sends ─────────────────────────────────────────────────────────────────────

void ZoneSession::SendLoginReq() {
    // NC_CHAR_LOGIN_REQ: [accountId(4)][charId(4)][token(16)]
    PacketBuffer body;
    body.WriteU32(m_kHandoff.uiAccountId);
    body.WriteU32(m_kHandoff.uiCharId);
    body.WriteBytes(m_kHandoff.aToken, 16);
    m_kNet.SendPacket(NC_CHAR_LOGIN_REQ, body.Data(), body.Size());
    m_eState = ZS_WAIT_LOGIN_ACK;
    SHINELOG_INFO("ZoneSession: sent CHAR_LOGIN_REQ acct=%u char=%u",
                  m_kHandoff.uiAccountId, m_kHandoff.uiCharId);
}

void ZoneSession::SendMove(float fX, float fY, float fZ, uint16 uiDir) {
    if (m_eState != ZS_IN_WORLD) return;
    PacketBuffer body;
    body.WriteF32(fX);
    body.WriteF32(fY);
    body.WriteF32(fZ);
    body.WriteU16(uiDir);
    m_kNet.SendPacket(NC_ACT_MOVERUN_CMD, body.Data(), body.Size());
}

void ZoneSession::SendTickAck(uint32 uiTick) {
    PacketBuffer body;
    body.WriteU32(uiTick);
    m_kNet.SendPacket(NC_MAP_WORLDTICK_CMD, body.Data(), body.Size());
}

// ── Handlers ──────────────────────────────────────────────────────────────────

void ZoneSession::HandleLoginAck(const GPacket& rPkt) {
    // NC_CHAR_LOGIN_ACK: [ok(1)][handle(4)]
    PacketBuffer body = rPkt.Body();
    uint8 bOk = 0;
    body.ReadU8(bOk);
    body.ReadU32(m_kPlayer.uiHandle);

    if (!bOk) {
        SHINELOG_ERROR("ZoneSession: CHAR_LOGIN_ACK rejected");
        if (m_cbFail) m_cbFail(m_pkCbCtx, "Zone rejected character login");
        return;
    }

    SHINELOG_INFO("ZoneSession: login accepted handle=%u", m_kPlayer.uiHandle);
    m_eState = ZS_WAIT_INFO;
}

void ZoneSession::HandleCharInfo(const GPacket& rPkt) {
    // NC_CHAR_INFO_CMD: [handle(4)][level(2)][class(2)][hp(4)][maxhp(4)]
    //                   [sp(4)][maxsp(4)][posX(4f)][posY(4f)][posZ(4f)][mapId(2)]
    PacketBuffer body = rPkt.Body();
    body.ReadU32(m_kPlayer.uiHandle);
    body.ReadU16(m_kPlayer.uiLevel);
    body.ReadU16(m_kPlayer.uiClass);
    body.ReadI32(m_kPlayer.iHP);
    body.ReadI32(m_kPlayer.iMaxHP);
    body.ReadI32(m_kPlayer.iSP);
    body.ReadI32(m_kPlayer.iMaxSP);
    body.ReadF32(m_kPlayer.fPosX);
    body.ReadF32(m_kPlayer.fPosY);
    body.ReadF32(m_kPlayer.fPosZ);
    body.ReadU16(m_kPlayer.uiMapId);

    SHINELOG_INFO("ZoneSession: char info lv=%u class=%u hp=%d/%d pos=(%.1f,%.1f,%.1f) map=%u",
                  m_kPlayer.uiLevel, m_kPlayer.uiClass,
                  m_kPlayer.iHP, m_kPlayer.iMaxHP,
                  m_kPlayer.fPosX, m_kPlayer.fPosY, m_kPlayer.fPosZ,
                  m_kPlayer.uiMapId);

    m_eState = ZS_IN_WORLD;
    if (m_cbReady) m_cbReady(m_pkCbCtx, m_kPlayer);
}

void ZoneSession::HandleWorldTick(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    uint32 uiTick = 0;
    body.ReadU32(uiTick);
    SendTickAck(uiTick); // echo tick back to keep session alive
}

} // namespace shine

// Added handlers for char select screen packets
namespace shine {
void ZoneSession::HandleCharBase(const GPacket& rPkt) {
    // Route to AccountFrameWork which routes to ShineApp::OnCharBaseCmd
    // AccountFrameWork wired via ZoneSession callback in OnPacket above
    if (m_cbPacket) m_cbPacket(m_pkCbCtx, rPkt); // passes through to AccountFrameWork packet handler
}
void ZoneSession::HandleCharShape(const GPacket& rPkt) {
    if (m_cbPacket) m_cbPacket(rPkt);
}
void ZoneSession::HandleCharNewAck(const GPacket& rPkt) {
    if (m_cbPacket) m_cbPacket(rPkt);
}
void ZoneSession::HandleCharDelAck(const GPacket& rPkt) {
    if (m_cbPacket) m_cbPacket(rPkt);
}
} // namespace shine
