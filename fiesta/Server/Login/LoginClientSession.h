// Server/Login/LoginClientSession.h
// 04 -- one connected login client.
// EVIDENCE: PDB_CONFIRMED  symbol: LoginClientSession, ClientVersionKeyInfo, OTPLogin, NC_USER_*
#ifndef FIESTA_LOGIN_LOGINCLIENTSESSION_H
#define FIESTA_LOGIN_LOGINCLIENTSESSION_H
#include "../Shared/Socket_Acceptor.h"
#include "../Shared/CToken.h"

namespace fiesta {

enum LoginState { LS_NEW = 0, LS_VERSION_OK, LS_AUTHED, LS_WORLD_PICKED };

class LoginClientSession : public IOCPSession {
public:
    LoginClientSession();
    virtual void OnConnect();
    virtual void OnDisconnect();
    virtual void OnPacket(const GPacket& rPkt);

    AccountID  GetAccount() const { return m_uiAccount; }
    LoginState GetState()   const { return m_eState; }
    CToken&    GetToken()         { return m_kToken; }
private:
    void HandleVersionCheck (const GPacket& rPkt);
    void HandleXTrapLegacy  (const GPacket& rPkt);   // legacy no-op (modern client doesn't send)
    void HandleLogin        (const GPacket& rPkt);
    void HandleWorldStatus  (const GPacket& rPkt);
    void HandleWorldSelect  (const GPacket& rPkt);

    LoginState m_eState;
    AccountID  m_uiAccount;
    CToken     m_kToken;
    std::string m_kAccountName;
};

} // namespace fiesta
#endif
