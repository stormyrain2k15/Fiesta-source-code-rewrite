// Client/Framework/AccountFrameWork.cpp
#include "AccountFrameWork.h"
#include "../Engine/ShineApp.h"
#include "../Engine/ShineConfig.h"
#include "../Input/KeyMap.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

namespace shine {

AccountFrameWork::AccountFrameWork() : m_eState(AS_IDLE) {}

void AccountFrameWork::OnStart() {
    SHINELOG_INFO("AccountFrameWork: started");
    m_eState = AS_IDLE;

    const ShineConfig& cfg = ShineConfig::Get();

    // Wire login callbacks via static thunks (VS2010: no lambdas, no
    // std::function). The session stores a C function pointer + void*
    // ctx; the thunk casts ctx back to AccountFrameWork* and dispatches.
    m_kLogin.SetCredentials(cfg.kUser, cfg.kPass);
    m_kLogin.SetCallbacks(&AccountFrameWork::LoginSuccessThunk,
                           &AccountFrameWork::LoginFailThunk,
                           this);

    // Wire WM callbacks.
    m_kWM.SetCallbacks(&AccountFrameWork::WMSuccessThunk,
                        &AccountFrameWork::WMFailThunk,
                        this);

    // Wire zone callbacks -- on ready, transition to GameFrameWork.
    m_kZone.SetCallbacks(&AccountFrameWork::ZoneReadyThunk,
                          &AccountFrameWork::ZonePacketThunk,
                          &AccountFrameWork::ZoneFailThunk,
                          this);

    if (cfg.bSkipLogin) {
        StartDirectZone();
    } else {
        StartLogin();
    }
}

void AccountFrameWork::OnStop() {
    SHINELOG_INFO("AccountFrameWork: stopped");
}

bool AccountFrameWork::Update(float /*fDt*/) {
    // Nothing to update in account framework yet (future: login screen animation)
    return true;
}

void AccountFrameWork::Render() {
    // Future: render login screen, character select UI
}

void AccountFrameWork::OnMsg(const FrameWorkMsgData& rMsg) {
    switch (rMsg.eMsg) {
    case PGFM_ZONE_READY:
        m_eState = AS_ZONE_READY;
        SHINELOG_INFO("AccountFrameWork: zone ready -- transitioning to GameFrameWork");
        Pgg_kFrameMgr.Start(GameFrameWorkID);
        break;
    case PGFM_EXIT:
    case PGFM_DISCONNECT:
        m_kZone.Disconnect();
        m_kWM.Disconnect();
        m_kLogin.Net().Disconnect();
        m_eState = AS_IDLE;
        break;
    default: break;
    }
}

void AccountFrameWork::StartLogin() {
    const ShineConfig& cfg = ShineConfig::Get();
    m_eState = AS_CONNECTING_LOGIN;
    if (!m_kLogin.Net().Connect(cfg.kLoginIp, cfg.uiLoginPort, &m_kLogin)) {
        SHINELOG_ERROR("AccountFrameWork: login connect failed");
        m_eState = AS_IDLE;
    }
}

void AccountFrameWork::StartDirectZone() {
    const ShineConfig& cfg = ShineConfig::Get();
    ZoneHandoff zh;
    zh.kIp         = cfg.kZoneIP;
    zh.uiPort      = cfg.uiZonePort;
    zh.uiAccountId = 1;
    zh.uiCharId    = cfg.uiCharId;
    ZeroMemory(zh.aToken, 16);
    m_eState = AS_CONNECTING_ZONE;
    if (!m_kZone.Connect(zh)) {
        SHINELOG_ERROR("AccountFrameWork: direct zone connect failed");
        m_eState = AS_IDLE;
    }
}

void AccountFrameWork::OnLoginSuccess(const WMHandoff& rHandoff) {
    SHINELOG_INFO("AccountFrameWork: login ok, connecting WM");
    WMSession::WMHandoff wh;
    wh.kIp         = rHandoff.kIp;
    wh.uiPort      = rHandoff.uiPort;
    wh.uiAccountId = rHandoff.uiAccountId;
    wh.uiCharId    = ShineConfig::Get().uiCharId;
    memcpy(wh.aToken, rHandoff.aToken, 16);
    m_eState = AS_CONNECTING_WM;
    if (!m_kWM.Connect(wh))
        SHINELOG_ERROR("AccountFrameWork: WM connect failed");
}

void AccountFrameWork::OnLoginFail(const char* szReason) {
    SHINELOG_ERROR("AccountFrameWork: login failed -- %s", szReason);
    m_eState = AS_IDLE;
}

void AccountFrameWork::OnWMSuccess(const ZoneHandoff& rHandoff) {
    SHINELOG_INFO("AccountFrameWork: WM ok, connecting zone");
    m_kWM.Disconnect();
    m_eState = AS_CONNECTING_ZONE;
    if (!m_kZone.Connect(rHandoff))
        SHINELOG_ERROR("AccountFrameWork: zone connect failed");
}

void AccountFrameWork::OnWMFail(const char* szReason) {
    SHINELOG_ERROR("AccountFrameWork: WM failed -- %s", szReason);
    m_eState = AS_IDLE;
}

void AccountFrameWork::HandleZonePacket(const GPacket& rPkt) {
    uint16 nc = (uint16)rPkt.GetOpcode();
    switch (nc) {
    case NC_CHAR_OPTION_KEYMAP_CMD: {
        PacketBuffer body = rPkt.Body();
        KeyMap::Get().OnServerKeyMap(body.Data(), (uint32)body.Size());
        break; }
    case NC_CHAR_CLIENT_BASE_CMD:
        RouteCharBaseToApp(rPkt);
        break;
    case NC_CHAR_CLIENT_SHAPE_CMD:
        RouteCharShapeToApp(rPkt);
        break;
    case NC_CHAR_NEW_ACK:
    case NC_CHAR_DEL_ACK: {
        ShineApp* pk = ShineApp::GetInstance();
        if (pk) pk->OnGamePacket(rPkt); // routed to CharSelectFW
        break; }
    default: break;
    }
}

void AccountFrameWork::RouteCharBaseToApp(const GPacket& rPkt) {
    ShineApp* pkApp = ShineApp::GetInstance();
    if (pkApp) pkApp->OnCharBaseCmd(rPkt);
}

void AccountFrameWork::RouteCharShapeToApp(const GPacket& rPkt) {
    ShineApp* pkApp = ShineApp::GetInstance();
    if (pkApp) pkApp->OnCharShapeCmd(rPkt);
}

// ── Static thunks ─────────────────────────────────────────────────────────────

void AccountFrameWork::LoginSuccessThunk(void* pkCtx, const WMHandoff& rHandoff) {
    ((AccountFrameWork*)pkCtx)->OnLoginSuccess(rHandoff);
}
void AccountFrameWork::LoginFailThunk(void* pkCtx, const char* szReason) {
    ((AccountFrameWork*)pkCtx)->OnLoginFail(szReason);
}
void AccountFrameWork::WMSuccessThunk(void* pkCtx, const ZoneHandoff& rHandoff) {
    ((AccountFrameWork*)pkCtx)->OnWMSuccess(rHandoff);
}
void AccountFrameWork::WMFailThunk(void* pkCtx, const char* szReason) {
    ((AccountFrameWork*)pkCtx)->OnWMFail(szReason);
}
void AccountFrameWork::ZoneReadyThunk(void* /*pkCtx*/, const PlayerState& /*rPlayer*/) {
    // Net thread; signal GameFrameWork (the actual player state is consumed
    // on the render side via ShineApp::PumpPendingEvents -> SetupWorld).
    Pgg_kFrameMgr.AddMsg(PGFM_ZONE_READY, 0, 0);
}
void AccountFrameWork::ZonePacketThunk(void* pkCtx, const GPacket& rPkt) {
    ((AccountFrameWork*)pkCtx)->HandleZonePacket(rPkt);
}
void AccountFrameWork::ZoneFailThunk(void* pkCtx, const char* szReason) {
    AccountFrameWork* p = (AccountFrameWork*)pkCtx;
    SHINELOG_ERROR("AccountFrameWork: zone error -- %s", szReason);
    p->m_eState = AS_IDLE;
}

} // namespace shine
