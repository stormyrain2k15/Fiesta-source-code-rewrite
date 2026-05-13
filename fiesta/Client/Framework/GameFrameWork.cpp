// Client/Framework/GameFrameWork.cpp
#include "GameFrameWork.h"
#include "../Engine/ShineConfig.h"
#include "../Engine/MachineOpt.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include "../../Server/Common/NETCOMMAND.h"

namespace shine {

GameFrameWork::GameFrameWork()
    : m_pkZone(NULL), m_pkRenderer(NULL), m_bWorldReady(false)
{
    ZeroMemory(&m_kPlayer, sizeof(m_kPlayer));
}

void GameFrameWork::OnStart() {
    SHINELOG_INFO("GameFrameWork: started");
}

void GameFrameWork::OnStop() {
    UILayout::Get().Save();
    m_kUI.Shutdown();
    m_kHotbar.Shutdown();
    m_kWorldObjs.Shutdown();
    m_kScene.Shutdown();
    m_bWorldReady = false;
    SHINELOG_INFO("GameFrameWork: stopped");
}

void GameFrameWork::SetupWorld(const PlayerState& rPlayer,
                                ZoneSession* pkZone,
                                NiRenderer* pkRenderer) {
    m_kPlayer    = rPlayer;
    m_pkZone     = pkZone;
    m_pkRenderer = pkRenderer;

    char szExeDir[MAX_PATH];
    GetModuleFileNameA(NULL, szExeDir, MAX_PATH);
    char* pSlash = strrchr(szExeDir, '\\');
    if (pSlash) *(pSlash+1) = '\0';
    std::string kExeDir(szExeDir);

    m_kScene.Init(pkRenderer);
    m_kCamera.Init(pkRenderer);
    m_kWorldObjs.Init(&m_kScene);

    m_kUI.Init(pkRenderer, kExeDir + "ShineUI.cfg");
    m_kHotbar.Init(pkRenderer, kExeDir + "ShineHotbar.cfg");

    ShineInput::Get().SetPosition(rPlayer.fPosX, rPlayer.fPosY, rPlayer.fPosZ);

    LoadMap();
    LoadPlayerCharacter();

    // Start map BGM
    const ShineConfig& cfg = ShineConfig::Get();
    char acBGM[64];
    sprintf_s(acBGM, sizeof(acBGM), "Map_%u.mp3", rPlayer.uiMapId);
    std::string kBGM = cfg.SoundPath("BGM", acBGM);
    g_kSoundMgr.PlayBGM(kBGM);

    m_kUI.SetVisible(true);
    m_kHotbar.SetVisible(true);
    m_bWorldReady = true;

    SHINELOG_INFO("GameFrameWork: world ready lv=%u class=%u map=%u",
                  rPlayer.uiLevel, rPlayer.uiClass, rPlayer.uiMapId);
}

bool GameFrameWork::Update(float fDt) {
    if (!m_bWorldReady) return true;

    m_kCamera.Tick(fDt);
    ShineInput::Get().SetCameraYaw(m_kCamera.GetYaw());
    ShineInput::Get().Tick(fDt, m_pkZone);

    const Vec3f& pos = ShineInput::Get().GetPosition();
    m_kCamera.SetTarget(pos.x, pos.y, pos.z);

    m_kScene.Tick(fDt);
    m_kWorldObjs.Tick(fDt);
    m_kHotbar.Update(fDt);

    POINT kMouse; GetCursorPos(&kMouse);
    bool bAlt  = (GetAsyncKeyState(VK_MENU)   & 0x8000) != 0;
    bool bLBtn = (GetAsyncKeyState(VK_LBUTTON) & 0x8000) != 0;
    m_kUI.Update(m_kPlayer, kMouse.x, kMouse.y, bAlt, bLBtn);

    return true;
}

void GameFrameWork::Render() {
    if (!m_pkRenderer || !m_bWorldReady) return;

    m_pkRenderer->BeginFrame();
    m_pkRenderer->ClearBuffer(NULL, NiRenderer::CLEAR_ALL);

    if (m_kScene.GetRoot()) {
        NiCullScene(m_kCamera.GetNiCamera(), m_kScene.GetRoot(), m_kVisibleSet);
        m_pkRenderer->RenderScene(m_kScene.GetRoot(), m_kCamera.GetNiCamera());
    }

    m_kHotbar.Render(m_pkRenderer);
    m_kUI.Render(m_pkRenderer);

    m_pkRenderer->DisplayFrame();
    m_pkRenderer->EndFrame();
}

void GameFrameWork::OnMsg(const FrameWorkMsgData& rMsg) {
    switch (rMsg.eMsg) {
    case PGFM_EXIT:
    case PGFM_DISCONNECT:
        g_kSoundMgr.StopBGM();
        if (m_pkZone) m_pkZone->Disconnect();
        Pgg_kFrameMgr.Start(AccountFrameWorkID);
        break;
    default: break;
    }
}

void GameFrameWork::OnGamePacket(const GPacket& rPkt) {
    uint16 nc = (uint16)rPkt.GetOpcode();
    switch (nc) {
    case NC_CHAR_STATUS_CHANGE_CMD:                 HandleStatusChange(rPkt);        break;
    case NC_MAP_MOVE_CMD:                           HandlePositionCorrect(rPkt);     break;
    case NC_CHAR_OPTION_KEYMAP_CMD:                 HandleKeyMapCmd(rPkt);           break;
    case NC_CHAR_OPTION_SHORTCUT_CMD:               HandleShortcutCmd(rPkt);         break;
    case NC_CHAR_BRIEFINFO_LOGINCHARACTER_CMD:      HandleBriefInfoLogin(rPkt);      break;
    case NC_CHAR_DISAPPEAR_CMD:                     HandleCharDisappear(rPkt);       break;
    case NC_ACT_MOVERUN_CMD:                        HandleObjMove(rPkt, true);       break;
    case NC_ACT_MOVEWALK_CMD:                       HandleObjMove(rPkt, false);      break;
    case NC_ACT_STOP_CMD:                           HandleObjStop(rPkt);             break;
    default: break;
    }
}

void GameFrameWork::LoadMap() {
    const ShineConfig& cfg = ShineConfig::Get();
    std::string kPath = cfg.MapPath(m_kPlayer.uiMapId, "terrain.sga");
    if (m_kScene.LoadSGA(kPath)) {
        ShineLighting::SetupWorldLighting(m_kScene.GetRoot(), 0.5f);
        m_kCamera.SetTarget(m_kPlayer.fPosX, m_kPlayer.fPosY, m_kPlayer.fPosZ);
        SHINELOG_INFO("GameFrameWork: map %u loaded", m_kPlayer.uiMapId);
    } else {
        SHINELOG_WARN("GameFrameWork: map %u not found -- empty world", m_kPlayer.uiMapId);
        // Still set up lighting on the empty root so the player model renders
        ShineLighting::SetupWorldLighting(m_kScene.GetRoot(), 0.5f);
    }
}

void GameFrameWork::LoadPlayerCharacter() {
    m_kPlayerAssets = CharacterLoader::Get().LoadPlayer(
        m_kPlayer.uiClass, GENDER_MALE, &m_kScene);

    // Position player model at spawn point
    if (m_kPlayerAssets.pkNode) {
        m_kPlayerAssets.pkNode->SetTranslate(
            NiPoint3(m_kPlayer.fPosX, m_kPlayer.fPosY, m_kPlayer.fPosZ));
        m_kPlayerAssets.pkNode->Update(0.0f);
    }

    if (!m_kPlayerAssets.bLoaded)
        SHINELOG_WARN("GameFrameWork: player model not loaded class=%u", m_kPlayer.uiClass);
}

void GameFrameWork::HandleStatusChange(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    body.ReadI32(m_kPlayer.iHP);
    body.ReadI32(m_kPlayer.iSP);
}

void GameFrameWork::HandlePositionCorrect(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    float fX=0,fY=0,fZ=0;
    body.ReadF32(fX); body.ReadF32(fY); body.ReadF32(fZ);
    ShineInput::Get().OnServerPositionCorrect(fX, fY, fZ);
    m_kCamera.SetTarget(fX, fY, fZ);
    if (m_kPlayerAssets.pkNode) {
        m_kPlayerAssets.pkNode->SetTranslate(NiPoint3(fX, fY, fZ));
        m_kPlayerAssets.pkNode->Update(0.0f);
    }
}

void GameFrameWork::HandleKeyMapCmd(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    KeyMap::Get().OnServerKeyMap(body.Data(), (uint32)body.Size());
}

void GameFrameWork::HandleShortcutCmd(const GPacket& rPkt) {
    PacketBuffer body = rPkt.Body();
    m_kHotbar.OnShortcutCmd(body.Data(), (uint32)body.Size());
}

void GameFrameWork::HandleBriefInfoLogin(const GPacket& rPkt) {
    m_kWorldObjs.OnBriefInfoLogin(rPkt);
}

void GameFrameWork::HandleCharDisappear(const GPacket& rPkt) {
    m_kWorldObjs.OnCharDisappear(rPkt);
}

void GameFrameWork::HandleObjMove(const GPacket& rPkt, bool bRun) {
    if (bRun) m_kWorldObjs.OnObjMoveRun(rPkt);
    else      m_kWorldObjs.OnObjMoveWalk(rPkt);
}

void GameFrameWork::HandleObjStop(const GPacket& rPkt) {
    m_kWorldObjs.OnObjStop(rPkt);
}

} // namespace shine
