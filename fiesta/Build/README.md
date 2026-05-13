# Build Instructions (Visual Studio 2010, Win32)

This directory contains the canonical Visual Studio 2010 project layout
for the entire Shine server + client tree.

## What's here

| File                                  | Purpose                                                     |
|---------------------------------------|-------------------------------------------------------------|
| `Shine.sln`                          | Master solution. Open this in VS2010.                       |
| `Gamebryo.props`                      | Property sheet for the Gamebryo SDK include / lib paths.    |
| `gen_vcxproj.py`                      | Re-generates every `.vcxproj` + `.filters` from disk.       |
| `Common.vcxproj`                      | StaticLib: NETCOMMAND, ProtocolParser, SendPacket.          |
| `Shared.vcxproj`                      | StaticLib: IOCP, sockets, encryption, packet buffers.       |
| `DataReader.vcxproj`                  | StaticLib: SHN/DAT/SHBD/AID/SBI parsers, ShnRegistry.       |
| `LuaRuntime.vcxproj`                  | StaticLib: Lua 5.2 runtime + bindings + generated APIs.     |
| `DataServerCommon.vcxproj`            | StaticLib: SQLP, Database (shared by all 4 DataServer EXEs).|
| `Login.vcxproj`                       | EXE: LoginServer.                                           |
| `DataServer.Account.vcxproj`          | EXE: account DB service.                                    |
| `DataServer.AccountLog.vcxproj`       | EXE: account-log DB service.                                |
| `DataServer.Character.vcxproj`        | EXE: CharDB service.                                        |
| `DataServer.GameLog.vcxproj`          | EXE: game-log DB service.                                   |
| `WorldManager.vcxproj`                | EXE: WorldManagerServer (KQ, GuildServer, GMEvent, etc.).   |
| `Zone.vcxproj`                        | EXE: ZoneServer (the big one -- 321 cpp).                   |
| `Client.vcxproj`                      | EXE: native game client.                                    |

## Configuration

* **Toolset**: v100 (VS2010)
* **Platform**: Win32 (x86 only)
* **Character set**: Multi-byte (MBCS)
* **Runtime library**: Multi-threaded (`/MT`) Release, Multi-threaded Debug (`/MTd`) Debug
* **Output dirs**:
    * `Bin/Debug/` and `Bin/Release/` for all `.exe` and `.lib`
    * `Obj/<Cfg>/<Project>/` for intermediates

## First-time setup

1. Open `Shine.sln` in Visual Studio 2010.
2. **Edit `Gamebryo.props`** -- set `<GamebryoRoot>` to wherever your
   Gamebryo SDK lives. Default points at `..\ThirdParty\Gamebryo\` (the
   read-only in-tree copy). If you have a fuller SDK install elsewhere,
   change the macro and every project picks it up.
3. **Edit `Lua.props`** -- set `<LuaRoot>` to your Lua 5.2.x install
   (must be 5.2; 5.1 and 5.4 are NOT compatible with the binding layer
   in `Lua/LuaRuntime.cpp`). Default expects an in-tree drop at
   `..\ThirdParty\Lua\{include,lib}` containing `lua.h`, `lauxlib.h`,
   `lualib.h`, and `lua52.lib`. The included `*.props` adds
   `lua52.lib` to every project's link line.
3. **MSSQL Native Client** -- `DataServer.*` projects link against
   `sqlncli11.lib` via `#pragma comment(lib, ...)` in `Database.cpp`.
   Install the SQL Server Native Client SDK and make sure its
   `lib\x86\` is on the system lib path, or add it to
   `VC++ Directories -> Library Directories` for each DataServer project.
5. **Build order** -- VS resolves automatically from `ProjectSection
   (ProjectDependencies)` blocks in `Shine.sln`. Order resolves to:
   ```
   Common, Shared, LuaRuntime, DataReader, DataServerCommon
       -> Login, DataServer.*
           -> WorldManager
               -> Zone, Client
   ```
6. Set **Zone** as the Startup Project (or whichever you're debugging)
   and hit F5.

## Re-generating after adding files

When you add or rename files anywhere in `Server/` or `Client/`, run:

```
cd Build
python3 gen_vcxproj.py
```

This rewrites every `.vcxproj` + `.filters` from a fresh directory walk
and updates `Shine.sln`. GUIDs are derived from project name (UUIDv5)
so they remain stable across regenerations -- diffs are clean and
project references never break.

`*.vcxproj.user` files are emitted blank; they hold per-developer
settings (debug args, working dir) and are usually `.gitignore`d.

## Troubleshooting

* **"Cannot find sqlncli11.h"** -- install SQL Server Native Client
  SDK (or change DataServer projects to use sqlncli10/sqlsrv32 via the
  Database.cpp `#pragma comment` line).
* **"Cannot find lua.h"** -- `Lua.props` is pointing at the wrong
  path. Edit `<LuaRoot>` in that file. Drop a Lua 5.2.x build with
  headers and `lua52.lib` at the indicated location.
* **"Cannot find NiMain.h"** -- `Gamebryo.props` is pointing at the
  wrong path. Edit `<GamebryoRoot>` in that file.
* **LNK2005 collisions in Zone** -- one of the anonymous-namespace
  stub files has been pulled out of its anonymous namespace without
  removing the duplicate declaration in the matching aggregate header.
  See `docs/HONEST_DISCLOSURE.md` section 1 for the stub list.
* **C2065 unknown identifier in DataReader** -- SchemaGen output is
  stale. Re-run SchemaGen against the SHN headers and rebuild.

## Notes

* The 320-file Zone project is intentionally one monolithic EXE
  matching the NA2016 PDB layout. Don't split it without consulting
  `docs/HONEST_DISCLOSURE.md` first -- many of the "files" are
  facade stubs whose ODR resolution depends on staying in their
  current TU shape.
* `MultiProcessorCompilation` is on so the Zone TU set parallelises
  across cores. `MinimalRebuild` is off (incompatible with `/MP`).
* The Gamebryo property sheet is imported with
  `Condition="exists(...)"` so checkout-without-Gamebryo still loads
  the solution; you only need the SDK to build `Client.vcxproj`.
