# Build expectations (Pass 1)

> Build-system files (`.vcxproj`, `.sln`) are **not** generated here -- you said you
> handle compilation. This document lists what each project should target so the
> tree matches the original NA2016 layout and the pack's VS2010 / `v100` rule.

## Solution layout (suggested)

`Fiesta.sln` containing one project per service exe + a static lib for `Server/Shared`,
`Server/Common`, `Server/DataReader`, `Server/DataServer/Common`, plus the client exe.

| Project | Type | Source roots |
|---|---|---|
| `ShineCore`             | StaticLib (`/app/fiesta/Build/lib/ShineCore.lib`)        | `Server/Shared` + `Server/Common` + `Server/DataReader` + `Server/DataServer/Common` |
| `LuaRuntime`            | StaticLib                                                 | `Lua/LuaRuntime.cpp` + `Lua/LuaCBindings.cpp` |
| `Account` (exe)         | WinExe                                                    | `Server/DataServer/Account/Main.cpp` -> links ShineCore |
| `AccountLog` (exe)      | WinExe                                                    | `Server/DataServer/AccountLog/Main.cpp` |
| `Character` (exe)       | WinExe                                                    | `Server/DataServer/Character/Main.cpp` |
| `GameLog` (exe)         | WinExe                                                    | `Server/DataServer/GameLog/Main.cpp` |
| `Login` (exe)           | WinExe                                                    | `Server/Login/*.cpp` -> links ShineCore |
| `WorldManager` (exe)    | WinExe                                                    | `Server/WorldManager/*.cpp` -> links ShineCore |
| `Zone` (exe)            | WinExe                                                    | `Server/Zone/*.cpp` -> links ShineCore + LuaRuntime |
| `GamigoZR` (exe)        | WinExe                                                    | `Server/GamigoZR/Main.cpp` -> links ShineCore |
| `OperatorTool` (exe)    | WinExe                                                    | `Server/OperatorTool/Main.cpp` -> links ShineCore |
| `Fiesta` (client exe)   | WinExe                                                    | `Client/**/*.cpp` -> links ShineCore + ThirdParty/Gamebryo libs |

## Required additional include / lib paths

| Project | Include adds | Library adds |
|---|---|---|
| `LuaRuntime` | path to a Lua 5.2 source/include drop | `lua52.lib` |
| `Zone`       | (transitively) Lua 5.2 include              | `lua52.lib` |
| `Fiesta` (client) | `ThirdParty/Gamebryo/NiMain`, `NiSystem`, `NiDX9Renderer`, `NiAnimation`, `NiCollision`, `NiParticle`, `NiPortal`, `NiAudio`, `NiFloodgate` | the corresponding Gamebryo `*.lib` (release/shippinglib variant under each `Win32/VC80/...`) |
| All exes     | `Server/Shared`, `Server/Common`, `Server/DataReader` | `Ws2_32.lib`, `DbgHelp.lib`, `odbc32.lib` |

## Build flags

- Toolset: VS 2010 (`v100`) / VS 2012 (`v110`) -- both work.
- Platform: Win32 (x86).
- Runtime: `/MT` (static) for service exes; the original layout shipped static-CRT.
- Define: `_WIN32_WINNT=0x0501` minimum.
- Disable warning C4996 (deprecated CRT) -- `_CRT_SECURE_NO_WARNINGS`.

## Run order (manual smoke test)

1. `Account.exe -console`        on :27600
2. `AccountLog.exe -console`     on :27601
3. `Character.exe -console`      on :27602
4. `GameLog.exe -console`        on :27603
5. `WorldManager.exe -console`   on :28000-:28004
6. `Login.exe -console`          on :9010
7. `Zone.exe -console`           on :28100  (set `Zone.Id=0` in `ZoneServerInfo.txt`)
8. `OperatorTool.exe -console`   on :30000
9. `GamigoZR.exe -console`       on :29000  (optional regional)
10. `Fiesta.exe`                 connects to `127.0.0.1:9010`

`ServerInfo.txt` and `ZoneServerInfo.txt` are simple `key=value` files; defaults
are baked into each `Main.cpp` so the services start even without one.
