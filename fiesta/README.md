# Fiesta Online — Original-Style Engine Rewrite

Native C++ reimplementation of the NA2016-era Fiesta Online server and
client engine. The 43 gameplay systems all carry their original
class / function names so the layout matches the published PDB symbol
manifests one-for-one.

## What this is, and is not

- This is **fresh, originally-authored C++**. The class and function
  surface mirrors the original PDB symbol manifests; bodies are written
  from the published documentation and from the data layout in the SHN /
  TS / .dat files. Nothing in here is decompiled or restored from
  proprietary binaries.
- `ThirdParty/Gamebryo/` is the vendored Gamebryo 2.3 CoreLibs tree
  provided by the project owner. It is treated as a **read-only**
  third-party SDK: the client `#include`s its headers and links the
  libraries you build. Do not modify files under `ThirdParty/Gamebryo/`.

## Build and run

VS2010 toolset (`v100`), Win32 only. See `docs/BUILD.md` for the project
file layout and the four DB exes (`Account`, `AccountLog`, `Character`,
`GameLog`).

## Source-tree map

```
Server/Shared      foundation: timers, packet, crypto, IOCP, log
Server/Common      NETCOMMAND, ProtocolParser, GPacket
Server/DataReader  SHN / TS / DAT / BMP loaders + ITableBase
Server/DataServer  the four DB exes
Server/Login       credential authority
Server/WorldManager  world router + OPTool loopback
Server/Zone        gameplay engine + 43 subsystems
Lua/               Lua 5.2 host + cBindings
Client/            client shell + Gamebryo bootstrap
ThirdParty/Gamebryo/  read-only vendored renderer
docs/              build notes + spec pack
```

## Persistent state

Every gameplay event that mutates lasting character state issues a real
SQL stored-proc call through one of the DB exes. See `memory/PRD.md` for
the per-event routing table.

## Data ingest

`9Data/Shine/<Map>/...` is walked at boot by `ZoneAssetLoader`. Every
SHN column is either hand-bound by a typed reader or audited by
`ShnAudit_EmitReport`; every World/*.txt column the same way via
`TsAudit_EmitReport`. Unread columns surface as a single WARN line per
`(table.column)` in the boot log.
