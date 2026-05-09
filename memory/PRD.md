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

## Pass 1.8 — Real config-file format (2026-05-09)

User supplied real `ServerInfo.txt`, `LoginServerInfo.txt`, `ClientVersionKeyInfo.txt`,
`ServerGroup.txt`, `SubAbStateClass.txt`. Replaced placeholder `key=value` parser.

- `Server/Shared/ConfigParser.{h,cpp}` — `#DEFINE`/`#ENDDEFINE`/`#include`/`#END`/`;`-comments, typed `<STRING>`/`<INTEGER>` fields, repeating records.
- `Server/Shared/ServerInfo.{h,cpp}` — rewritten to expose `Services()`, `OdbcEntries()`, `FindFirst(kind, zone, bindClass)`, `FindOdbc(name)`.
- `Server/Login/ClientVersionKeyInfo.{h,cpp}` — singleton with exact-match `IsAcceptable()`.
- `Login` and `Account DB` service mains rewired to the typed API; others use back-compat `GetInt` mappings.
- Service kind enum (`SK_AccountDB=0, SK_AccountLogDB=1, SK_CharacterDB=2, SK_GameLogDB=3, SK_Login=4, SK_WorldManager=5, SK_Zone=6`) decoded from the real samples.
- Self-tested the parser logic via Python equivalent against the real file: 4 schema defs, 25 `SERVER_INFO`, 6 `ODBC_INFO` — all field types match.

Total now: **129 source files**. See `docs/PASS1_8_REAL_CONFIG.md`.

## Pass 1.9 — Game-data tree consumers (2026-05-09)

User clarified `Shine.zip` is the runtime data tree (configs + Lua content +
game data), not server source. Used the zip's directory listing to gap-check
the engine's loaders. Verified 16 KQ names + 11 instance names + per-event
file convention (`{Routine,Progress,SubFunc}.lua + Data/*.lua`) + 200+ mob
attack sequence files + AreaBMP skill-shape bitmaps.

- `Server/Zone/ScriptLoader.{h,cpp}` — scans `LuaScript/` and loads each event in canonical order (Data/*.lua → SubFunc.lua → Progress.lua → Routine.lua). `LoadAllKQ` / `LoadAllInstances` use the verified name registries.
- `Server/Zone/MobAttackSequence.{h,cpp}` — `MobAttackSequence/<MobName>.txt` line-based pattern loader, indexed by `MobAttackSequenceBox`.
- `Server/Zone/AreaBMP.{h,cpp}` — Windows BMP reader (BI_RGB 8/24/32 bpp) → `AreaMask` AOE shape grids.

No Lua / `.txt` / BMP content copied into code; only directory layout used.

Total now: **135 source files**. See `docs/PASS1_9_SCRIPT_AND_DATA_LOADERS.md`.

## Pass 1.10 — Shine.zip data format integration (2026-02)

User insisted: "open this and look at it... read the files. Then implement them."
Downloaded and walked the entire `Shine.zip` (18 MB; AbState/AreaBMP/BlockInfo/
LuaScript/MobAttackSequence/MobBehaviorDescript/MobRegen/MobRoam/MobSetting/
NPCItemList/ScenarioBookShelf/Script/View/World/DefaultCharacterData.txt).
Wrote first-class C++ readers for every shipped format that wasn't yet covered:

- **TableScript text parser** (`Server/DataReader/TableScriptFile.{h,cpp}`):
  generic `#Ignore/#Exchange/#delimiter/#Table/#ColumnType/#ColumnName/#Record/
  #recordin/#End` parser. Multi-table per file. Critical fix: when
  `#exchange # \x20` is declared, drops space from the default delimiter set
  so dialog strings stay in a single STRING cell. Validated against
  `World/Field.txt` (153 rows × 50 cols + 3 sub-tables), `World/NPC.txt`
  (530 ShineNPC + 233 LinkTable), `World/ChrCommon.txt` (7 sub-tables),
  `Script/Event.txt` (dialog preserved), `MobAttackSequence/AdlFH_Eglack.txt`,
  `NPCItemList/AdlAertsina.txt`.

- **Typed loaders on top of TableScript**:
  `Server/Zone/FieldTable.{h,cpp}`, `Server/Zone/ShineNPCTable.{h,cpp}`,
  `Server/Zone/MobRegenTable.{h,cpp}`, `Server/Zone/NPCItemListTable.{h,cpp}`,
  `Server/Zone/ScriptStringTable.{h,cpp}`. Rewrote
  `Server/Zone/MobAttackSequence.cpp` on top of TableScriptFile (was a naive
  line-tokeniser).

- **`.ps` DSL parser + runtime** (`Server/DataReader/PsScriptFile.{h,cpp}`):
  tokenizer + AST for the Pascal-flavoured AI / scenario DSL with
  `open[name]/close`, `var`, `if/then/else`, `infinite`, `break "name"`,
  `call "name"`, generic verb statements, `%`-string-concat, `@`-builtins,
  full operator vocabulary (`==/===/=!=/!=/<=/>=`). Validated by
  open/close balance and named-block extraction on
  `MobBehaviorDescript/KQ/KingSlime.ps` (28 blocks, 8 named top-level)
  and `ScenarioBookShelf/Promote/JobChange1.ps`.

- **Runtime wrappers**: `Server/Zone/MobBehaviorScript.{h,cpp}` (executes
  whoistarget/whokillme/permillage/getname/chat plus full control flow,
  pass-through hooks for world-altering verbs); `Server/Zone/ScenarioScript.{h,cpp}`
  (block exposure for the scenario host).

- **BlockInfo binary readers** (`Server/DataReader/BlockInfoFile.{h,cpp}`):
  `.shbd` (packed walk grid), `.aid` (area-id name table + payload),
  `.sbi` (sub-block info: name + 5×u32 fields per entry). Layouts derived
  from hex prefixes of `Adl.shbd`, `Adl.aid`, `AdlF.sbi`.

- **AbState binary readers** (`Server/DataReader/AbStateInfoFile.{h,cpp}`):
  `AbStateInfo.dat` (count + N entries of `{u8 id, u8 pad, char[32] en, char[32] localised}`),
  `StaXxx.dat` (record count + auto-detected stride 28/32/24/20/16, rows
  exposed as up to 7 raw u32s).

- **ScriptLoader category extensions**: re-aligned `KnownKQNames` (18 entries)
  and `KnownInstanceNames` (24 entries) to exactly match the directories
  shipped in Shine.zip. Added `LoadTutorial`, `LoadPetSystem`, `LoadAIScript`
  for the three additional `LuaScript/` sub-trees.

Total now: **~157 source files**. See `docs/PASS1_10_SHINE_DATA_PARSERS.md`.

## Pass 1.11 — Real Battle Pipeline + Stat / Item / Soul Systems (2026-02)

User clarified the engine is the world for their AI agent (Elle); combat
must be coherent and tunable without surgery. Also stripped XTrap as dead
code and pinned every confirmed hex opcode from F2/NCProtocol.h.

- **NETCOMMAND.h opcodes pinned**: `NC_USER_LOGIN_REQ=0x0C06`,
  `NC_MAP_LOGIN_ACK=0x1038`, `NC_MAP_INVENTORY_CMD=0x103A`,
  `NC_MAP_EQUIPMENT_CMD=0x103B`, `NC_MAP_SKILLBUFF_CMD=0x10CE`,
  `NC_MAP_QUEST_CMD=0x10D7`, `NC_WM_HANDSHAKE_REQ=0x200F`,
  `NC_MAP_WORLDPARAMS_CMD=0x700D`, `NC_MAP_WORLDTICK_CMD=0x7C07`, etc.
- **XTrap stripped** to a legacy no-op handler that always ACKs success;
  `LS_XTRAP_OK` removed from the login state machine; static token
  preserved as a reference comment only.
- **Editable build constants**: `kClientID`, `kClientBuildToken`,
  `kMapLoginShaToken`, `kMapLoginShaLen`, `kWmHandshakeTokenLen` now live
  as named constants in `Server/Login/ClientVersionKeyInfo.h`.
- **Single-home tunables**: `Server/Zone/BattleTunables.h` collects every
  formula number (damage floor, level-gap clamp, crit scaler, variance,
  angle multipliers, PvP scaler, per-stat fallbacks, upgrade cap +
  fail-penalty curve, free-stat grants, drop scaler, regen ticks).
- **`BATTLESTAT` + `DAMAGERESULT`** structs (40+ fields) with a 12-stage
  `RuleOfEngagement::CalcDamage` pipeline: hit roll -> raw -> level-gap ->
  element -> angle -> crit -> block -> dmg-type-resist -> variance ->
  absorb -> PvP scaler -> floor.
- **`BuildBattleStat` composer** layering RAWCHARSTAT (MoverMain.shn
  base) + EQUIPSUMMARY (sum of equipped ItemInfo + upgrade absolutes) +
  BUFFMODIFIERS (AbState).
- **`MoverMain` / `MoverAbility` / `MobResist` / `LevelGap` SHN loaders**
  (each falls back to neutral defaults if the SHN is missing).
- **`EquipSummaryBuilder`** sums equipped ItemInfo rows including
  upgrade-level absolutes derived from `BasicUpInx` + `AddUpInx` *
  `(uiEnchant - 1)`.
- **`ItemUpgrade::Try`** -- real +N enhancement with
  UpSucRatio/UpLuckRatio rolls, success/fail/downgrade/destroy outcomes
  via the editable `kFailPenaltyAtLevel[20]` curve, capped by
  `kMaxUpgradeLevel`.
- **`FreeStatSystem`** -- STR/END/DEX/INT/MEN allocation with per-level
  `MoverMain.MaxStat` cap, `kMaxFreeStatPerStat` clamp, and a refund
  helper for stat-reset tickets.
- **`SoulStoneSystem`** (final, Pass 1.11.2) -- HP/SP soul stones modeled
  exactly as the project owner described: **two flat counters** on the
  character row (`uiHpCount`, `uiSpCount`), **two independent cooldown
  clocks**, **tier auto-derived from level** (player never sees tier).
  No inventory cost. Use disabled while dead. Single-edit tunables in
  `SoulStoneSystem.h`: `kHpSoulHealByTier[6]`, `kSpSoulHealByTier[6]`,
  `kSoulStoneTierByLevel[200]`, `kSoulStoneCooldownMs`,
  `kSoulStoneMaxCount`, `kSoulStonePriceByTier[6]`.
- **`SoulStoneVendor`** (Pass 1.11.2) -- vendor flow for NPCs flagged
  `Role=Merchant RoleArg0=SoulStone` in `World/NPC.txt`. Two SKUs (HP /
  SP), price = `PriceForLevel(buyer.level)`. Validates NPC role, money,
  and counter cap; debits Vis; calls `SoulStoneSystem::Grant`.
- **`DeathReviveSystem`** (Pass 1.11.1, separate concern) -- on-death
  dialog flow: `ShineSoulPouch` revive token + town respawn + queued
  resurrect.

Total now: **~177 source files**. See `docs/PASS1_11_BATTLE_PIPELINE.md`.

## Pass 1.12 -- World/ Folder Done Right (2026-02)

After re-reading every file in `World/` properly, the soul-stone tier /
heal / cap / price are NOT made-up constants -- they're per-(class, level)
columns in `Param<Class>Server.txt`. Same for the full Power Stone /
Guard Stone ladders. The real NPC role roster is 10 types, not 3, and
`ChrCommon.txt` ships 5 free-stat tables (181 rows each) defining what
each STR/INT/DEX/CON/MEN point grants.

- **`ClassParamTable`** -- loads all 22 `Param<Class>Server.txt` files.
  Per-(class, level) row exposes the full 35-column shape: SoulHP /
  MAXSoulHP / PriceHPStone, SoulSP / MAXSoulSP / PriceSPStone,
  Max/Num/Price/WC/MA `PwrStone`, Max/Num/Price/AC/MR `GrdStone`,
  PainRes/RestraintRes/CurseRes/ShockRes, MaxHP/MaxSP, etc. Plus a
  class-name <-> enum mapping for all 27 classes.
- **`SoulStoneSystem` rewritten on real data** -- heal / cap / price now
  sourced from `ClassParamTable`. No fake tier table left.
- **`PowerGuardStoneSystem`** -- new. Same shape as soul stones; Power
  Stone bumps WC+MA, Guard Stone bumps AC+MR, both for `kStoneBuffSec`
  (30 min default) per stone consumed. Independent HP / SP / Pwr / Grd
  cooldowns. `PowerGuardStoneVendor` matches `RoleArg0` in {`PowerStone`,
  `GuardStone`, `Stone`}.
- **`NPCRole`** -- the real 10-role roster (`ClientMenu`, `NPCMenu`,
  `QuestNpc`, `StoreManager`, `Merchant`, `Gate`, `IDGate`, `ModeIDGate`,
  `RandomGate`, `Guard`) plus 14 merchant sub-kinds (`Weapon`, `Item`,
  `Skill`, `Guild`, `SoulStone`, `PowerStone`, `GuardStone`, `Stone`,
  `Mount`, `Pet`, `Costume`, `Title`, `WeaponTitle`, `ItemCraft`).
- **`ChrCommonTable`** -- loads `Common` + `StatTable` + 5 `FreeStat*Table`s
  with named-column accessors: `WCPerStr`, `MAPerInt`, `SPPerInt`,
  `HitPerDex`, `BlockPerDex`, `ACPerCon`, `HPPerCon`, `MRPerMen`.
- **`WorldTables`** -- single TU with typed loaders for `QuestTable`
  (6 sub-tables joined on QuestHandle), `ExpRecalcTable`,
  `RecallCoordTable`, `DamageByAngleTable` (with linear interpolation),
  `DamageBySoulTable`, `ItemUseFunctionTable`, `RandomOptionTable`,
  `ItemDropGroupTable`, `PineScriptTable`, `SubLayerInteractTable`
  (visibility + attackability matrices), `NPCActionTable`
  (NPCCondition rules).

Total now: **190 source files**. See `docs/PASS1_12_WORLD_FOLDER.md`.

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
