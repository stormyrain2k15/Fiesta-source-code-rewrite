# Shine Client — Phase 1 Bootstrap

Full source client wired to the Shine server stack.

## Project Structure

```
fiesta/Client/
  Engine/
    Main.cpp            WinMain — reads ShineClient.ini, runs ClientApp
    ClientApp.h/.cpp    Top-level coordinator — owns login/zone sessions,
                        bridges net worker thread to render thread
    ShineApp.h/.cpp     NiApplication subclass — render loop, scene, camera
    ShineScene.h/.cpp   Scene graph wrapper — loads .sga (NIF) assets
    ShineCamera.h/.cpp  Third-person chase camera — mouse orbit, wheel zoom
  Network/
    ShineNetClient.h/.cpp   IOCP outbound TCP socket — same wire format as server
    LoginSession.h/.cpp     NC_USER_* state machine — login → world select
    ZoneSession.h/.cpp      NC_CHAR_* state machine — char login → in-world
  UI/
    ShineHUD.h/.cpp     HP/SP bars via NiScreenElements
```

## Build Setup (Visual Studio)

1. Add to existing project or create new Win32 exe project.

2. Include paths (add to project C/C++ → Additional Include Directories):
   ```
   $(SolutionDir)Gamebryo_2.3\CoreLibs\NiMain\Win32\VC80\Include
   $(SolutionDir)Gamebryo_2.3\CoreLibs\NiAnimation\Win32\VC80\Include
   $(SolutionDir)Gamebryo_2.3\CoreLibs\NiSystem\Win32\VC80\Include
   $(SolutionDir)Gamebryo_2.3\AppFrameworks\NiApplication
   $(SolutionDir)fiesta\Server\Shared
   $(SolutionDir)fiesta\Server\Common
   ```

3. Library paths (Linker → Additional Library Directories):
   ```
   $(SolutionDir)Gamebryo_2.3\SDK\Win32\VC80\$(Configuration)
   ```

4. Libraries (Linker → Additional Dependencies):
   ```
   NiMain.lib NiAnimation.lib NiSystem.lib NiDX9Renderer.lib
   NiApplication.lib Ws2_32.lib d3d9.lib d3dx9.lib
   ```

5. Place `ShineClient.ini` next to the built exe.

## Network Flow

```
[Login:9010]
  VERSION_REQ → SEED_ACK → cipher init
  LOGIN_REQ → LOGIN_ACK
  WORLDSELECT_REQ → WORLDSELECT_ACK (WM ip + 16-byte token)

[Zone (ip from WM)]
  CHAR_LOGIN_REQ [accountId, charId, token]
  ← CHAR_LOGIN_ACK [handle]
  ← CHAR_INFO_CMD  [level, class, hp, sp, pos, mapId]
  → [in-world packet stream]
```

## Asset Convention

Assets live under `Assets\` beside the exe:
```
Assets\
  Graphics\
    Map\
      <mapId>\
        terrain.sga     (NIF binary, renamed extension)
    Character\
    Monster\
  Data\
    *.shine             (SHN binary, renamed extension)
```

## Phase 1 State

- [x] IOCP client socket with stream cipher
- [x] Full login handshake state machine  
- [x] Zone character login state machine
- [x] NiApplication render loop
- [x] .sga scene loading (NiStream, extension-transparent)
- [x] Third-person chase camera
- [x] HP/SP HUD bars
- [ ] WM handshake (phase-1 connects direct to zone — add WMSession for full path)
- [ ] Character model loading (add per-class .sga path lookup from CharEngine)
- [ ] Movement input → NC_ACT_MOVERUN_CMD
- [ ] UI overhaul (replace NiScreenElements HUD with full UI framework)
- [ ] TTS voice profile system
- [ ] Cutscene player
