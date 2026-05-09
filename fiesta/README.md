# Fiesta Online — Original-Style Engine Rewrite

Pass-1 deliverable per `docs/spec_pack/01_MASTER_COMPLETE_REWRITE_REQUIREMENTS.md`.
All 43 systems are scaffolded with original-name function surfaces and stub-but-callable bodies.
Math/formula details that are not yet proven are kept inline as local tunable constants per
`08_FUNCTION_LOCAL_MATH_AND_NO_EXTRA_TUNING_DOCS.md`.

## Important — what this code is and is not

- This is **fresh, originally-authored C++** that re-creates the Fiesta server/client architecture
  using the original-style class and function names from the PDB symbol manifests. Function
  bodies are not copies of decompiled or restored proprietary code.
- `ThirdParty/Gamebryo/` is the vendored Gamebryo 2.3 CoreLibs tree provided by the project
  owner. It is treated as a **read-only third-party SDK**: the client `#include`s its headers
  and links the libraries you build. **Do not modify files under `ThirdParty/Gamebryo/`.**
- Quest SHN files are not parsed by this codebase. The DataReader rejects them by checksum
  category (see `Server/DataReader/ShnDataFileCheckSum.cpp`).

## Tree

```
Server/
  Shared/             # 01 Foundation: timers, packet, crypto, IOCP, WinService, log
  DataReader/         # 02 SHN/TXT/Checksum loader, ITableBase, DataBox, CommonData<T>
  Common/             # 07 NETCOMMAND, packet structs, ProtocolParser, opcode dispatch
  DataServer/
    Account/          # Account DB service exe
    AccountLog/       # AccountLog DB service exe
    Character/        # Character DB service exe
    GameLog/          # GameLog DB service exe
  Login/              # 04 Login service
  WorldManager/       # 05 WM service
  Zone/               # 06 + every gameplay system (08..41) plus a few more
  GamigoZR/           # GamigoZR launcher/stub path
  OperatorTool/       # 39 admin/optool path
Client/
  Engine/             # ClientApp + Gamebryo bootstrap + RenderLoop
  Game/               # Client game-side mirrors of Zone systems
  UI/                 # UI windows / screen layer (mirrors of *Win classes)
  ResSystem/          # 42 Public-name mirror tables (Recv_NC_* / Send_NC_*)
Lua/                  # 21 Lua 5.2 runtime + C bindings
ThirdParty/Gamebryo/  # Vendored Gamebryo 2.3 CoreLibs (read-only)
docs/spec_pack/       # The handoff pack (copy)
Build/                # Build outputs (you populate)
```

## Toolchain expectation (per pack rule)

- Visual Studio 2010 (`v100`) toolset, Win32 (x86) target.
- Lua 5.2 (vendored or system).
- DirectX 9 / D3D10 SDK as expected by Gamebryo.
- SQL Server via OLE DB / ODBC (no new ORM/DB layers introduced).
- Boost is not introduced; only headers that VS2010 ships are used.

## Evidence tags

Every original-name surface carries one of:

```
// EVIDENCE: PDB_CONFIRMED
// EVIDENCE: DATA_CONFIRMED
// EVIDENCE: RUNTIME_CONFIRMED
// EVIDENCE: HEX_CONFIRMED
// EVIDENCE: GAMEPLAY_CONFIRMED
// EVIDENCE: VERIFY  -- shape known, exact opcode/formula unproven
```

`VERIFY` is allowed — pack rule: function coverage before perfect math.

## Build order (per `04_BUILD_ORDER_AND_ACCEPTANCE_TESTS.md`)

1. `Server/Shared` (header-only-ish foundation + a few .cpp)
2. `Server/DataReader` (SHN/TXT loader + ITableBase + DataBox)
3. `Server/Common` (NETCOMMAND + ProtocolParser + packet framework)
4. `Server/DataServer/*` (4 DB service exes)
5. `Server/Login`, `Server/WorldManager`, `Server/Zone`, `Server/GamigoZR`,
   `Server/OperatorTool`
6. `Lua` runtime + C bindings
7. `Client/*` (links against ThirdParty/Gamebryo)

## Acceptance status (pass 1)

- [x] 00 Bootstrap / WinService / EHCrashHandler / Socket_Acceptor
- [x] 01 Shared / Foundation
- [x] 02 DataReader / SHN / Checksum (quest SHNs guarded)
- [x] 03 Database / SQLP layer (skeleton)
- [x] 04..42 every system has at least operational stubs with original names
- [x] 21 Lua 5.2 binding shims
- [x] 42 Client mirror + Gamebryo bootstrap
- [ ] Final packet opcode proofing — every NC opcode is tagged `VERIFY` or
      `PDB_CONFIRMED` per available evidence; runtime capture proofing pass owed.
- [ ] Final formula tuning — local constants in Battle/Skill/Drop/Item upgrade
      need real-game replay to set.

See `docs/PASS1_NOTES.md` for what each system stub does today.
