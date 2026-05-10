// Client/Engine/ShineApp.cpp
#include "ShineApp.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

NiImplementRTTI(fiesta::ShineApp, NiApplication);

namespace fiesta {

ShineApp::ShineApp()
    : NiApplication("Shine", DEFAULT_WIDTH, DEFAULT_HEIGHT, true),
      m_bWorldReady(false), m_fAccumDt(0.0f), m_dwLastTick(0)
{
    ZeroMemory(&m_kPlayer, sizeof(m_kPlayer));
}

ShineApp::~ShineApp() {}

// ── NiApplication overrides ───────────────────────────────────────────────────

bool ShineApp::Initialize() {
    // Gamebryo renderer is already up at this point (NiApplication::Initialize
    // calls CreateRenderer() internally before calling us). Scene graph init.
    if (!NiApplication::Initialize()) return false;

    m_dwLastTick = GetTickCount();

    // Build scene root
    if (!m_kScene.Init(m_pkRenderer)) {
        SHINELOG_ERROR("ShineApp: ShineScene::Init failed");
        return false;
    }

    // Set up camera
    if (!m_kCamera.Init(m_pkRenderer)) {
        SHINELOG_ERROR("ShineApp: ShineCamera::Init failed");
        return false;
    }
    m_pkCamera = m_kCamera.GetNiCamera();

    // HUD overlay
    m_kHUD.Init(m_pkRenderer);

    SHINELOG_INFO("ShineApp: initialized (%ux%u)", m_uiWidth, m_uiHeight);
    return true;
}

bool ShineApp::UpdateFrame() {
    if (!NiApplication::UpdateFrame()) return false;

    float fDt = GetDeltaTime();

    // Camera input
    m_kCamera.Tick(fDt);

    // Scene animation update
    if (m_bWorldReady) {
        m_kScene.Tick(fDt);
        m_kHUD.Update(m_kPlayer);
    }

    // Advance scene graph time
    NiTimeController::UpdateTime();

    return true;
}

bool ShineApp::RenderFrame() {
    if (!m_pkRenderer) return false;

    m_pkRenderer->BeginFrame();
    m_pkRenderer->ClearBuffer(NULL, NiRenderer::CLEAR_ALL);

    if (m_bWorldReady && m_kScene.GetRoot()) {
        // Cull scene from camera's perspective
        NiCullScene(m_pkCamera, m_kScene.GetRoot(), m_kVisibleSet);
        // Draw visible geometry
        m_pkRenderer->RenderScene(m_kScene.GetRoot(), m_pkCamera);
    }

    // HUD draws on top of 3D
    m_kHUD.Render(m_pkRenderer);

    m_pkRenderer->DisplayFrame();
    m_pkRenderer->EndFrame();

    return true;
}

void ShineApp::Terminate() {
    m_kHUD.Shutdown();
    m_kScene.Shutdown();
    m_kCamera.Shutdown();
    NiApplication::Terminate();
    SHINELOG_INFO("ShineApp: terminated");
}

// ── Game callbacks ────────────────────────────────────────────────────────────

void ShineApp::OnZoneReady(const PlayerState& rPlayer) {
    m_kPlayer     = rPlayer;
    m_bWorldReady = true;
    m_kHUD.SetVisible(true);

    // Load the starting map assets (.sga files)
    LoadStartMap();

    SHINELOG_INFO("ShineApp: world ready lv=%u class=%u map=%u",
                  rPlayer.uiLevel, rPlayer.uiClass, rPlayer.uiMapId);
}

void ShineApp::OnGamePacket(const GPacket& rPkt) {
    uint16 nc = (uint16)rPkt.GetOpcode();
    // Route incoming game packets to the correct subsystem
    switch (nc) {
    case NC_CHAR_STATUS_CHANGE_CMD: {
        // Server is updating our stats (hp/sp changed etc)
        PacketBuffer body = rPkt.Body();
        // Update local player state and push to HUD
        // PROVISIONAL -- full field layout from capture
        int32 iHP = 0, iSP = 0;
        body.ReadI32(iHP); body.ReadI32(iSP);
        m_kPlayer.iHP = iHP;
        m_kPlayer.iSP = iSP;
        break;
    }
    case NC_MAP_WORLDTICK_CMD:
        // Handled in ZoneSession, shouldn't reach here
        break;
    default:
        break;
    }
}

// ── Private ───────────────────────────────────────────────────────────────────

float ShineApp::GetDeltaTime() {
    DWORD dwNow = GetTickCount();
    float fDt   = (float)(dwNow - m_dwLastTick) / 1000.0f;
    m_dwLastTick = dwNow;
    if (fDt > 0.1f) fDt = 0.1f; // clamp runaway dt
    return fDt;
}

bool ShineApp::LoadStartMap() {
    if (m_kAssetRoot.empty()) return false;
    // Build path: Assets/Graphics/Map/<mapId>/terrain.sga
    char szPath[512];
    _snprintf_s(szPath, sizeof(szPath), "%s\\Graphics\\Map\\%u\\terrain.sga",
                m_kAssetRoot.c_str(), m_kPlayer.uiMapId);

    bool bOk = m_kScene.LoadSGA(szPath);
    if (bOk) {
        m_kCamera.SetTarget(m_kPlayer.fPosX, m_kPlayer.fPosY, m_kPlayer.fPosZ);
        SHINELOG_INFO("ShineApp: map %u loaded", m_kPlayer.uiMapId);
    } else {
        SHINELOG_WARN("ShineApp: map %u asset not found at '%s' -- world renders empty",
                      m_kPlayer.uiMapId, szPath);
    }
    return bOk;
}

} // namespace fiesta
