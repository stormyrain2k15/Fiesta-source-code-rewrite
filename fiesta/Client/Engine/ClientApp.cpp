// Client/Engine/ClientApp.cpp
#include "ClientApp.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <NiSystem.h>

namespace fiesta {

ClientApp::ClientApp()
    : m_pkApp(NULL), m_lZoneReady(0)
{
    ZeroMemory(&m_kPendingPlayer, sizeof(m_kPendingPlayer));
    InitializeCriticalSection(&m_kStateLock);
}

ClientApp::~ClientApp() {
    DeleteCriticalSection(&m_kStateLock);
}

bool ClientApp::Init(HINSTANCE hInst, const ClientConfig& rCfg) {
    m_kCfg = rCfg;

    SHINELOG_INFO("ClientApp: starting Shine client");
    SHINELOG_INFO("  Login   : %s:%u", rCfg.kLoginIp.c_str(), rCfg.uiLoginPort);
    SHINELOG_INFO("  User    : %s",    rCfg.kUser.c_str());
    SHINELOG_INFO("  CharId  : %u",    rCfg.uiCharId);
    SHINELOG_INFO("  Assets  : %s",    rCfg.kAssetRoot.c_str());

    // Build ShineApp (NiApplication subclass)
    m_pkApp = NiNew ShineApp();
    if (!m_pkApp) {
        SHINELOG_ERROR("ClientApp: NiNew ShineApp failed");
        return false;
    }
    m_pkApp->SetAssetRoot(rCfg.kAssetRoot);

    // Gamebryo init sequence
    NiInit();

    if (!m_pkApp->Create()) {
        SHINELOG_ERROR("ClientApp: ShineApp::Create (NiApplication) failed");
        return false;
    }

    // Wire login + zone callbacks via static thunks (no std::function in VS2010).
    m_kLogin.SetCredentials(rCfg.kUser, rCfg.kPass);
    m_kLogin.SetCallbacks(&ClientApp::OnLoginSuccessThunk,
                          &ClientApp::OnLoginFailThunk,
                          this);

    m_kZone.SetCallbacks(&ClientApp::OnZoneReadyThunk,
                         &ClientApp::OnZonePacketThunk,
                         &ClientApp::OnZoneFailThunk,
                         this);

    // Phase 1 bypass: connect direct to zone, skip login server.
    if (rCfg.bSkipLogin) {
        SHINELOG_INFO("ClientApp: SkipLogin=1, going direct to zone %s:%u",
                      rCfg.kZoneIP.c_str(), rCfg.uiZonePort);
        ZoneHandoff zh;
        zh.kIp         = rCfg.kZoneIP;
        zh.uiPort      = rCfg.uiZonePort;
        zh.uiAccountId = 1;             // phase-1 fixed test account
        zh.uiCharId    = rCfg.uiCharId;
        memset(zh.aToken, 0, 16);       // server should accept zero-token in dev
        if (!m_kZone.Connect(zh)) {
            SHINELOG_ERROR("ClientApp: direct-zone connect failed");
            return false;
        }
        SHINELOG_INFO("ClientApp: initialized (direct-zone mode)");
        return true;
    }

    // Connect to login server -- everything else is async from here.
    if (!m_kLogin.Net().Connect(rCfg.kLoginIp, rCfg.uiLoginPort, &m_kLogin)) {
        SHINELOG_ERROR("ClientApp: could not connect to login server %s:%u",
                       rCfg.kLoginIp.c_str(), rCfg.uiLoginPort);
        return false;
    }

    SHINELOG_INFO("ClientApp: initialized");
    return true;
}

int ClientApp::Run() {
    // NiApplication main loop — calls UpdateFrame + RenderFrame each iteration.
    // We hook PumpNetEvents inside the message loop via subclassing.
    while (m_pkApp->MainLoop()) {
        PumpNetEvents();
    }
    return 0;
}

void ClientApp::Shutdown() {
    m_kZone.Disconnect();
    m_kLogin.Net().Disconnect();

    if (m_pkApp) {
        m_pkApp->Destroy();
        NiDelete m_pkApp;
        m_pkApp = NULL;
    }
    NiShutdown();
    SHINELOG_INFO("ClientApp: shutdown complete");
}

// ── Net -> Render thread bridge ───────────────────────────────────────────────

void ClientApp::PumpNetEvents() {
    // InterlockedExchange returns the prior value -- non-zero means a zone
    // ready event was queued and we should pick up the player state.
    if (InterlockedExchange(&m_lZoneReady, 0) != 0) {
        EnterCriticalSection(&m_kStateLock);
        PlayerState player = m_kPendingPlayer;
        LeaveCriticalSection(&m_kStateLock);
        // Now safe to call into render-thread owned ShineApp
        m_pkApp->OnZoneReady(player);
    }
}

// ── Static thunks: route the C-style callbacks back to member methods ─────────

void ClientApp::OnLoginSuccessThunk(void* pkCtx, const WMHandoff& rHandoff) {
    ((ClientApp*)pkCtx)->OnLoginSuccess(rHandoff);
}
void ClientApp::OnLoginFailThunk(void* pkCtx, const char* szReason) {
    ((ClientApp*)pkCtx)->OnLoginFail(szReason);
}
void ClientApp::OnZoneReadyThunk(void* pkCtx, const PlayerState& rPlayer) {
    ((ClientApp*)pkCtx)->OnZoneReady(rPlayer);
}
void ClientApp::OnZonePacketThunk(void* pkCtx, const GPacket& rPkt) {
    ((ClientApp*)pkCtx)->OnZonePacket(rPkt);
}
void ClientApp::OnZoneFailThunk(void* pkCtx, const char* szReason) {
    ((ClientApp*)pkCtx)->OnZoneFail(szReason);
}

// ── LoginSession callbacks (network worker thread) ────────────────────────────

void ClientApp::OnLoginSuccess(const WMHandoff& rHandoff) {
    SHINELOG_INFO("ClientApp: login success, connecting to zone via WM %s:%u",
                  rHandoff.kIp.c_str(), rHandoff.uiPort);

    // WM handshake would go here in a full implementation.
    // For phase-1 we assume WM transparently routes to Zone and passes the token.
    // Connect directly to the zone address the WM provided.
    ZoneHandoff zh;
    zh.kIp          = rHandoff.kIp;
    zh.uiPort       = rHandoff.uiPort;
    zh.uiAccountId  = rHandoff.uiAccountId;
    zh.uiCharId     = m_kCfg.uiCharId;
    memcpy(zh.aToken, rHandoff.aToken, 16);

    if (!m_kZone.Connect(zh)) {
        SHINELOG_ERROR("ClientApp: zone connect failed");
    }
}

void ClientApp::OnLoginFail(const char* szReason) {
    SHINELOG_ERROR("ClientApp: login failed -- %s", szReason);
    // Signal main loop to show error screen (future UI path)
}

// ── ZoneSession callbacks (network worker thread) ─────────────────────────────

void ClientApp::OnZoneReady(const PlayerState& rPlayer) {
    // This fires on the network worker thread — don't touch NiApplication here.
    // Store and signal; PumpNetEvents() picks it up on the render thread.
    EnterCriticalSection(&m_kStateLock);
    m_kPendingPlayer = rPlayer;
    LeaveCriticalSection(&m_kStateLock);
    InterlockedExchange(&m_lZoneReady, 1);
    SHINELOG_INFO("ClientApp: zone ready signal queued");
}

void ClientApp::OnZonePacket(const GPacket& rPkt) {
    // Game packets after world entry — pass to ShineApp on render thread via queue.
    // For phase-1 ShineApp handles the small set it cares about; everything else drops.
    m_pkApp->OnGamePacket(rPkt);
}

void ClientApp::OnZoneFail(const char* szReason) {
    SHINELOG_ERROR("ClientApp: zone error -- %s", szReason);
}

} // namespace fiesta
