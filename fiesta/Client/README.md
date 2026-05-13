# Shine Client Bootstrap

Full client source built against Gamebryo 2.3, wired to the Shine server stack.
No XTrap. No SSO. No WebCtrl. No NetMarble branding. Clean.

## Architecture

Matches the original MainApp.cpp pattern:

```
WinMain
  └─ NiApplication::Run()
       └─ NiApplication::Create()  ← ShineApp factory
            └─ ShineApp::Initialize()
                 └─ Pgg_kFrameMgr.Start(AccountFrameWorkID)
                      ├─ AccountFrameWork  (login → WM → Zone)
                      └─ GameFrameWork     (in-world, driven by NC packets)
```

## File Structure

```
shine/Client/
  Engine/
    Main.cpp              WinMain -- loads configs, calls NiApplication::Run()
    ShineApp.h/.cpp       NiApplication subclass -- NiApplication::Create() factory,
                          CreateRenderer() override, framework manager driver
    ShineRenderer.h/.cpp  MachineOpt-driven DX9 renderer -- validates display modes,
                          AA, VSync, fullscreen, applies option changes at runtime
    MachineOpt.h/.cpp     Player-adjustable settings -- resolution, sound, game options
                          Saved to ShineOption.cfg
    ShineConfig.h/.cpp    Server/path config -- IPs, ports, asset paths
                          Read from ShineClient.ini
    ShineScene.h/.cpp     NiNode scene graph wrapper -- loads .sga (NIF) assets
    ShineCamera.h/.cpp    Third-person chase camera -- mouse orbit, wheel zoom
    ClientApp.h/.cpp      DEPRECATED -- network moved into AccountFrameWork

  Framework/
    ShineFrameWork.h/.cpp  Pgg_kFrameMgr -- state machine, AddMsg(), IsInRun()
    AccountFrameWork.h/.cpp Login/WM/ZoneSession -- handles NC_CHAR_OPTION_KEYMAP_CMD
    GameFrameWork.h/.cpp   In-world -- scene/camera/UI/input, packet handlers

  Network/
    ShineNetClient.h/.cpp  IOCP outbound TCP -- same wire format as server
    LoginSession.h/.cpp    NC_USER_* state machine
    WMSession.h/.cpp       NC_WM_* state machine (between login and zone)
    ZoneSession.h/.cpp     NC_CHAR_* state machine, world tick keepalive

  Input/
    ShineInput.h/.cpp      WASD movement -- camera-relative, dead-reckoning,
                           NC_ACT_MOVERUN_CMD / NC_ACT_MOVEWALK_CMD / NC_ACT_STOP_REQ
    KeyMap.h/.cpp          95 key bindings from tKeyMapInit (Key_Remapping.sql)
                           Server override via NC_CHAR_OPTION_KEYMAP_CMD (0x7033)
                           Saved to ShineKeys.cfg

  UI/
    ShineUI.h/.cpp         Loads resmenu\game\ + resctrl\ DDS textures via NiSourceTexture
                           Positions driven by UILayout -- Alt+drag to move panels
                           Saved to ShineUI.cfg
    UILayout.h/.cpp        Panel position save/load -- ShineUI.cfg, per-panel float
                           Drag system, clamped to screen bounds, save on drag-end
    ShineHUD.h/.cpp        DEPRECATED -- replaced by ShineUI

  ResSystem/
    PEResourceReader.h/.cpp     FindResource/LoadResource -- zero-copy PE section reads
    ShineResourceLoader.h/.cpp  PE first, disk fallback (SHINE_EMBED_ONLY disables disk)
    ActionDat.h/.cpp            ressystem\action\<InxName>.dat loader (352 bytes/entry)
                                Verified against ActionReader v12 / ActionFile.cs
    ActionDatCache              Singleton cache keyed by InxName
    CharacterLoader.h/.cpp      class ID → InxName → body.sga + action dat
```

## Config Files (beside exe)

| File              | Contents                                      |
|-------------------|-----------------------------------------------|
| ShineClient.ini   | Server IPs, ports, asset paths, debug flags   |
| ShineOption.cfg   | Resolution, fullscreen, sound, game options   |
| ShineKeys.cfg     | Key bindings (from tKeyMapInit / server)      |
| ShineUI.cfg       | UI panel positions (Alt+drag to adjust)       |

## Build Setup (Visual Studio)

**Include paths:**
```
$(GB)\CoreLibs\NiMain\Win32\VC80\Include
$(GB)\CoreLibs\NiAnimation\Win32\VC80\Include
$(GB)\CoreLibs\NiSystem\Win32\VC80\Include
$(GB)\AppFrameworks\NiApplication
$(SolutionDir)shine\Server\Shared
$(SolutionDir)shine\Server\Common
```

**Libraries:**
```
NiMain.lib  NiAnimation.lib  NiSystem.lib
NiDX9Renderer.lib  NiApplication.lib
d3d9.lib  d3dx9.lib  Ws2_32.lib
```

**Preprocessor defines:**
```
SHINE_CLIENT          (enables PE resource loading in ActionDatCache)
SHINE_EMBED_ONLY      (release builds -- disables disk fallback for SHNs/DATs)
```

## Network Flow

```
Login:9010
  VERSION_REQ → SEED_ACK → cipher init
  LOGIN_REQ → LOGIN_ACK
  WORLDSELECT_REQ → WORLDSELECT_ACK (WM ip + 16-byte token)

WM:28000
  CHAR_LOGIN_REQ [accountId, token]
  ← CHAR_LOGIN_ACK
  WM_CHARSELECT_REQ [charId]
  ← WM_ZONE_ASSIGN_CMD [zoneIp, zonePort, zoneToken]

Zone (ip from WM)
  CHAR_LOGIN_REQ [accountId, charId, token]
  ← CHAR_LOGIN_ACK [handle]
  ← CHAR_OPTION_KEYMAP_CMD (0x7033) -- seeds KeyMap, saved to ShineKeys.cfg
  ← CHAR_INFO_CMD [level, class, hp, sp, pos, mapId]
  → [in-world stream]
```

## Dropped from original client

- XTrap anti-cheat
- SSOWebLib / WebCtrl (browser-based login)
- ShineException server crash reporter
- NetMarble logo video / branding
- Locale version defines (_LOCALE_VER_KR etc.)
- Renderer selection dialog (NiDX9Select)
