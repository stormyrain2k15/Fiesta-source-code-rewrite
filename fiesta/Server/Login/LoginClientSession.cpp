// Server/Login/LoginClientSession.cpp
#include "LoginClientSession.h"
#include "ClientVersionKeyInfo.h"
#include "../Shared/ShineLogSystem.h"
#include "../Common/NETCOMMAND.h"
#include "../Common/SendPacket.h"

namespace fiesta {

LoginClientSession::LoginClientSession() : m_eState(LS_NEW), m_uiAccount(0) {}

void LoginClientSession::OnConnect() {
    SHINELOG_INFO("Login: client connected");
}

void LoginClientSession::OnDisconnect() {
    SHINELOG_INFO("Login: client disconnect (acct=%u state=%d)", m_uiAccount, (int)m_eState);
}

void LoginClientSession::OnPacket(const GPacket& rPkt) {
    switch (rPkt.GetOpcode()) {
        case NC_USER_VERSION_REQ:              HandleVersionCheck(rPkt); break;
        case NC_USER_XTRAP_REQ:                HandleXTrapLegacy(rPkt);  break; // legacy no-op
        case NC_USER_LOGIN_REQ:                HandleLogin(rPkt);        break;
        case NC_USER_WORLD_STATUS_REQ:         HandleWorldStatus(rPkt);  break;
        case NC_USER_WORLDSELECT_REQ:          HandleWorldSelect(rPkt);  break;
        case NC_USER_LOGOUT_REQ:
            SendPacket(this, NC_USER_LOGOUT_ACK); Close(); break;
        default:
            SHINELOG_WARN("Login: unknown NC=0x%04X", rPkt.GetOpcode()); break;
    }
}

// ---------------------------------------------------------------------------
// Client-version / build-token check.
//
// Disabled by default. The original game shipped without this check for
// most of its lifetime, only the WorldManager ever consulted the build key,
// and even there the gate was advisory: if WM accepted the connection
// nothing downstream re-validated. We follow the same posture here -- the
// packet is acknowledged blindly and the session is moved into LS_VERSION_OK
// so the auth flow continues. Re-enable below ("MUST CONFIGURE TO ENABLE")
// only for hardened deployments that actually ship `ClientVersionKeyInfo.txt`.
// ---------------------------------------------------------------------------
void LoginClientSession::HandleVersionCheck(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    std::string clientKey;
    body.ReadString(clientKey);

    /*  MUST CONFIGURE TO ENABLE -- security gate, off by default.
    if (!ClientVersionKeyInfo::Get().IsAcceptable(clientKey)) {
        SHINELOG_WARN("Login: version mismatch got='%s' expected='%s'",
                      clientKey.c_str(), ClientVersionKeyInfo::Get().Key().c_str());
        SendPacket(this, NC_USER_LOGINFAIL_ACK);
        return;
    }
    */
    (void)clientKey; // accept any value while the gate is disabled

    m_eState = LS_VERSION_OK;
    PacketBuffer ack; ack.WriteU8(1);
    SendPacket(this, NC_USER_CLIENT_VERSION_CHECK_REQ + 1, ack.Data(), ack.Size());
}

void LoginClientSession::HandleXTrap(const GPacket& rPkt) {
    // 40 -- XTrap handshake. Acknowledge OK; full DLL ack flow in XTrap4Server.
    m_eState = (m_eState >= LS_VERSION_OK) ? LS_XTRAP_OK : m_eState;
    PacketBuffer ack; ack.WriteU8(1); SendPacket(this, NC_USER_XTRAP_ACK, ack.Data(), ack.Size());
}

void LoginClientSession::HandleLogin(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    std::string user, pass;
    body.ReadString(user); body.ReadString(pass);
    m_kAccountName = user;
    // Real verify -> AccountDB SQLP_Account::VerifyLogin; provisional accept-all stub.
    m_uiAccount = (AccountID)(0x10000000u ^ (uint32)user.size());
    m_eState = LS_AUTHED;
    PacketBuffer ack;
    ack.WriteU8(1);
    ack.WriteU32(m_uiAccount);
    SendPacket(this, NC_USER_LOGIN_ACK, ack.Data(), ack.Size());
}

void LoginClientSession::HandleWorldStatus(const GPacket&) {
    PacketBuffer ack;
    ack.WriteU8(1);                       // world count
    ack.WriteU16(1);                      // world id
    ack.WriteString("World00");           // name
    ack.WriteU8(2);                       // status: NORMAL=2, BUSY=3, FULL=4 (EV_VERIFY)
    ack.WriteU8(0);                       // pvp flag
    SendPacket(this, NC_USER_WORLD_STATUS_ACK, ack.Data(), ack.Size());
}

void LoginClientSession::HandleWorldSelect(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    uint16 uiWorldId = 0; body.ReadU16(uiWorldId);
    if (m_eState != LS_AUTHED) {
        SendPacket(this, NC_USER_LOGINFAIL_ACK);
        return;
    }
    m_kToken.Generate(m_uiAccount, uiWorldId);
    m_eState = LS_WORLD_PICKED;
    PacketBuffer ack;
    ack.WriteU8(1);
    ack.WriteString("127.0.0.1");        // WM ip (provisional)
    ack.WriteU16(28000);                 // WM port (provisional)
    ack.WriteBytes(m_kToken.GetSecret(), 16);
    SendPacket(this, NC_USER_WORLDSELECT_ACK, ack.Data(), ack.Size());
    // The WM is told about the issued token via NC_INTER_AUTH_TOKEN_PUSH (LoginAccountDBSession).
}

} // namespace fiesta
