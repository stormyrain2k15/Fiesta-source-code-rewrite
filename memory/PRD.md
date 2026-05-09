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

## Pass 1.5 — Documentation integration (2026-05-09)

User provided `Documentation-main` + `fiesta_documentation_integration_pack`. Generated
5 new files from structural facts (names, types, ordinals, admin levels):

- `Server/DataReader/Schemas.{h,cpp}` — 20 documented `*Row` structs + Tab classes
  (ItemInfo 91 fields, ActiveSkill 114, GuildTournament 73, MobInfo 15, AbState 18, etc.).
- `Lua/LuaAPI_Generated.cpp` — 272 documented Lua C bindings, all callable stubs.
- `Lua/LuaEnums_Generated.cpp` — 11 Lua enum tables as globals (ObjectType, KQ_TEAM, etc.).
- `Server/Zone/AmpersandCommands_Generated.{h,cpp}` — dispatch table for 154 GM commands
  with admin-level gates.
- `Server/DataServer/Common/DBSchemaConstants.h` — 56 documented DB column-name accessors.

See `docs/PASS1_5_DOC_INTEGRATION.md` for the full inventory and wiring notes.

Total now: **119 source files**.

## Pass 1.6 — Real SHN binary reader (2026-05-09)

User provided `SHN_Editor_4.7` source. Wrote a real C++ SHN binary reader:

- `Server/DataReader/ShnFile.{h,cpp}` — full decrypt + parse: 32-byte crypt header,
  4-byte total-length prefix, symmetric XOR-rolling decrypt, 16 documented type codes
  (1/2/3/5/9/11/12/13/16/18/20/21/22/24/26/27).
- `DataReader::LoadAsRows` now tries `.shn` first via `ShnFile`, falls back to `.txt`.
- `Schemas.cpp` row decoders consume parsed `.shn` rows unchanged.

Total now: **122 source files**. See `docs/PASS1_6_SHN_BINARY.md` for details.

## Pass 1.7 — `.dat` reader + SHN→DAT→NIF link (2026-05-09)

User clarified `.dat` files bridge SHN rows to NIF animation clips. Empirically
derived format from supplied samples (plain LE binary, not encrypted, 352-byte
entries for Action/, 104-byte for AbState/).

- `Server/DataReader/DatFile.{h,cpp}` — loader + `ActionDatBox` registry.
- `Client/Engine/AnimationLink.{h,cpp}` — `ResolveClip(actionDat, actionId)` →
  clip name → Gamebryo `NiControllerManager::ActivateSequence` (wiring deferred
  until Client VS project picks up Gamebryo include paths).

Declined to crawl external leaked-source repository per pass-1 IP boundary;
continued with user-supplied artifacts only.

Total now: **126 source files**. See `docs/PASS1_7_DAT_AND_NIF_LINK.md`.

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
