// Client/Network/LoginSession.h
// Login server state machine.
// Drives the full NC_USER_* handshake sequence and hands off to WM on success.
//
// Flow:
//   Connect → OnConnected()
//     → NC_USER_VERSION_REQ (send build token)
//   ← NC_USER_SEED_ACK        (init cipher)
//   ← NC_USER_CLIENT_VERSION_CHECK_REQ+1 (version ok)
//     → NC_USER_WORLD_STATUS_REQ
//   ← NC_USER_WORLD_STATUS_ACK (world list)
//     → NC_USER_LOGIN_REQ (credentials)
//   ← NC_USER_LOGIN_ACK (account id)
//     → NC_USER_WORLDSELECT_REQ
//   ← NC_USER_WORLDSELECT_ACK (WM ip + port + 16-byte token)
//     → [disconnect login, connect WM]
#ifndef SHINE_CLIENT_NETWORK_LOGINSESSION_H
#define SHINE_CLIENT_NETWORK_LOGINSESSION_H

#include "ShineNetClient.h"
#include <string>

namespace fiesta {

struct WorldInfo {
    uint16      uiId;
    std::string kName;
    uint8       uiStatus;   // 2=normal 3=busy 4=full
    uint8       uiPvP;
};

struct WMHandoff {
    std::string kIp;
    uint16      uiPort;
    uint8       aToken[16];
    uint32      uiAccountId;
};

// VS2010 has no std::function; use C-style function pointers + a void* ctx
// so a single LoginSession can call back into arbitrary owner objects.
typedef void (*LoginSuccessCallback)(void* pkCtx, const WMHandoff& rHandoff);
typedef void (*LoginFailCallback)   (void* pkCtx, const char* szReason);

class LoginSession : public INetPacketSink {
public:
    LoginSession();

    void SetCredentials(const std::string& rUser, const std::string& rPass);
    void SetCallbacks(LoginSuccessCallback onSuccess,
                      LoginFailCallback onFail,
                      void* pkCtx);

    // INetPacketSink
    virtual void OnConnected()                  override;
    virtual void OnDisconnected()               override;
    virtual void OnPacket(const GPacket& rPkt)  override;

    ShineNetClient& Net() { return m_kNet; }

private:
    enum State {
        LS_IDLE = 0,
        LS_WAIT_SEED,
        LS_WAIT_VERSION_ACK,
        LS_WAIT_WORLD_STATUS,
        LS_WAIT_LOGIN_ACK,
        LS_WAIT_WORLDSELECT_ACK,
        LS_DONE
    };

    ShineNetClient       m_kNet;
    State                m_eState;
    std::string          m_kUser;
    std::string          m_kPass;
    uint32               m_uiAccountId;
    std::vector<WorldInfo> m_kWorlds;

    LoginSuccessCallback m_cbSuccess;
    LoginFailCallback    m_cbFail;
    void*                m_pkCbCtx;

    void SendVersionReq();
    void SendWorldStatusReq();
    void SendLoginReq();
    void SendWorldSelectReq(uint16 uiWorldId);

    void HandleSeedAck          (const GPacket& rPkt);
    void HandleVersionAck       (const GPacket& rPkt);
    void HandleWorldStatusAck   (const GPacket& rPkt);
    void HandleLoginAck         (const GPacket& rPkt);
    void HandleLoginFail        (const GPacket& rPkt);
    void HandleWorldSelectAck   (const GPacket& rPkt);
};

} // namespace fiesta
#endif // SHINE_CLIENT_NETWORK_LOGINSESSION_H
