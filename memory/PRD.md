# PRD — Fiesta Online Original-Style Engine Rewrite

## Original problem statement

Rewrite the Fiesta Online server + client engine from the `fiesta_emergent_complete_rewrite_pack`
spec, integrating the vendored `ElleAnn/Engine/Gamebryo_2.3_CoreLibs/` engine source as
the client renderer. Identical NA2016-style layout. Reuse PDB symbol names. Lua 5.2.
"All systems at least operational, bugs expected." User compiles; agent writes code only.

## Architecture

- Native C++ (VS2010 / `v100` / Win32) MMORPG service stack.
- Layout under `/app/fiesta/`:
  - `Server/Shared` — foundation (timers, packet, crypto, IOCP, WinService, log)
  - `Server/Common` — NETCOMMAND, ProtocolParser, packet framework
  - `Server/DataReader` — SHN/TXT/Checksum loader, ITableBase, DataBox
  - `Server/DataServer/{Account,AccountLog,Character,GameLog}` — 4 DB service exes
  - `Server/Login`, `Server/WorldManager`, `Server/Zone`, `Server/GamigoZR`, `Server/OperatorTool`
  - `Lua/` — Lua 5.2 host + cBindings (cDamaged / cStaticDamage / cSetAbstate / cLinkTo / cFinishKey)
  - `Client/{Engine,ResSystem,...}` — client shell + Gamebryo bootstrap
  - `ThirdParty/Gamebryo/` — vendored Gamebryo 2.3 CoreLibs (read-only, untouched)
  - `docs/spec_pack/` — handoff pack (copy of zip)
  - `docs/{PASS1_NOTES.md,BUILD.md}` — system-by-system status + project wiring

## What's been implemented (Pass 1, 2026-05-09)

- 111 fresh-authored C++ source files, 0 modifications to Gamebryo internals.
- Foundation, network/IOCP, NETCOMMAND opcode space, SHN/TXT loader with quest-SHN guard,
  MD5 checksum, packet cipher scaffold, CToken Login→WM bridge.
- All 43 pack systems (00..42) have original-named classes with operational stubs
  or real implementations. See `docs/PASS1_NOTES.md` for per-system status table.
- Lua 5.2 runtime + the five canonical script bindings.
- Client shell with Gamebryo integration points (forward-declared until VS project
  picks up the vendored Gamebryo include/lib paths).
- Build expectations / project layout in `docs/BUILD.md` (no `.vcxproj` generated;
  user handles compilation).

## Pack rule compliance

| Rule | Status |
|---|---|
| Original PDB names reused | Yes (every class/function name is PDB-derived) |
| No new tuning JSON / SQL / config layers | Yes (provisional values inline as local consts) |
| Quest SHNs protected | Yes (`ShnDataFileCheckSum::IsQuestSHN`, `QuestParserScript::LoadCompiled` refuses `.shn`) |
| Lua 5.2 (not 5.4) | Yes |
| VS2010-compatible C++ | Yes (no C++11 syntax) |
| Gamebryo untouched | Yes (vendored read-only at `ThirdParty/Gamebryo/`) |
| Evidence-tagged | Yes (`EVIDENCE: PDB_CONFIRMED` / `VERIFY` markers throughout) |

## Backlog (Pass 2+)

P0:
- Pin every NC opcode to capture-proven values (edit `Server/Common/NETCOMMAND.h` in place).
- Decode the binary `.shn` row format inside `DataReader::LoadAsRows`.
- Wire SQLP procedure parameter shapes against the restored `World00_Character.bak` schema.

P1:
- Replace forward-decl Gamebryo types in `Client/Engine/ClientApp.cpp` with real
  `<NiMain.h>` / `<NiDX9Renderer.h>` / etc. `#include`s once VS project paths are added.
- Per-player `CharacterSkill` / `AbnormalState` attachment on Zone CharLogin.
- Real CharDataLoad through `WMCharDBSession` (current `FillFromCharLogin` uses a
  provisional level/class fill).

P2:
- Deepen subsystem ticks: Battle aggro list, AbState save-on-link, Booth persistence,
  GuildWar declaration window, ItemMall billing flow.
- Unit tests for opcode round-trip on PacketBuffer / GPacket.
- Anti-cheat: real XTrap handshake against XTrap4 server DLL.

## Personas

- Server operator (runs the WinServices, edits `ServerInfo.txt`).
- Game designer (edits provisional constants in Battle/Skill/Drop/Item upgrade).
- Reverse-engineer (uses `function_names/*.csv` + `manifests/*.csv` to deepen pinning).
