// Client/Framework/AccountFrameWork.h
// Account framework -- handles login, account creation, character select.
// Active from app launch until the player enters the world.
// Transitions to GameFrameWorkID on successful zone entry.
#ifndef SHINE_CLIENT_FRAMEWORK_ACCOUNTFRAMEWORK_H
#define SHINE_CLIENT_FRAMEWORK_ACCOUNTFRAMEWORK_H

#include "ShineFrameWork.h"
#include "../Network/LoginSession.h"
#include "../Network/WMSession.h"
#include "../Network/ZoneSession.h"

namespace shine {

class AccountFrameWork : public ShineFrameWork {
public:
    AccountFrameWork();

    virtual void OnStart()                          override;
    virtual void OnStop()                           override;
    virtual bool Update(float fDt)                  override;
    virtual void Render()                           override;
    virtual void OnMsg(const FrameWorkMsgData& rMsg) override;

    // Called by ShineApp after network events are pumped
    void OnLoginSuccess (const WMHandoff& rHandoff);
    void OnLoginFail    (const char* szReason);
    void OnWMSuccess    (const ZoneHandoff& rHandoff);
    void OnWMFail       (const char* szReason);
    void RouteCharBaseToApp (const GPacket& rPkt);
    void RouteCharShapeToApp(const GPacket& rPkt);

    ZoneSession& GetZoneSession() { return m_kZone; }

private:
    enum State {
        AS_IDLE,
        AS_CONNECTING_LOGIN,
        AS_CONNECTING_WM,
        AS_CONNECTING_ZONE,
        AS_ZONE_READY,
    };

    State           m_eState;
    LoginSession    m_kLogin;
    WMSession       m_kWM;
    ZoneSession     m_kZone;

    void StartLogin();
    void StartDirectZone();

    // VS2010-compatible static thunks (no lambdas). Each network session
    // takes a C-style function pointer + void* ctx; the thunks cast the
    // ctx back to AccountFrameWork* and dispatch to the member methods.
    static void LoginSuccessThunk (void* pkCtx, const WMHandoff& rHandoff);
    static void LoginFailThunk    (void* pkCtx, const char* szReason);
    static void WMSuccessThunk    (void* pkCtx, const ZoneHandoff& rHandoff);
    static void WMFailThunk       (void* pkCtx, const char* szReason);
    static void ZoneReadyThunk    (void* pkCtx, const PlayerState& rPlayer);
    static void ZonePacketThunk   (void* pkCtx, const GPacket& rPkt);
    static void ZoneFailThunk     (void* pkCtx, const char* szReason);

    void HandleZonePacket(const GPacket& rPkt); // body of former ZoneSession packet lambda
};

} // namespace shine
#endif // SHINE_CLIENT_FRAMEWORK_ACCOUNTFRAMEWORK_H
