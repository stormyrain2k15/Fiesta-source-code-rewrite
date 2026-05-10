// Client/Network/LoginSession.cpp
#include "LoginSession.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

namespace fiesta {

LoginSession::LoginSession()
    : m_eState(LS_IDLE), m_uiAccountId(0),
      m_cbSuccess(NULL), m_cbFail(NULL), m_pkCbCtx(NULL)
{}

void LoginSession::SetCredentials(const std::string& rUser, const std::string& rPass) {
    m_kUser = rUser;
    m_kPass = rPass;
}

void LoginSession::SetCallbacks(LoginSuccessCallback onSuccess,
                                LoginFailCallback onFail,
                                void* pkCtx) {
    m_cbSuccess = onSuccess;
    m_cbFail    = onFail;
    m_pkCbCtx   = pkCtx;
}

// ── INetPacketSink ────────────────────────────────────────────────────────────

void LoginSession::OnConnected() {
    SHINELOG_INFO("LoginSession: connected");
    m_eState = LS_WAIT_SEED;
    // Server sends NC_USER_SEED_ACK immediately on connect before we speak.
    // We wait for it before sending anything.
}

void LoginSession::OnDisconnected() {
    SHINELOG_INFO("LoginSession: disconnected (state=%d)", (int)m_eState);
    if (m_eState != LS_DONE && m_cbFail)
        m_cbFail(m_pkCbCtx, "Disconnected from login server");
}

void LoginSession::OnPacket(const GPacket& rPkt) {
    uint16 nc = (uint16)rPkt.GetOpcode();
    SHINELOG_DEBUG("LoginSession: NC=0x%04X state=%d", nc, (int)m_eState);

    switch (nc) {
    case NC_USER_SEED_ACK:                              HandleSeedAck(rPkt);        break;
    case NC_USER_CLIENT_VERSION_CHECK_REQ + 1:          HandleVersionAck(rPkt);     break;
    case NC_USER_XTRAP_ACK:                             /* legacy no-op, ignore */  break;
    case NC_USER_WORLD_STATUS_ACK:                      HandleWorldStatusAck(rPkt); break;
    case NC_USER_LOGIN_ACK:                             HandleLoginAck(rPkt);       break;
    case NC_USER_LOGINFAIL_ACK:                         HandleLoginFail(rPkt);      break;
    case NC_USER_WORLDSELECT_ACK:                       HandleWorldSelectAck(rPkt); break;
    default:
        SHINELOG_WARN("LoginSession: unhandled NC=0x%04X", nc);
        break;
    }
}

// ── Sends ─────────────────────────────────────────────────────────────────────

void LoginSession::SendVersionReq() {
    // NC_USER_VERSION_REQ: [string clientBuildToken]
    // We send "Shine" as our build token. Server has version check disabled
    // by default (LoginClientSession.cpp) so any token passes.
    PacketBuffer body;
    body.WriteString("Shine");
    m_kNet.SendPacket(NC_USER_VERSION_REQ, body.Data(), body.Size());
    m_eState = LS_WAIT_VERSION_ACK;
    SHINELOG_INFO("LoginSession: sent VERSION_REQ");
}

void LoginSession::SendWorldStatusReq() {
    m_kNet.SendPacket(NC_USER_WORLD_STATUS_REQ);
    m_eState = LS_WAIT_WORLD_STATUS;
    SHINELOG_INFO("LoginSession: sent WORLD_STATUS_REQ");
}

void LoginSession::SendLoginReq() {
    // NC_USER_LOGIN_REQ: [string user][string pass]
    PacketBuffer body;
    body.WriteString(m_kUser);
    body.WriteString(m_kPass);
    m_kNet.SendPacket(NC_USER_LOGIN_REQ, body.Data(), body.Size());
    m_eState = LS_WAIT_LOGIN_ACK;
    SHINELOG_INFO("LoginSession: sent LOGIN_REQ for '%s'", m_kUser.c_str());
}

void LoginSession::SendWorldSelectReq(uint16 uiWorldId) {
    PacketBuffer body;
    body.WriteU16(uiWorldId);
    m_kNet.SendPacket(NC_USER_WORLDSELECT_REQ, body.Data(), body.Size());
    m_eState = LS_WAIT_WORLDSELECT_ACK;
    SHINELOG_INFO("LoginSession: sent WORLDSELECT_REQ world=%u", uiWorldId);
}

// ── Handlers ──────────────────────────────────────────────────────────────────

void LoginSession::HandleSeedAck(const GPacket& rPkt) {
    // NC_USER_SEED_ACK: [uint32 seed]
    PacketBuffer body = rPkt.Body();
    uint32 uiSeed = 0;
    body.ReadU32(uiSeed);
    m_kNet.InitCipher(uiSeed);
    SHINELOG_INFO("LoginSession: cipher seed 0x%08X", uiSeed);
    // Now send our version hello
    SendVersionReq();
}

void LoginSession::HandleVersionAck(const GPacket& rPkt) {
    // Version accepted. Get world list then send credentials.
    SHINELOG_INFO("LoginSession: version accepted");
    SendWorldStatusReq();
}

void LoginSession::HandleWorldStatusAck(const GPacket& rPkt) {
    // NC_USER_WORLD_STATUS_ACK: [uint8 count]{[uint16 id][string name][uint8 status][uint8 pvp]}
    PacketBuffer body = rPkt.Body();
    uint8 uiCount = 0;
    body.ReadU8(uiCount);
    m_kWorlds.clear();
    for (uint8 i = 0; i < uiCount; ++i) {
        WorldInfo w;
        body.ReadU16(w.uiId);
        body.ReadString(w.kName);
        body.ReadU8(w.uiStatus);
        body.ReadU8(w.uiPvP);
        m_kWorlds.push_back(w);
        SHINELOG_INFO("LoginSession: world[%u] id=%u name='%s' status=%u",
                      i, w.uiId, w.kName.c_str(), w.uiStatus);
    }
    // Now send login credentials
    SendLoginReq();
}

void LoginSession::HandleLoginAck(const GPacket& rPkt) {
    // NC_USER_LOGIN_ACK: [uint8 ok][uint32 accountId]
    PacketBuffer body = rPkt.Body();
    uint8 bOk = 0; body.ReadU8(bOk);
    body.ReadU32(m_uiAccountId);
    SHINELOG_INFO("LoginSession: login ok=%u acctId=%u", bOk, m_uiAccountId);
    // Select first available world
    if (!m_kWorlds.empty()) {
        SendWorldSelectReq(m_kWorlds[0].uiId);
    } else {
        if (m_cbFail) m_cbFail(m_pkCbCtx, "No worlds available");
    }
}

void LoginSession::HandleLoginFail(const GPacket& rPkt) {
    SHINELOG_WARN("LoginSession: login rejected");
    if (m_cbFail) m_cbFail(m_pkCbCtx, "Invalid username or password");
}

void LoginSession::HandleWorldSelectAck(const GPacket& rPkt) {
    // NC_USER_WORLDSELECT_ACK: [uint8 ok][string wmIp][uint16 wmPort][bytes[16] token]
    PacketBuffer body = rPkt.Body();
    uint8 bOk = 0; body.ReadU8(bOk);

    WMHandoff handoff;
    handoff.uiAccountId = m_uiAccountId;
    body.ReadString(handoff.kIp);
    body.ReadU16(handoff.uiPort);
    body.ReadBytes(handoff.aToken, 16);

    SHINELOG_INFO("LoginSession: world selected WM=%s:%u", handoff.kIp.c_str(), handoff.uiPort);
    m_eState = LS_DONE;

    if (m_cbSuccess) m_cbSuccess(m_pkCbCtx, handoff);
    // Caller should now connect to WM using the handoff data.
    // We stay connected until caller calls Disconnect().
}

} // namespace fiesta
