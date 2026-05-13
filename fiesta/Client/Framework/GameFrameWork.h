// Client/Framework/GameFrameWork.h
// In-world game framework -- owns scene/camera/UI/input/sound/objects.
#ifndef SHINE_CLIENT_FRAMEWORK_GAMEFRAMEWORK_H
#define SHINE_CLIENT_FRAMEWORK_GAMEFRAMEWORK_H

#include "ShineFrameWork.h"
#include "../Engine/ShineScene.h"
#include "../Engine/ShineCamera.h"
#include "../Engine/ShineLighting.h"
#include "../UI/ShineUI.h"
#include "../UI/ShineHotbar.h"
#include "../Input/ShineInput.h"
#include "../Input/KeyMap.h"
#include "../ResSystem/CharacterLoader.h"
#include "../World/WorldObjectMgr.h"
#include "../Sound/ShineSoundMgr.h"
#include "../Network/ZoneSession.h"

namespace shine {

class GameFrameWork : public ShineFrameWork {
public:
    GameFrameWork();

    virtual void OnStart()                           override;
    virtual void OnStop()                            override;
    virtual bool Update(float fDt)                   override;
    virtual void Render()                            override;
    virtual void OnMsg(const FrameWorkMsgData& rMsg) override;

    void SetupWorld(const PlayerState& rPlayer, ZoneSession* pkZone,
                    NiRenderer* pkRenderer);
    void OnGamePacket(const GPacket& rPkt);

    ShineScene*     GetScene()      { return &m_kScene; }
    ShineCamera*    GetCamera()     { return &m_kCamera; }
    ShineUI*        GetUI()         { return &m_kUI; }
    ShineHotbar*    GetHotbar()     { return &m_kHotbar; }
    WorldObjectMgr* GetWorldObjs()  { return &m_kWorldObjs; }

private:
    ShineScene      m_kScene;
    ShineCamera     m_kCamera;
    ShineUI         m_kUI;
    ShineHotbar     m_kHotbar;
    WorldObjectMgr  m_kWorldObjs;
    CharacterAssets m_kPlayerAssets;
    ZoneSession*    m_pkZone;
    NiRenderer*     m_pkRenderer;
    PlayerState     m_kPlayer;
    bool            m_bWorldReady;

    void LoadMap();
    void LoadPlayerCharacter();
    void HandleStatusChange      (const GPacket& rPkt);
    void HandlePositionCorrect   (const GPacket& rPkt);
    void HandleKeyMapCmd         (const GPacket& rPkt);
    void HandleShortcutCmd       (const GPacket& rPkt);
    void HandleBriefInfoLogin    (const GPacket& rPkt);
    void HandleCharDisappear     (const GPacket& rPkt);
    void HandleObjMove           (const GPacket& rPkt, bool bRun);
    void HandleObjStop           (const GPacket& rPkt);
};

} // namespace shine
#endif // SHINE_CLIENT_FRAMEWORK_GAMEFRAMEWORK_H
