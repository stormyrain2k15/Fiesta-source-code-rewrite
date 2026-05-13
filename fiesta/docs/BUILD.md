# Build expectations

> Build-system files (`.vcxproj`, `.sln`) are **not** generated here -- you handle
> compilation. This document lists what each project should target so the tree
> matches the original NA2016 production layout exactly.

## Production exe naming

Per real-shipping Shine builds the executables are renamed at link time. The
numeric prefix is the boot order. Match the names below verbatim if you want
existing operator scripts (`runall.bat`, `taskkill /im ...`) to keep working:

| Source dir                       | Output exe name (Release/x86) |
|----------------------------------|-------------------------------|
| `Server/Login/`                  | **`3LoginServer2.exe`**       |
| `Server/WorldManager/`           | **`4WorldManagerServer2.exe`**|
| `Server/Zone/`                   | **`5ZoneServer2.exe`**        |
| `Server/DataServer/Account/`     | **`Account Release.exe`**     |
| `Server/DataServer/AccountLog/`  | **`AccountLog Release.exe`**  |
| `Server/DataServer/Character/`   | **`Character Release.exe`**   |
| `Server/DataServer/GameLog/`     | **`GameLog_Release.exe`**     |
| `Client/Engine/`                 | `Shine.exe`                  |

The space (`Account Release.exe`) and underscore (`GameLog_Release.exe`) styles
are inconsistent in the original drop; preserve them as shown so symbol-server
lookups against shipped PDBs still match.

> **Note on `GamigoZR`** -- removed from this tree as of . The "ZR"
> service was an EU-side workaround for an ancient client-version security
> check inside the Zone server, not a real game function. With that check
> disabled (see ) the workaround is unnecessary.

> **Note on `Account.exe` (309 KB, Mar 2 build)** in the file-listing
> screenshot -- that is the operator-side admin tool, distinct from
> `Account Release.exe`. Not part of this tree; the in-game flows it
> exercises live in `Server/OperatorTool/`.

## Solution layout (suggested)

`Shine.sln` containing one project per service exe + a static lib for
`Server/Shared`, `Server/Common`, `Server/DataReader`,
`Server/DataServer/Common`, plus the client exe.

| Project                  | Type      | Source roots |
|--------------------------|-----------|---|
| `ShineCore`              | StaticLib | `Server/Shared` + `Server/Common` + `Server/DataReader` + `Server/DataServer/Common` |
| `LuaRuntime`             | StaticLib | `Lua/LuaRuntime.cpp` + `Lua/LuaCBindings.cpp` |
| `Account` (exe)          | WinExe    | `Server/DataServer/Account/Main.cpp`     -> ShineCore                    -> rename to `Account Release.exe` |
| `AccountLog` (exe)       | WinExe    | `Server/DataServer/AccountLog/Main.cpp`  -> ShineCore                    -> rename to `AccountLog Release.exe` |
| `Character` (exe)        | WinExe    | `Server/DataServer/Character/Main.cpp`   -> ShineCore                    -> rename to `Character Release.exe` |
| `GameLog` (exe)          | WinExe    | `Server/DataServer/GameLog/Main.cpp`     -> ShineCore                    -> rename to `GameLog_Release.exe` |
| `LoginServer2` (exe)     | WinExe    | `Server/Login/*.cpp`                     -> ShineCore                    -> rename to `3LoginServer2.exe` |
| `WorldManagerServer2`(exe)| WinExe   | `Server/WorldManager/*.cpp`              -> ShineCore                    -> rename to `4WorldManagerServer2.exe` |
| `ZoneServer2` (exe)      | WinExe    | `Server/Zone/*.cpp`                      -> ShineCore + LuaRuntime       -> rename to `5ZoneServer2.exe` |
| `Shine` (client exe)    | WinExe    | `Client/**/*.cpp` -> ShineCore + ThirdParty/Gamebryo libs |

## Required additional include / lib paths

| Project | Include adds | Library adds |
|---|---|---|
| `LuaRuntime`      | path to a Lua 5.2 source/include drop | `lua52.lib` |
| `ZoneServer2`     | (transitively) Lua 5.2 include        | `lua52.lib` |
| `Shine` (client) | `ThirdParty/Gamebryo/NiMain`, `NiSystem`, `NiDX9Renderer`, `NiAnimation`, `NiCollision`, `NiParticle`, `NiPortal`, `NiAudio`, `NiFloodgate` | the corresponding Gamebryo `*.lib` (release/shippinglib variant under each `Win32/VC80/...`) |
| All exes          | `Server/Shared`, `Server/Common`, `Server/DataReader` | `Ws2_32.lib`, `DbgHelp.lib`, `odbc32.lib` |

## Build flags

- Toolset: VS 2010 (`v100`) / VS 2012 (`v110`) -- both work.
- Platform: Win32 (x86).
- Runtime: `/MT` (static) for service exes; the original layout shipped static-CRT.
- Define: `_WIN32_WINNT=0x0501` minimum.
- Disable warning C4996 (deprecated CRT) -- `_CRT_SECURE_NO_WARNINGS`.

## Run order (manual smoke test)

```
1.  "Account Release.exe"        -console   :27600
2.  "AccountLog Release.exe"     -console   :27601
3.  "Character Release.exe"      -console   :27602
4.  "GameLog_Release.exe"        -console   :27603
5.  "4WorldManagerServer2.exe"   -console   :28000..:28004
6.  "3LoginServer2.exe"          -console   :9010
7.  "5ZoneServer2.exe"           -console   :28100   (Zone.Id=0 in ZoneServerInfo.txt)
8.  "Shine.exe"                            -> connects to 127.0.0.1:9010
```

`ServerInfo.txt` and `ZoneServerInfo.txt` are simple `key=value` files; defaults
are baked into each `Main.cpp` so the services start even without one.
