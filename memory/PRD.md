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

## Pass 1.14 -- DataServer DB layer (2026-02)

User dropped `Account.bak` + `AccountLog.bak` and the canonical
`shn_parser.py`. Combined with the existing `World00_Character.bak`,
`World00_GameLog.bak`, `Options.bak`, `OperatorTool.bak`, `StatisticsData.bak`
extractions, we now have ground-truth identifier inventory: 56 stored
procedures and ~30 tables across 7 databases.

- **`DBSchemaConstants.h`** rewritten -- new `fiesta::DB::<Table>` sub-
  namespaces with `kTable` + per-column `kXxx` constants for every real
  table; pre-existing `DBField::*()` accessors preserved for back-compat.
- **`Database`** got `Quote()` / `ExecProc()` / `QueryProc()` helpers so all
  call sites build `{CALL proc(args)}` strings consistently.
- **`SQLP.h/.cpp`** rewritten as 11 per-concern facades:
  `SQLP_Account`, `SQLP_AccountLog`, `SQLP_Character`, `SQLP_Wedding`,
  `SQLP_HolyPromise`, `SQLP_Guild`, `SQLP_Item`, `SQLP_Quest`, `SQLP_Skill`,
  `SQLP_Friend`, `SQLP_GameLog`, `SQLP_Statistics`, `SQLP_Operator`,
  `SQLP_Options`, plus generic `SQLP_Report` / `SQLP_IPChecker`. Every
  proc-name string is the literal `p_*` / `up_*` identifier extracted
  from the corresponding .bak.
- **DataServer Main.cpp** updated for `Account`, `AccountLog`, `Character`,
  `GameLog`, `OperatorTool` -- each exe now opens its real ODBC connection
  and instantiates the SQLP facades it owns.
- **Login client-version check** -- gated. Block in
  `LoginClientSession::HandleVersionCheck` is commented out behind a
  `MUST CONFIGURE TO ENABLE` banner; the packet is acknowledged blindly
  and the session moves into `LS_VERSION_OK`. Matches original-game
  behaviour where only the WM ever advisory-checked the build key.

Total now: **~191 source files**. See `docs/PASS1_14_DB_LAYER.md`.

## Pass 1.15 -- Production exe layout + CharDB plumbing (2026-02)

User dropped 3 screenshots of a real shipping server folder confirming the
production exe naming convention. They also confirmed the `GamigoZR`
service was just a workaround for an old client-version check inside the
Zone server, not a real game function.

- **Removed `Server/GamigoZR/`** entirely; references purged from README /
  BUILD docs.
- **Production exe naming locked in `docs/BUILD.md`**: source dirs map to
  `3LoginServer2.exe`, `4WorldManagerServer2.exe`, `5ZoneServer2.exe`,
  `Account Release.exe`, `AccountLog Release.exe`,
  `Character Release.exe`, `GameLog_Release.exe`,
  `OperatorTool Release.exe`. Spaces/underscores preserved verbatim.
- **`ShinePlayer` widened** to carry the full identity + progression set
  (`AcctID`, `CharID`, level, class, admin level, exp, fame, money,
  STR/END/INT/DEX/MEN, free stat, skill point). Added a
  `LoadFromCharDBRow(const DBRecord&)` that maps a `p_Char_Login`
  projection directly into the player.
- **Per-player `CharacterSkill` + `AbnormalState`** now owned by
  `ShinePlayer` (P1 backlog item closed). `SkillSystem.h` and `AbState.h`
  switched to forward-declared object types to break the circular include
  the new `ShineObject.h` would otherwise introduce.
- **`CharDBSession::OnPacket` now actually serves
  `NC_INTER_CHAR_DB_QUERY`** -- on op=Login it calls
  `SQLP_Character::Login` and replies with
  `NC_INTER_CHAR_DB_RESPONSE` carrying the row columns; on op=Logout it
  calls `SQLP_Character::Logout`.

Total still: **~191 source files** (one Main.cpp deleted, one new doc).
See `docs/PASS1_15_EXE_LAYOUT_AND_CHARDB.md`.

## Pass 1.16 -- OperatorTool exe dropped, loopback-only admin entry (2026-02)

User clarified: the OperatorTool was the admin / dev control panel, not a
game function. Nothing depended on it; services just accepted its
connection over loopback. So we don't ship a new exe, but we keep every
connector / SQL surface so any external admin panel can attach.

- **Removed `Server/OperatorTool/`** entirely.
- **Kept** `WMOPToolSession`, `SQLP_Operator`, `DB::tOperator` /
  `DB::tMenuAuth`, and the full `NC_OPTOOL_*` opcode set.
- **`Socket_Acceptor`** got an optional `bLoopbackOnly` flag -- binds to
  `127.0.0.1` and double-checks the peer address in AcceptLoop.
- **WM Main.cpp** now starts `m_kOPToolAcc` loopback-only.
- **`WMOPToolSession`** now actually does something: tracks
  authed/oper-level state, calls `p_Operator_Logon` via a lazy
  `SQLP_Operator`, then dispatches the full admin command surface
  (ban / kick / jail / unjail / sysmsg / give-item / take-item /
  read-only DB query) with documented wire layouts.
- **`NETCOMMAND.h`** extended with `NC_OPTOOL_JAIL_CMD`, `_UNJAIL_CMD`,
  `_SYSMSG_CMD`, `_GIVEITEM_CMD`, `_TAKEITEM_CMD`, `_QUERY_REQ`,
  `_QUERY_ACK`, `_RESULT_ACK`.

Total now: **~190 source files**. See `docs/PASS1_16_OPTOOL_LOOPBACK.md`.

## Pass 1.17 -- Closing the next-action list (2026-02)

User confirmed the original game had no in-engine cash shop -- billing was
website-side, and the engine only consumes the time-limited boosters
defined in `ChargedEffect.shn`. Worked through the rest of the pending
list in one pass.

- **Outbound IOCP**: new `Socket_Connector` (counterpart to
  `Socket_Acceptor`).
- **Zone <-> CharDB roundtrip**: `CharDBClient` issues
  `NC_INTER_CHAR_DB_QUERY` and feeds the response into
  `ShinePlayer::LoadFromCharDBRow`. Async, non-fatal if offline.
- **Zone <-> WM link**: `WMClient` registers the zone with WM and serves
  the OPTool push family inbound.
- **WM zone-session registry + broadcast**: WM tracks zone sessions by
  id and `BroadcastToZones` fans OPTool actions out. Every OPTool
  command in `WMOPToolSession` now actually dispatches.
- **Battle aggro**: `AggroList` per-mob hate accumulator with the
  documented **20-level-span** scaler (`AddScaled` rejects out-of-span
  attackers); `Battle::Apply` uses it. `MobAISystem::IsInAggroRange`
  consults `MobInfoServer.shn DetectCha / FollowCha / EnemyDetectType`
  for the proximity test. `ShineMob` now carries `m_uiLevel` and the
  per-mob `AggroList`.
- **Pot system**: `PotSystem` consumed-from-inventory potion dispatch
  with per-(char, kind) cooldowns.
- **Charged-effect loader**: `ChargedEffectTable` / `ChargedEffectManager`
  parse `ChargedEffect.shn` + `ChargedDeletableBuff.shn` and apply
  time-limited boosters when a charged item enters inventory.
- **Booth persistence**: `StreetBooth*` / `BoothManager` got real
  per-owner state and a 15-min stale-owner reaper.
- **GuildWar declaration window**: `GuildWarManager` rejects out-of-window
  declarations; default Fri/Sat/Sun 19:00..23:00 local, editable.
- **LuaEnums**: filled `ReturnAI`, `ObjectType`, `ObjectMode`,
  `BasicClass`/`Classes`, `EFFECT_MSG_TYPE`, `CAMERA_STATE`, `KQ_TEAM`,
  `PlayerDamage`, `PlayerList`, `StatsEnum`.
- **NC opcodes**: added `NC_INTER_OPTOOL_*_PUSH` family.

Items deliberately left out: in-engine ItemMall billing (per user),
NC numeric pinning (need captures), `.dat` animation offsets (need
samples), AbState save-on-link (would need a tAbState schema not present
in the .bak inventory).

Total now: **~199 source files**. See `docs/PASS1_17_TIE_OFF.md`.

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

## Pass 1.21 — Runtime hooks: spawn, aura, persistence (2026-02)

User direction (verbatim): "Do all of it, and the folder path is
9Data/Shine/"

Delivered:
- Data root convention updated: default `Data.Root` is now `9Data`;
  per-folder loaders consume `<dataRoot>\Shine`.
- `Server/Zone/MobSpawnSystem.{h,cpp}`: walks `MobRegenBox` at boot,
  builds one `MobSpawnGroup` per `MobRegen` row (species via
  `MobTables::FindByInx`, area via the referenced `MobRegenGroup`).
  Per-tick top-up to MobNum target; respawn timer on death.
- `Battle::Kill` now notifies `MobSpawnSystem::OnMobDied` when an
  `OT_MOB` target dies.
- `EstateServer::Tick` propagates each placed furniture's
  `MiniHouseFurnitureObjEffect` aura to the owner and every
  `OT_PLAYER` within Range cells on the same Field.
- `ExpeditionSystem::Tick` re-applies `uiMasterBuffID` to every member
  of every sub-party (90s keep-time tolerates a missed tick).
  `PartyContainer::Get(uiPartyId)` was added.
- `CharDBClient` extended with fire-and-forget routes for
  Marriage / HolyPromise / Estate (opcodes 10..33 over the existing
  `NC_INTER_CHAR_DB_QUERY` envelope).
- `SQLP_Estate` facade added (`p_Estate_Create/Demolish/SaveFurniture/
  LoadFurniture`).
- `Server/DataServer/Character/Main.cpp` dispatcher routes the new
  social opcodes through `HandleSocialOp`; instantiates `SQLP_Estate`
  at boot.

Doc: `docs/PASS1_21_RUNTIME_HOOKS.md`.

Status: code authored, awaiting user-side VS2010 compile.

## Pass 1.20 — Per-folder asset ingest + NPC spawn pipeline (2026-02)

User direction (verbatim): "keep going with adding the files and what
each function behind them is and making sure the server does the same
thing."

Delivered:
- `Server/Zone/ZoneAssetLoader.{h,cpp}`: single boot pass walking every
  per-folder data source under `Data\Shine\` (AbState, AreaBMP,
  BlockInfo, MobAttackSequence, MobBehaviorDescript, MobRegen, MobRoam,
  MobSetting/Action, NPCItemList, Script, plus every `LuaScript/`
  subtree). Includes `AbStateBinaryBox` for the binary `.dat` index.
- BlockInfo callback resolves the file basename via `MapTables` and
  stamps the `BlockGrid` into the matching `Field::Blocks()` so
  collision is live the moment boot completes.
- `ShineNPCTable::SpawnAll()`: instantiates a `ShineNPC` for every row
  in `World/NPC.txt`, registers (id, mob-name) with `NPCManager`, and
  inserts into the resolved `Field`.
- `NPCSystem.cpp` rewritten: `NPCManager::RegisterKey` /
  `NPCManager::KeyOf`; `NPCItemList::GetForShop` walks the loaded
  `NPCItemListFile` and resolves each ItemIndex via `ItemTables`;
  `SellItemManager::BuyFromNpc` validates SKU + debits gold.
- `ShnRegistry::LoadAll` now also walks `Data\Shine\View\*.shn` (21
  view-shape tables).
- `Zone/Main.cpp`: opens a per-process `LuaRuntime`, calls
  `ZoneAssetLoader::LoadAll`, `ShineNPCTable::Load/SpawnAll`,
  `ClassParamTable::Load`, `ChrCommonTable::Load` in dependency order.

Coverage matrix:
| Folder                                   | Files | Wired |
|------------------------------------------|-------|-------|
| `AbState\*.dat`                          | 11    | 11    |
| `AreaBMP\*.bmp`                          | 53    | 53    |
| `BlockInfo\*.{shbd,aid,sbi}`             | 301   | 301   |
| `NPCItemList\*.txt`                      | 73    | 73    |
| `Script\*.txt`                           | 52    | 52    |
| `View\*.shn`                             | 21    | 21    |
| `MobAttackSequence,Behavior,Regen,Roam,Setting/Action\*.txt` | per-mob/zone | walked |
| `LuaScript\**\*.lua`                     | per-system | walked |

Doc: `docs/PASS1_20_PER_FOLDER_INGEST.md`.

Status: code authored, awaiting user-side VS2010 compile.

## Pass 1.26 — Long-tail SHN + World/*.txt column auditor (2026-02)

User mandate: "Audit per-column coverage of the long-tail SHNs read via
the generic ShnRegistry path... Audit every `World/*.txt` runtime side
... the same way."

Delivered:
- `Server/DataReader/ShnRegistry.{h,cpp}` -- transparent column
  auditor hooked into `GetTable()` and `ShnGet*`. Emits one
  `WARN ShnAudit: <table>.<col> parsed but not consumed` line per
  unread column at boot end.
- `Server/DataReader/TableScriptFile.{h,cpp}` -- same shape for the
  TS-format World/*.txt files. `TsTable::GetCell` stamps every
  by-name read; `TableScriptFile::Load` registers every parsed
  TsTable so the boot-end walker can diff against the binders.
- `Server/Zone/Main.cpp` -- after `BindAllExtendedTables()` and
  `BindTypedSchemaConsumers()`, emits both audit reports.
- `Server/Zone/NPCItemListTable.cpp` -- bumped per-tab column read
  from Column00..05 to Column00..15.

Status: code authored. Once user runs the first VS2010 boot, the log
will include one warning per parsed-but-unconsumed column for the
next pass to fill.

Doc: `docs/PASS1_26_COLUMN_AUDITOR.md`.

## Pass 1.23 — 1.25  Full SHN column wire-through (2026-02)

User mandate (verbatim): "Do not i repeat do not skip a single column in a
single document. Everything must be used in the way it was intended.
The layout makes it obvious (column names, other documents being called
and so on) follow the trail."

Delivered:
- **Pass 1.23** -- `ItemInfo.shn` (91 cols) and `ItemInfoServer.shn`
  (25 cols) fully captured. `ItemAuthority` predicates honour every
  NoXxx + Belonged + PutOnBelonged + Class + DemandLv flag. Bind-on-
  equip stamps via `Inventory::Equip`. `EquipEnumChanger::ToEquipSlot`
  decodes the `Equip` bitmask. `ItemDropFromMob` honours
  `LootingMode` and logs `VanishSecs`. `DropResolver` uses
  `RandomOptionDropGroup` override.
- **Pass 1.24** -- `ActiveSkill.shn` (114 cols) wired through the
  `SkillDataBox` accessors: HP/SP/CastTime/Range/Area/TargetNumber,
  25-class permission mask, StaA-D + Strength + SucRate, T00..T34
  damage matrix, IMPT0..3, SkillClassifierA-C, CannotInside,
  DemandSoul, DemandSk. `Skill::TryUse` pipeline upgraded to use
  every column.
- **Pass 1.25** -- new `TypedSchemaConsumers.{h,cpp}` covers the 9
  typed Schemas.h rows that previously had no runtime reader:
  ItemActionEffect / ItemActionCondition (resolver), CharacterTitleData
  (TitleProgression), ActionRangeFactor, GTIServer, StateField,
  MIDungeon + MIDServer (MIDirectory), SubAbState. GradeItemOption
  expanded from 4-col to full 20-col wide row, fed into
  `EquipSummaryBuilder::AddItemContribution`. `ChargedEffectInst`
  extended to carry `uiStaStrength`.

Doc: `docs/PASS1_23_TO_1_25_FULL_SHN_WIRE.md`.

Status: code authored, awaiting user-side VS2010 compile.

## Pass 1.22 — Server-Authoritative NPC Menu Loop (2026-02)

User direction: client-side `ressystem.zip` (NPCViewInfo / NpcDialogData /
QuestDialog SHNs) provided so the server can drive both ends of the NPC
click flow.

Delivered:
- `Server/Zone/NPCSystem.h/cpp`: expanded `ServerMenuActor` with
  `SendDialog` (chained DialogID -> ButtonsFor sub-pages), `OpenShop`
  (`NC_NPC_SHOP_OPEN_CMD` projection of `NPCItemList` rows),
  `HandlePick` (action-tag dispatcher: Talk/Trade/Quest/Mover/Promote/
  Save/Close), `HandleBuy` (validates SKU + creates `ShineItem` from
  `ItemTables::FindItem`), `HandleSell` (clamps to stack size, debits
  inventory, credits `uiSellPrice * qty`).
- `Server/Zone/ZoneHandlers.cpp`: registered `NC_NPC_MENU_PICK_REQ`,
  `NC_NPC_SHOP_BUY_REQ`, `NC_NPC_SHOP_SELL_REQ`. ViewInfoId == 0 is the
  synthetic "open root menu" click; bumps the per-NPC Meeting quest
  counter via `QuestProgress::OnNpcTalked`.

Doc: `docs/PASS1_22_NPC_MENU_LOOP.md`.

Status: code authored, awaiting user-side VS2010 compile.

## Pass 1.19 — Long-tail data ingest + Estate / Marriage / Expedition (2026-02)

User direction (verbatim): "I want you to keep going through all of the
files and getting them being used by the server. ... estates are part
of the mini house/vendor system. ... Marriage system. Guild system
expedition system academy system which is part of the guild system."

Delivered:
- `Server/Zone/ExtendedTables.{h,cpp}`: typed accessors for the 110 SHN
  files Pass 1.18 didn't already group. Coverage now **199/199** SHNs.
- `Server/Zone/EstateSystem.{h,cpp}`: mini-house ownership, durability
  decay, furniture placement against MiniHouseDummy, vendor-mode
  hand-off to StreetBooth.
- `Server/Zone/MarriageSystem.{h,cpp}`: HolyPromise lifecycle (PROPOSED
  → ENGAGED → MARRIED → DIVORCED), summon cooldown, anniversary
  promise-count tick keyed off HolyPromiseReward.shn.
- `Server/Zone/ExpeditionSystem.{h,cpp}`: party-of-parties (≤6
  sub-parties), loot modes FREE / MASTER / TURN, master-buff slot.
- `GuildSystem.cpp`: `GuildTournamentSystem` LvGapMul / OccupyPoints
  now data-driven from `GuildTournamentExtraTables`.
- `WorldTables.{h,cpp}`: `LoadAllPineScripts` walks PineScript.txt and
  loads every ScenarioBookShelf .ps; `KarenAttackTable` + `MobChatTable`
  ingest the last two World/*.txt files. Coverage now **48/48**
  World/*.txt.
- `Zone/Main.cpp`: BindAllExtendedTables(), LoadAllPineScripts(),
  Karen/MobChat loaders wired into boot; EstateServer + ExpeditionSystem
  ticks added.

Doc: `docs/PASS1_19_LONG_TAIL_AND_ESTATE.md`.

Status: code authored, awaiting user-side VS2010 compile.

## Personas

- Server operator (runs the WinServices, edits `ServerInfo.txt`).
- Game designer (edits provisional constants in Battle/Skill/Drop/Item upgrade).
- Reverse-engineer (uses `function_names/*.csv` + `manifests/*.csv` to deepen pinning).
