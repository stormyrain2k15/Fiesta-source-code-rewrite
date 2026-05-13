// Client/Engine/ShineApp.cpp
#include "ShineApp.h"
#include "MachineOpt.h"
#include "ShineConfig.h"
#include "ShineRenderer.h"
#include "../Sound/ShineSoundMgr.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

NiImplementRTTI(shine::ShineApp, NiApplication);

shine::ShineApp* shine::ShineApp::ms_pkApp = NULL;

NiApplication* NiApplication::Create() {
    shine::ShineApp::ms_pkApp = NiNew shine::ShineApp;
    return shine::ShineApp::ms_pkApp;
}

namespace shine {

ShineApp::ShineApp()
    : NiApplication("Shine",
                     MachineOpt::Get().GetWidth(),
                     MachineOpt::Get().GetHeight(),
                     MachineOpt::Get().IsFullscreen() ? 1 : 0),
      m_pkZone(NULL),
      m_lZoneReady(0), m_lCharBase(0), m_lCharShape(0),
      m_dwLastTick(0)
{
    ZeroMemory(&m_kPendingPlayer, sizeof(m_kPendingPlayer));
    InitializeCriticalSection(&m_kStateLock);
    SHINELOG_INFO("ShineApp: created");
}

ShineApp::~ShineApp() {
    DeleteCriticalSection(&m_kStateLock);
}

bool ShineApp::Initialize() {
    if (!NiApplication::Initialize()) return false;
    m_dwLastTick = GetTickCount();

    // Init sound (non-fatal if Miles missing)
    g_kSoundMgr.Init(ShineConfig::Get().kMilesDir);

    // Register all frameworks
    Pgg_kFrameMgr.Register(AccountFrameWorkID,    &m_kAccountFW);
    Pgg_kFrameMgr.Register(CharSelectFrameWorkID, &m_kCharSelectFW);
    Pgg_kFrameMgr.Register(GameFrameWorkID,        &m_kGameFW);

    // Boot at account/login screen
    Pgg_kFrameMgr.Start(AccountFrameWorkID);

    SHINELOG_INFO("ShineApp::Initialize complete");
    return true;
}

bool ShineApp::CreateRenderer() {
    HWND hWnd = m_pkAppWindow ? m_pkAppWindow->GetWindowReference() : NULL;
    if (!hWnd) return false;
    m_pkRenderer = ShineRenderer::Create(hWnd);
    return m_pkRenderer != NULL;
}

bool ShineApp::UpdateFrame() {
    if (!NiApplication::UpdateFrame()) return false;
    float fDt = GetDeltaTime();
    PumpPendingEvents();
    if (!Pgg_kFrameMgr.Update(fDt)) return false;
    NiTimeController::UpdateTime();
    return true;
}

bool ShineApp::RenderFrame() {
    if (!m_pkRenderer) return false;
    Pgg_kFrameMgr.Render();
    return true;
}

void ShineApp::Terminate() {
    g_kSoundMgr.Shutdown();
    Pgg_kFrameMgr.Stop();
    MachineOpt::Get().Save();
    NiApplication::Terminate();
    SHINELOG_INFO("ShineApp::Terminate");
}

bool ShineApp::OnDefault(NiEventRef pEventRecord) {
    switch (pEventRecord->uiMsg) {
    case WM_SETCURSOR:
        ::SetCursor(NULL);
        return true;
    case WM_SYSKEYDOWN:
        if (pEventRecord->wParam == VK_F10 || pEventRecord->wParam == VK_MENU)
            return true;
        break;
    case WM_KEYUP:
        if (pEventRecord->wParam == VK_SNAPSHOT) {
            SHINELOG_INFO("ShineApp: screenshot");
            return true;
        }
        break;
    case WM_LBUTTONDOWN:
        if (Pgg_kFrameMgr.IsInRun(CharSelectFrameWorkID))
            m_kCharSelectFW.GetUI().OnLButtonDown(
                LOWORD(pEventRecord->lParam), HIWORD(pEventRecord->lParam));
        break;
    case WM_MOUSEMOVE:
        if (Pgg_kFrameMgr.IsInRun(CharSelectFrameWorkID))
            m_kCharSelectFW.GetUI().OnMouseMove(
                LOWORD(pEventRecord->lParam), HIWORD(pEventRecord->lParam));
        break;
    case WM_KEYDOWN:
        if (Pgg_kFrameMgr.IsInRun(CharSelectFrameWorkID))
            m_kCharSelectFW.GetUI().OnKeyDown((int)pEventRecord->wParam);
        break;
    case WM_CLOSE:
        if (Pgg_kFrameMgr.IsInRun(GameFrameWorkID)) {
            Pgg_kFrameMgr.AddMsg(PGFM_EXIT);
            return true;
        }
        break;
    case WM_ACTIVATE:
        switch (LOWORD(pEventRecord->wParam)) {
        case WA_ACTIVE: case WA_CLICKACTIVE:
            OnWindowActivate(NULL, true);
            g_kSoundMgr.HoldSound(false);
            break;
        case WA_INACTIVE:
            OnWindowActivate(NULL, false);
            g_kSoundMgr.HoldSound(true);
            break;
        }
        break;
    }
    return NiApplication::OnDefault(pEventRecord);
}

// ── Network event bridge ──────────────────────────────────────────────────────

void ShineApp::SetZoneSession(ZoneSession* pkZone) {
    m_pkZone = pkZone;
    m_kCharSelectFW.SetZoneSession(pkZone);
    // GameFrameWork's SetupWorld is deferred to PumpPendingEvents() when
    // OnZoneReady fires and we have a PlayerState.
}

void ShineApp::OnZoneReady(const PlayerState& rPlayer) {
    EnterCriticalSection(&m_kStateLock);
    m_kPendingPlayer = rPlayer;
    LeaveCriticalSection(&m_kStateLock);
    InterlockedExchange(&m_lZoneReady, 1);
}

void ShineApp::OnCharBaseCmd(const GPacket& rPkt) {
    EnterCriticalSection(&m_kStateLock);
    m_kPendingCharBase = rPkt;
    LeaveCriticalSection(&m_kStateLock);
    InterlockedExchange(&m_lCharBase, 1);
}

void ShineApp::OnCharShapeCmd(const GPacket& rPkt) {
    EnterCriticalSection(&m_kStateLock);
    m_kPendingCharShape = rPkt;
    LeaveCriticalSection(&m_kStateLock);
    InterlockedExchange(&m_lCharShape, 1);
}

void ShineApp::OnGamePacket(const GPacket& rPkt) {
    if (Pgg_kFrameMgr.IsInRun(GameFrameWorkID))
        m_kGameFW.OnGamePacket(rPkt);
}

void ShineApp::PumpPendingEvents() {
    // Char base/shape → CharSelectFrameWork
    if (InterlockedExchange(&m_lCharBase, 0) != 0) {
        EnterCriticalSection(&m_kStateLock);
        GPacket pkt = m_kPendingCharBase;
        LeaveCriticalSection(&m_kStateLock);

        // Transition to char select if not already there
        if (!Pgg_kFrameMgr.IsInRun(CharSelectFrameWorkID)) {
            m_kCharSelectFW.SetZoneSession(m_pkZone);
            // Wire UI callbacks via static thunks (VS2010: no lambdas).
            m_kCharSelectFW.GetUI().SetCallbacks(
                &ShineApp::CharSelectThunk,
                &ShineApp::CharCreateThunk,
                &ShineApp::CharDeleteThunk,
                this);
            m_kCharSelectFW.SetRenderer(m_pkRenderer);
            Pgg_kFrameMgr.Start(CharSelectFrameWorkID);
        }
        m_kCharSelectFW.OnCharBaseCmd(pkt);
    }

    if (InterlockedExchange(&m_lCharShape, 0) != 0) {
        EnterCriticalSection(&m_kStateLock);
        GPacket pkt = m_kPendingCharShape;
        LeaveCriticalSection(&m_kStateLock);
        if (Pgg_kFrameMgr.IsInRun(CharSelectFrameWorkID))
            m_kCharSelectFW.OnCharShapeCmd(pkt);
    }

    // Zone ready → GameFrameWork
    if (InterlockedExchange(&m_lZoneReady, 0) != 0) {
        EnterCriticalSection(&m_kStateLock);
        PlayerState player = m_kPendingPlayer;
        LeaveCriticalSection(&m_kStateLock);
        m_kGameFW.SetupWorld(player, m_pkZone, m_pkRenderer);
    }
}

// Member dispatchers invoked by the static thunks below.
void ShineApp::OnCharSelected(uint32 uiCharId) {
    m_kCharSelectFW.SelectChar(uiCharId);
}
void ShineApp::OnCharCreated(const std::string& rName, uint16 uiClass,
                              uint8 uiHair, uint8 uiHairColor, uint8 uiFace) {
    m_kCharSelectFW.CreateChar(rName, uiClass, uiHair, uiHairColor, uiFace);
}
void ShineApp::OnCharDeleted(uint32 uiCharId) {
    m_kCharSelectFW.DeleteChar(uiCharId);
}

// Static thunks: cast ctx to ShineApp*, dispatch.
void ShineApp::CharSelectThunk(void* pkCtx, uint32 uiCharId) {
    ((ShineApp*)pkCtx)->OnCharSelected(uiCharId);
}
void ShineApp::CharCreateThunk(void* pkCtx, const std::string& rName,
                                uint16 uiClass, uint8 uiHair,
                                uint8 uiHairColor, uint8 uiFace) {
    ((ShineApp*)pkCtx)->OnCharCreated(rName, uiClass, uiHair, uiHairColor, uiFace);
}
void ShineApp::CharDeleteThunk(void* pkCtx, uint32 uiCharId) {
    ((ShineApp*)pkCtx)->OnCharDeleted(uiCharId);
}

float ShineApp::GetDeltaTime() {
    DWORD dwNow  = GetTickCount();
    float fDt    = (float)(dwNow - m_dwLastTick) / 1000.0f;
    m_dwLastTick = dwNow;
    if (fDt > 0.1f) fDt = 0.1f;
    return fDt;
}

} // namespace shine
