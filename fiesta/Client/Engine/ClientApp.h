// Client/Engine/ClientApp.h
// ClientApp: top-level client coordinator.
// Owns LoginSession and ZoneSession, drives ShineApp render loop,
// bridges network events onto the render thread safely.
#ifndef SHINE_CLIENT_ENGINE_CLIENTAPP_H
#define SHINE_CLIENT_ENGINE_CLIENTAPP_H

#include "ShineApp.h"
#include "../Network/LoginSession.h"
#include "../Network/ZoneSession.h"
#include "../../Server/Shared/ShineTypes.h"
#include <string>

namespace fiesta {

struct ClientConfig {
    std::string kLoginIp;
    uint16      uiLoginPort;
    std::string kUser;
    std::string kPass;
    uint32      uiCharId;       // character slot to log in with
    std::string kAssetRoot;     // path to Assets/ directory
    uint32      uiWidth;
    uint32      uiHeight;
    // Phase 1 direct-to-zone bypass (skips login when bSkipLogin=true)
    bool        bSkipLogin;
    std::string kZoneIP;
    uint16      uiZonePort;
};

class ClientApp {
public:
    ClientApp();
    ~ClientApp();

    bool Init(HINSTANCE hInst, const ClientConfig& rCfg);
    int  Run();
    void Shutdown();

private:
    ClientConfig    m_kCfg;
    ShineApp*       m_pkApp;
    LoginSession    m_kLogin;
    ZoneSession     m_kZone;

    // Thread-safe handoff from network worker -> render thread.
    // VS2010 has no <atomic>; use a volatile LONG + InterlockedExchange.
    volatile LONG       m_lZoneReady;
    PlayerState         m_kPendingPlayer;
    CRITICAL_SECTION    m_kStateLock;

    // Static thunks (function-pointer entry into member methods).
    static void OnLoginSuccessThunk(void* pkCtx, const WMHandoff& rHandoff);
    static void OnLoginFailThunk   (void* pkCtx, const char* szReason);
    static void OnZoneReadyThunk   (void* pkCtx, const PlayerState& rPlayer);
    static void OnZonePacketThunk  (void* pkCtx, const GPacket& rPkt);
    static void OnZoneFailThunk    (void* pkCtx, const char* szReason);

    // Real handlers (fire on network worker thread).
    void OnLoginSuccess(const WMHandoff& rHandoff);
    void OnLoginFail(const char* szReason);
    void OnZoneReady(const PlayerState& rPlayer);
    void OnZonePacket(const GPacket& rPkt);
    void OnZoneFail(const char* szReason);

    // Per-frame pump -- checks for pending handoffs from net thread.
    void PumpNetEvents();
};

} // namespace fiesta
#endif // SHINE_CLIENT_ENGINE_CLIENTAPP_H
