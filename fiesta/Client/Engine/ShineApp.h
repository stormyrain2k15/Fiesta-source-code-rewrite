// Client/Engine/ShineApp.h
// ShineApp: NiApplication subclass.
// Owns the Gamebryo render loop, scene graph, and camera.
// Wired to ShineNetClient through ClientApp which calls Tick() and Render()
// on each frame from the Win32 message pump.
//
// NiApplication lifecycle (Gamebryo 2.3):
//   NiInit()                       -- global Ni* subsystem startup
//   NiApplication::Initialize()    -- virtual, called once before main loop
//   NiApplication::UpdateFrame()   -- virtual, called each frame
//   NiApplication::RenderFrame()   -- virtual, called each frame
//   NiApplication::Terminate()     -- virtual, cleanup
//   NiShutdown()                   -- global teardown
//
// Renderer selection: D3D9 by default (NiDX9Renderer). D3D10 path exists
// in Gamebryo 2.3 but Fiesta assets are D3D9 era. Switch to D3D10 or your
// own renderer by overriding CreateRenderer().
#ifndef SHINE_CLIENT_ENGINE_SHINEAPP_H
#define SHINE_CLIENT_ENGINE_SHINEAPP_H

// Gamebryo headers -- add ThirdParty/Gamebryo include paths in project settings.
#include <NiApplication.h>
#include <NiMain.h>
#include <NiAnimation.h>
#include <NiDX9Renderer.h>

#include "../../Server/Shared/ShineTypes.h"
#include "ShineScene.h"
#include "ShineCamera.h"
#include "../UI/ShineHUD.h"
#include "../Network/ZoneSession.h"
#include <string>

namespace fiesta {

class ShineApp : public NiApplication {
    NiDeclareRTTI;
public:
    ShineApp();
    virtual ~ShineApp();

    // NiApplication overrides
    virtual bool    Initialize()   override;
    virtual bool    UpdateFrame()  override;
    virtual bool    RenderFrame()  override;
    virtual void    Terminate()    override;

    // Called by ClientApp when zone is ready with player state
    void OnZoneReady(const PlayerState& rPlayer);

    // Called by ClientApp each game tick (from WinMain pump, not render thread)
    void OnGamePacket(const GPacket& rPkt);

    // Asset root (path to Assets/ dir set by ClientApp before Initialize)
    void SetAssetRoot(const std::string& rRoot) { m_kAssetRoot = rRoot; }

    ShineScene*  GetScene()  { return &m_kScene; }
    ShineCamera* GetCamera() { return &m_kCamera; }
    ShineHUD*    GetHUD()    { return &m_kHUD; }

private:
    std::string  m_kAssetRoot;
    ShineScene   m_kScene;
    ShineCamera  m_kCamera;
    ShineHUD     m_kHUD;

    bool         m_bWorldReady;
    PlayerState  m_kPlayer;

    // Frame timing
    float        m_fAccumDt;
    DWORD        m_dwLastTick;

    float        GetDeltaTime();
    bool         LoadStartMap();
};

} // namespace fiesta
#endif // SHINE_CLIENT_ENGINE_SHINEAPP_H
