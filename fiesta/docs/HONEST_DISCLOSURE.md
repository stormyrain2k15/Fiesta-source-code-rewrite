# Honest Disclosure: Stubs, Guesses, and Verify Markers

This document is for the next reviewer (codex audit pass, opus static
debug, sonnet runtime debug, opus function verification). It catalogs
everything I know to be incomplete, invented, or stubbed in the Zone
server tree so you don't have to discover it the hard way.

I'd rather you find this list and triage it than spend cycles
chasing ghosts in a 350-file directory.

---

## Audit pass log

### Pass 7 (Feb 2026, AbState as the central effect engine)

User directive: *"AbState is the unified temporary-effect/state-modifier
system used by skills, monster abilities, items, maps, mounts, and
premium effects. Any system that depends on temporary buffs, debuffs,
stat changes, triggered effects, or timed conditions must route through
AbState."*

**Schema audit.** Decoded `AbState.shn` (777 rows, 19 cols) and
`SubAbState.shn` (2041 rows, 14 cols) directly off the data drop:

* `AbState.shn` columns -- `ID`, `InxName`, `AbStataIndex`, `KeepTimeRatio`,
  `KeepTimePower`, `StateGrade`, `PartyState1..5`, `PartyRange`,
  `PartyEnchantNumber`, `SubAbState`, `DispelIndex`, `SubDispelIndex`,
  `AbStateSaveType`, `MainStateInx`, `Duplicate`.
* `SubAbState.shn` columns -- `ID`, `InxName`, `Strength`, `Type`,
  `SubType`, `KeepTime`, `ActionIndexA..D`, `ActionArgA..D`.

The previous AbState plumbing was hollow: only `ID/InxName/SaveType`
was kept, the priority resolver looked for a non-existent "Priority"
column, and `Apply()` wrote a row with no actions wired.

**Build-out.**

* `AbStateRuntime.{h,cpp}` -- new central dispatcher. Holds a
  `AbStateRuntimeRow` (resolved AbState + SubAbState id, expiry,
  4 action slots, shield absorb capacity). Per-row Tick() runs the
  4 action handlers; OnApply / OnRemove emit one-shot transitions;
  `AbsorbIncoming` / `ReflectIncoming` are gates Battle calls before
  /after HP loss to honor shield + reflect actions.
* `AbState.cpp` -- real `AbnormalState::Apply` / `ApplyAt` /
  `ApplySubByName` / `Remove` / `RemoveBySubInxName` /
  `DispelByCategory` / `Tick` / `Has` / `HasInxName` / `StatModX1k`.
  Replace/stack rule in `SubAbstatePriority::ShouldReplace` now uses
  the real `AbStateRow.Duplicate` and `StateGrade` columns.
* `AbnormalState` ledger moved onto base `ShineObject` so mob targets
  can carry status states too (CC chains, debuff stacks). Players
  alias the inherited ledger via the legacy `pk->AbState()` accessor.
* Action-id constants live in `BattleTunables.h::kAbAction_*` (DotHP_A,
  DotHP_B, DotSP, StatMod{ATK,DEF,MoveSpd,AtkSpd,MATK,MDEF},
  RegenHP, RegenSP, DisableMove, DisableAct, AbsorbShield, ReflectDmg,
  DispelOnHit). Per-tick cadence + DoT scaler tunable in same file.
* `Battle::Apply` (both overloads) now routes incoming damage through
  `AbStateRuntime::AbsorbIncoming` and mirrors a fraction back via
  `ReflectIncoming` for any active reflect rows.
* `ZoneServer::Tick` walks every registered ShineObject and ticks the
  AbState ledger -- DoTs / regens / shield-decay run uniformly across
  players and mobs.
* `SkillSystem::Use` upgraded: skill `StaApply` slots resolve by
  SubAbState InxName + Strength via `AbState().ApplySubByName(...)`.
  Previous code passed a SubAbState id into `Apply()` which expected
  an AbState id -- the buff fired at a 0% rate.
* `StateFieldTable::OnPlayerEnter` upgraded to the same SubAbState
  direct path.
* `TypedSchemaConsumers::ItemActionResolver::EffectApply` -- new
  `case 4` (ApplyAbState) routes Value->SubAbState id, Area->Strength
  through `AbState().ApplySubByName`. Unknown EffectActivity codes
  surface a warning instead of silently no-op'ing.
* `AbnormalStateShelter` -- save / load uses the real ledger and
  joins through `AbStateSaveTypeInfoTable` for the
  link/die/logoff trigger mask.
* `AbnormalStateDictionary` exposes `GetRow(id)` and `GetSaveType(id)`
  driven by the loaded `AbStateTables` row data.
* `AbState.h` / `AbnormalStateShelter.h` are the only canonical
  declarations; the legacy duplicate definitions in older headers
  were removed.

**Other hollow-fix sweep this pass:**

* `Lua_cFinishKey` -- now records the (charId, key) tuple on the
  player's `CharQuest` ledger; `CharQuest::FinishKeyAdd` /
  `FinishKeyHas` / `FinishKeyCount` exposed for the matching
  `cIsFinishKey` binding.
* `ActionTargetTypeValidator` -- real (Self / Single / Ally / Enemy /
  AOE / Cone / Line) target-type gate with explicit faction rules.
  Previous body returned `true` after passing a distance check.
* `BoothManager.{h,cpp}` -- real per-zone live booth registry with
  Open/Close/AddItem/RemoveItem/Buy/Find/ListNearby. The previous
  implementation lived in an anonymous namespace and did nothing.
* `GuildAcademy.{h,cpp}` -- real impl reading `GuildAcademy.shn`,
  applying the academy buff via `AbnormalState::Apply` only after
  the membership-time threshold passes. `GuildServer::GuildOf` /
  `JoinedMs` exposed; `GuildMember` carries `uiJoinedMs`.
* `KQServer::ForceEnd` -- now actually broadcasts `NC_KQ_STATE_CMD`
  to every queued participant. Previous code only mutated state.
* `NpcScheduleServer::Tick` -- now adds/removes the NPC from its
  `Field` on flip events. Previous code only logged.
* `Lua_cDoorAction` -- routes through new `NC_MAP_DOOR_STATE_CMD`
  opcode (FAMILY_KQ + 0x08, PROVISIONAL until a real capture).
* `Lua_cResetAbstate` -- returns the actual remove result instead of
  a "did the player exist" boolean.

### Pass 6.5 (Feb 2026, post-Pass-6 fill-out)

User directive: *"all remaining stubs we know the function but not the
values like the dmg calc we know the function but not what numbers it's
using in the math. I want u to build out the rest of the stubs and use
generic values to at least make the function testable and able to be
fine tuned through value adjustment in the code."*

Every TODO / stub-return-nil / "not yet wired" branch is now built out
against the documented function shape with **generic, named tunables**
held in one place so a value sweep is a single-file edit.

| Area | What was a stub | What it does now | Where to tune |
|------|-----------------|------------------|---------------|
| `RuleOfEngagement::Roe_*` | Return `pkA->nATK + nSkill`, return `pkT->nDEF`, hit/crit floor estimates | Full ATK swing (`nATK + rand[0..nATK*kRoeAtkRangeX1k/1000] + skillBase`), element-resist scaled DEF, hit/crit clamp formulas | `BattleTunables.h::kRoe*` |
| `MobSpawnSystem` script API | `cMobRegen_XY/Rectangle/Circle/Obj/GroupRegenInstance` returned `nil` | Real spawns through `SpawnAt` / `SpawnRectangle` / `SpawnCircle` / `SpawnGroup`, registered in unified ZoneServer object map | Edit count clamps in the bindings |
| `Lua_cExecCheck` | `(void)fn` | Routes script function name to `SHINELOG_DEBUG` (or `SHINELOG_INFO` if level arg > 0) | n/a (debug helper) |
| `Lua_cDoorAction` | `(void)blk; (void)bOpen` | Resolves handle, broadcasts door-state envelope to every player on the same map | Replace `NC_ACT_SCRIPT_MSG_CMD` with the real door opcode when captured |
| `KingdomQuest::End` | Returned `true` without state mutation | Forces `KQServer` into `KQS_END`; the standard tick advances back to `KQS_IDLE` after `kKQEndTimeoutMs` | `KingdomQuest.cpp::kKQ*TimeoutMs` |
| `KQServer::Tick` durations | Magic numbers `60000`, `30 * 60000`, `30000` | Named constants `kKQRecruitTimeoutMs`, `kKQRunningTimeoutMs`, `kKQVoteTimeoutMs`, `kKQEndTimeoutMs`, `kKQRecruitMinPlayers` | `KingdomQuest.cpp` |
| `KQRewardDataBox::GoldFor` | `return c * 10` | `base + perPoint*c`, clamped to maxGold | `BattleTunables.h::kKQReward*` |
| `LiveOpsBoosts::kGMEvent_*` | Inline `1001..1004` placeholders | Lifted to `BattleTunables.h::kGMEvent_*` with VERIFY note | `BattleTunables.h` |
| `AbnormalStateShelter::Save/Load` | `(void)cid; (void)id; (void)ms` | Routes to `CharDBClient::AbStateSet` (proc 90) / `AbStateGetAll` (proc 91) | Edit proc-id constants in `CharDBClient::AbStateSet` |
| `SetItemData::CountEquippedPieces` | Returned 0 always | Walks `pkPlayer->Inv().All()` filtering `bEquipped == 1`, matches `ItemInfoRow.kSetItemIndex == szSetIndex` | n/a (data-driven) |
| `SubAbstatePriority::ShouldReplace` | `return uiNew >= uiOld` | Looks up `SubAbState.shn::Priority` column when present; falls back to numeric-id rule | n/a (data-driven) |
| `SoulStoneSystem::ClassOf` | Returned `SC_FIGHTER` | Reads `ShinePlayer::GetClass()` and clamps to enum range | n/a |
| `TargetAnalyser::IsLegalTarget` | Distance-only | Adds same-self / same-map / mob-vs-mob / dead-target gates | n/a |
| `NpcScheduleServer::Tick` (Zone) | `(void)hourOfWeek` cadence anchor | Walks `NPCManager::NpcKeys()`, matches `NpcScheduleTable::IsActive`, surfaces flip events to log | Polling cadence is caller-controlled |

Tunable surface summary:

* `Server/Zone/BattleTunables.h` -- ALL combat / Roe / KQ-reward / GMEvent-id tunables
* `Server/Zone/KingdomQuest.cpp` -- KQ state-machine timings (file-local constants)
* `Server/Zone/CharDBClient.cpp` -- AbState stored-proc ids (90, 91)

### Pass 6 (Feb 2026, post-Lyra-pass-5 cleanup)

Lyra Pass 5 introduced LUA-01..LUA-20 binding bodies and several WIRE-*
lifts. Most worked, but the lifted bindings referenced symbols that
didn't yet exist; this pass closed those gaps and finished the P0
functional items from `HONEST_DISCLOSURE.md` §5.

| # | ID | Issue | Resolution |
|---|----|-------|------------|
| 1 | PASS6-001 | `pk->Abstate()` typo across LUA-11, LUA-19 -> compile error (real method is `AbState()`) | Fixed both call sites |
| 2 | PASS6-002 | LUA-07 / LUA-08 / LUA-17 referenced `MapField::NameToID(...)` which never existed | Added `ResolveMapByName` free fn in `MapField.{h,cpp}` that routes through `MapTables::FindByName` |
| 3 | PASS6-003 | LUA-10 cScriptMessage_Obj called `pk->GetSession()->Send(pkt)` (no such method); also wrote opcode into the body | Routed through `SendPacket(session, NC_ACT_SCRIPT_MSG_CMD, body, size)` |
| 4 | PASS6-004 | LUA-16 cRandomInt / cPermileRate used `Well512::Get().NextU32()` -- class is `well512` (lowercase) and has no `Get()` singleton | Added `ScriptRng()` static helper in LuaCBindings that lazy-seeds a process-local well512 from GTimer |
| 5 | PASS6-005 | LUA-17 cQuestResult / LUA-18 cEndOfKingdomQuest called `NC_KQ_END_CMD` and `KingdomQuest::Get().End()` -- neither existed | Added `NC_KQ_END_CMD` (NC_FAMILY_KQ + 0x06) and `NC_ACT_SCRIPT_MSG_CMD` (NC_FAMILY_KQ + 0x07) opcodes; added `KingdomQuest::End(const char*)` static API |
| 6 | PASS6-006 | PASS3-005 deferred: unified `Handle -> ShineObject` registry (mobs/npcs/players/pets) | `ZoneServer` now owns `m_kObjects` keyed by handle. `RegisterObject` / `UnregisterObject` / `FindObject` / `FindMob` / `FindNPC` / `FindPet` exposed. `AttachPlayer` mirrors automatically; `MobSpawnSystem::Tick` and `ShineNPCTable::SpawnAll` call `RegisterObject` on every spawn. Lua bindings cDamaged / cObjectLocate / cObjectHP / cIsObjectDead / cNPCVanish now resolve through `FindObject` so they work for any object type, not just players. `cObjectCount` walks the object snapshot and matches against the documented ObjectType enum (Player=2, Mob=5, Npc=6, Pet=12) |
| 7 | PASS6-007 | §5 ItemUpgrade::Try ignored UpResource (free upgrades) | Now resolves a same-bucket consumable item (UpLimit==0 row whose UpResource matches the target's UpResource) and removes one stack regardless of outcome. Missing-resource path returns `UPGRADE_BLOCKED` and never deducts. |
| 8 | PASS6-008 | §5 ItemUpgrade::Try ignored bUseLuckStone | Now resolves a LuckStone via `ItemInfo.ItemUseSkill == "LuckStone"` (only stable string anchor without per-server config), consumes it on attempt, and adds `UpLuckRatio` to the per-mille rate. Falls back to no-bonus + warning if the player has none. |
| 9 | PASS6-009 | §5 WMClient::OnTakeItem only logged | Body shape upgraded to `{ uint32 charNo, uint64 itemKey }`; legacy 8-byte form still accepted via `Remaining()` length-check. The matching ShineItem is removed from the live `Inventory` so the client view reconciles immediately. |
| 10 | PASS6-010 | §5 NC_INTER_BROADCAST_CMD kind=1 (daily reset) and kind=4 (NPC schedule) dropped on the floor by Zone | Unified the kind-byte protocol across every WM sender: 0=ChatShout, 1=WorldYell, 2=GMEvent, 3=DailyReset, 4=NpcSchedule. WM senders DailyQuestTimer / NpcScheduleServer now prepend the kind byte. Zone WMClient::OnInterBroadcast dispatches all five via the new `DailyResetSink` and `NpcScheduleSink` (process-wide registries; subsystems register a function ptr in Zone::Main). Chat broadcasts now re-emit `NC_CHAT_SHOUT_CMD` / `NC_CHAT_NORMAL_CMD` to local clients. |
| 11 | PASS6-011 | `PacketBuffer` had no `Remaining()` -- needed for length-tolerant body parses (e.g. WMClient OnTakeItem dual shape) | Added `Remaining()` const helper |

### Lyra task list pass 4 (May 2026)

Heavy task list arrived from the user. Of ~50 tasks (FIX-01..03 + WIRE-01..18 + LUA-01..20 + DOC-01..05), I tackled the highest-impact subset:

| # | Task ID | Issue | Resolution |
|---|---------|-------|------------|
| 1 | FIX-01 | `Lua/LuaEnums.cpp` had invented values for ObjectType / ObjectMode / Classes -> every script comparing against them got wrong results | Replaced with PDB-confirmed values: ObjectType (Invalid=-1, Flag=0, ..., Mob=5, ..., Pet=12, Max=13), Classes (None=0, Fighter=1, Cleric=6, Archer=11, Mage=16, Joker=21, Sentinel=26 -- step-of-5 IDs). Added `InstallEnumStrTable` so ObjectMode is now installed as STRING-valued (`linking`/`normal`/`fight`/`corpse`/`house`/`booth`/`riding`/`logoutwait`) since scripts compare against strings, not ints |
| 2 | FIX-02 | 4 SHN table-name mismatches: MoverData->MoverMain, MoverUpgrade->MoverUpgradeEffect, KQReward->KingdomQuestRew, MultiHit->MultiHitType | Updated all 4 .cpp files with correct ShnRegistry table names. MoverDataBox / MoverUpgradeData / KQRewardDataBox now load real data instead of always failing |
| 3 | FIX-03 | `LoginClientSession::HandleWorldSelect` hardcoded WM endpoint `127.0.0.1:28000` | Added `ServerInfo::GetCurrent()` / `SetCurrent()` process-wide getter (set by Login service main on boot, cleared on stop). `HandleWorldSelect` now resolves WM via `ServerInfo::FindFirst(SK_WorldManager, -1, -1)`. WM-missing case logs WARN and falls back to provisional values (visible in logs rather than silent) |
| 4 | WIRE-01 | `MultiHitTable.cpp` was a 7-line anonymous-namespace stub that always returned 1 hit | New `MultiHitTable.h` declaring the full surface. `.cpp` now loads `MultiHitType.shn`, indexes by `HitGroupID`, exposes `GetGroup(id, *count)` for the Battle.cpp hit-loop. `Resolve(SkillID)` keeps the single-hit default until the SkillID->HitGroupID wiring (ActiveSkill.shn nHitID) lands |
| 5 | WIRE-06 | `AttackRhythm.cpp` was an anonymous-namespace stub | New `AttackRhythm.h` + real `.cpp` with per-(cid, sid) cadence gate keyed by `GTimer::NowMillis()`. `Allow(cid, sid, cdMs)` returns true and advances; `Forget(cid)` drops a player's history on disconnect |
| 6 | WIRE-07 | `AbnormalStateDictionary.cpp` was an anonymous-namespace stub | New `AbnormalStateDictionary.h` + real `.cpp`. Loads `AbState.shn` at boot, builds `name -> id` map. `Lookup("Stun")` returns the abstate id |
| 7 | WIRE-08 | `NearScan` was declared in two places (MoveManager.h class + an anonymous-namespace stub in NearScan.cpp) -> ODR collision | Single canonical `NearScan` class in new `NearScan.h`. Added `Players(uiMap, cx, cy, r, &out)` for AOE / KQ scans + `RunOnField(field, from, &out)` for the legacy MoveManager call site (visibility radius 30). MoveManager.h now includes NearScan.h instead of redeclaring |
| 8 | WIRE-09 | `PartyContainer.cpp` was an anonymous-namespace 4-field struct | New `PartyContainer.h` with full `MAX_SLOTS=8` array, leader index, ePartyLootRule enum, level-range fields. `.cpp` provides `AddMember/RemoveMember/HasMember/LeaderCid/SetLeader` with auto-leader reseat on leader removal |

### Deferred (longer / blocked / lower priority)

* WIRE-02..05 (other anonymous-namespace stubs) -- pattern is now established (header + lift); apply the same pattern as bugs surface
* WIRE-10..18 (further data-box wireups) -- mostly follow-on to FIX-02
* LUA-01..20 (real bodies for the 20 most-used Lua bindings) -- the 5 critical bindings (cDamaged / cStaticDamage / cSetAbstate / cLinkTo / cFinishKey) are done; the remaining 15 need ShineObject getter helpers that opus's static round will bring in
* DOC-01..05 (architecture docs) -- HONEST_DISCLOSURE.md already covers most of this
* PASS3-005 (unified Handle->ShineObject registry) -- still deferred to opus round

### codex pass 3 (May 2026)

| # | Audit ID | Issue | Resolution |
|---|----------|-------|------------|
| 1 | PASS3-001 | `Server/Zone/tests/*.cpp` had its own `main()` AND was being pulled into `Zone.vcxproj` -> duplicate `main` | Reworked `Build/gen_vcxproj.py` with `EXCLUDES`/`EXTRA_DEFINES` tables. Zone now drops the `tests/` subtree (321→312 cpp); new `ZoneTests.vcxproj` owns the 9 tests + defines `FIESTA_TEST_MAIN`. Re-running `gen_vcxproj.py` keeps both projects in sync |
| 2 | PASS3-002 | `Client.vcxproj` was in the active build but is out-of-scope for the server-only rounds | Removed `Client` from `PROJECTS[]`. Source under `fiesta/Client/` stays so it doesn't drift. Re-enable instructions are in a comment in `gen_vcxproj.py` |
| 3 | PASS3-003 | 4 calls to `GetTickCount64()` (Vista+ API, banned by VS2010-on-XP target) | Replaced with `GTimer::NowMillis()` in `MarketSystems.cpp`, `QuestRuntime.cpp`, `SoulStoneSystem.cpp`, `PowerGuardStoneSystem.cpp`, `Main.cpp`. Added missing `#include "../Shared/GTimer.h"` where needed |
| 4 | PASS3-004 | `DataServer/Character/Main.cpp` used raw `vsnprintf` without VS2010 guard | Added `_MSC_VER` guard around `_vsnprintf_s` (matching the pattern in `SQLP.cpp`). Added missing `#include <stdarg.h>` |
| 5 | PASS3-006 | `Skill::TryUse` had `static CharacterSkill s_kDummy` -> one player's cast locked the spell for everyone | Replaced with `pk->Skills()` (per-character `CharacterSkill` book). Reordered: cooldown gate now runs BEFORE SP/HP deduction so a still-cooling skill doesn't drain resources from a failed attempt |
| 6 | PASS3-007 | `NC_CHAR_EVENT_ATTENDANCE_CHECK_CMD` was an alias to a non-existent base. Pass 2 added the alias label but no numeric base | Added 4 numeric opcodes under `NC_FAMILY_CHAR + 0x12-0x15` with PDB-confirmed names (CHECK_CMD, CHANGE_DAY_CMD, REWARD_DB_REQ, REWARD_DB_ACK). The alias label now points at a real value |
| 7 | PASS3-008 | `QuestProgress` was both a `struct` (per-player counters in `QuestRuntime.h`) and a `class` (global event dispatcher in `QuestSystem.h`) -> ODR collision | Renamed the struct to `PlayerQuestProgress` (per-player progress data) and the class to `QuestEventDispatcher` (global event hub). Updated 3 call sites in `Battle.cpp` and `NPCSystem.cpp` |
| 8 | PASS3-009 | Battle pipeline lacked the original NA2016 PDB function surface (Roe_*, normalpyRoe_*, normalmaRoe_*) so PineScript bindings have nothing to target | Added free-function namespaces in `Battle.h` declaring `Roe::Roe_calcdamage / getattack / defendpower / hitrate / criticalrate / isdamageincrease`, `normalpyRoe::*`, `normalmaRoe::*`. Implementations in `RuleOfEngagement.cpp` are thin wrappers around the existing pipeline (math stays VERIFY-tagged in `Battle.cpp`; surface is the deliverable) |
| 9 | PASS3-010 | The 3 invented packet bodies were tagged `// VERIFY:` but not greppable as a class | Renamed to `// PROVISIONAL_BODY:` for `NC_NPC_MENU_PICK_ACK`, `NC_ITEM_UPGRADE_OPEN_CMD`, `NC_INTER_GMEVENT_TRIGGER_REQ`. Codex / opus can now `grep -r PROVISIONAL_BODY` to find every untrusted wire shape |

### Deferred to opus / sonnet rounds

* PASS3-005 (unified `Handle -> ShineObject` registry across mobs/npcs/players) -- larger touch, deferred per audit's own guidance
* PASS3-011 (`QuestShnReader` real row parsing) -- requires QuestData.shn binary RE first
* PASS1-FUNC-004 (real Lua bindings beyond the 5 implemented)
* PASS1-FUNC-005 (Login WM endpoint from config -- still hardcoded `127.0.0.1:28000`)

### codex pass 2 (May 2026)

| # | Audit ID | Issue | Resolution |
|---|----------|-------|------------|
| 1 | PASS2-001 | `QuestShnReader.cpp/.h` and `ShnCsvExporter.cpp` not in any vcxproj -> Zone link fail | Re-ran `Build/gen_vcxproj.py`. DataReader.vcxproj now compiles 14 cpp / 15 h (was 12 / 14) |
| 2 | PASS2-004 | `QuestShnReader.cpp` used `::tolower` without `<ctype.h>` | Added `#include <ctype.h>` |
| 3 | PASS2-003 | `NC_CHAR_EVENT_ATTENDANCE_CHECK_CMD` referenced in client UI table but undefined | Added 4 PDB-confirmed event-attendance opcodes to `NETCOMMAND.h` (CHECK_CMD, CHANGE_DAY_CMD, REWARD_DB_REQ, REWARD_DB_ACK) under NC_FAMILY_CHAR + 0x12-0x15 |
| 4 | (audit-tool improvement) | `ShnAudit_EmitReport` only audited tables already touched via GetTable -> couldn't catch a loaded SHN that no system opened | Added registry-level "unowned table" audit pass. Walks the full registry and emits `ShnAudit: table 'X' loaded but no system owns it` WARN for any non-quest, non-deferred table that never appeared in `s_kAuditReads`. Boot now logs both `ShnColumnAuditor` and `ShnRegistryAuditor` summary lines |

### Deferred to opus static debug / sonnet runtime debug rounds

Per the audit's own next-pass guidance, these stay open for the
deeper-pass rounds:

* PASS2-002 (`QuestShnReader` only slurps bytes) -- needs real format RE first
* PASS1-FUNC-001 (player-only handle lookup, breaks PvE combat)
* PASS1-FUNC-002 (`static CharacterSkill s_kDummy` -> per-character state)
* PASS1-FUNC-003 (Roe_* / normalpyRoe / normalmaRoe original function surface)
* PASS1-FUNC-004 (Lua API surface mostly stubs)
* PASS1-FUNC-005 (Login WM endpoint hardcoded `127.0.0.1:28000`)

### codex pass 1 (May 2026)

Hard compile/link blockers reported and resolved in the same response:

| # | Issue | Resolution |
|---|-------|------------|
| 1 | `AIScript.cpp:5` had `extern "C" { #include ... }` on a single line (invalid preprocessing) | Split onto separate lines |
| 2 | `RegisterZoneLuaAPI(lua_State*)` referenced but undefined | Defined in `Lua/LuaCBindings.cpp` -- routes through `RegisterAllLuaAPIs` + `RegisterLuaEnums` + the 5 real C bindings |
| 3 | `Server/Zone/ShnDataFileCheckSum.cpp` had wrong include (`Md5/`) and wrong API (`HexOf`) | Replaced the duplicate with a thin facade that includes the canonical `DataReader/ShnDataFileCheckSum.h`. The DataReader implementation is the only emitter |
| 4 | Several PDB-confirmed NC_* opcodes missing | Added as `// PDB_NAME_ALIAS` aliases pointing at the existing numeric slots: `NC_USER_CLIENT_VERSION_CHECK_REQ`, `NC_ACT_MOVERUN_CMD`, `NC_ACT_MOVEWALK_CMD`, `NC_ACT_STOP_REQ`, `NC_ACT_STOP_CMD`. Plus `// PROVISIONAL_ALIAS` for `NC_BAT_NORMALATTACK_CMD` and `NC_BAT_HIT_REQ` (both forms tolerated until a packet capture confirms which the live server emits). Client-side UIResourceTables typos fixed: `NC_USER_WORLD_SELECT_*` -> `NC_USER_WORLDSELECT_*`, `NC_QUEST_BEGIN_REQ` -> `NC_QUEST_START_REQ`, `NC_PARTY_REQ` -> `NC_PARTY_CREATE_REQ`, `NC_ATTEND_GET_CMD` -> `NC_CHAR_EVENT_ATTENDANCE_CHECK_CMD` (PDB-confirmed) |
| 5 | `SQLP.cpp` used `va_list` without `<stdarg.h>` | Added `#include <stdarg.h>`. Switched to `_vsnprintf_s` under `_MSC_VER` for VS2010-safe truncation |

Bonus fixes from the audit's broader observations:

* **Quest SHN policy correction** -- the initial response added a blanket guard that skipped quest/PineScript SHNs at the registry level. The user clarified the intended split:
  * **Generic SHN tooling / CSV exporter / audit parser**: BLOCK `Quest*.shn` and `PineScript*.shn` (these tools mis-parse or false-flag the dedicated on-disk shape).
  * **Runtime server data loader**: ALLOW `Quest*.shn`, but route them through the dedicated quest/scenario loader (`Server/DataReader/QuestShnReader`).

  Implementation:
  * `ShnRegistry::IsQuestShn(stem)` -- public predicate any tool can call to decide whether to touch a file.
  * `ShnFile::MarkAsQuestDeferred(path)` / `IsQuestDeferred()` / `QuestDeferredPath()` -- runtime tag set at boot in `EnumerateShn`. The placeholder carries no rows or columns; generic getters fail silently for it.
  * `QuestShnReader::LoadAllDeferred()` -- runs after `ShnRegistry::LoadAll` in `Server/Zone/Main.cpp`; walks the registry, picks every quest-deferred placeholder, dispatches to a per-file specialization (QuestData / QuestDialog / QuestSpecies / QuestEvent / QuestFramework / PineScript). Specializations slurp the bytes; column extraction fills in as each on-disk shape is RE'd.
  * `ShnAudit_EmitReport` -- explicitly skips quest SHNs (BLOCK rule applied to the audit parser).
  * `Server/DataReader/ShnCsvExporter` -- new generic CSV exporter, skips quest SHNs by the same rule.

Functional gaps from the audit (sections "Critical functional gaps" and
"Runtime robustness issues" of `fiesta_static_code_audit_report.md`)
are deferred to the runtime-debug round and tracked under sections 5
and the existing VERIFY markers below. Specifically:

* Player-only target lookup -- needs a unified `ShineObject` registry. Marked deferred.
* Skill cooldowns are global/static (`static CharacterSkill s_kDummy`). Deferred.
* Roe combat function-surface naming. Deferred (re-shape, don't re-tune).
* Lua API surface mostly stubs. Deferred.
* Login WM endpoint hardcoded to 127.0.0.1:28000. Deferred.
* `PacketBuffer::Reserve` returns void on alloc failure. Deferred (low risk on small game-state buffers, but should be tightened).
* `Socket_Acceptor::FlushSend_NoLock` blocking send. Deferred.

---

## 1. Dead anonymous-namespace stub TUs

The following 27 `.cpp` files contain a class wrapped in
`namespace fiesta { namespace { ... } }` whose symbols are NEVER
referenced from anywhere else in the tree. They exist to occupy a
project-file slot mirroring the NA2016 PDB layout, not to provide
working behaviour:

```
AbnormalState.cpp                 -- aggregate already in AbState.h
AbnormalStateDictionary.cpp       -- string/id mapping, no callers
AbnormalStateShelter.cpp          -- shelter-list, no callers
AttackRhythm.cpp                  -- per-class swing cadence, no callers
BRAccUpgradeDataBox.cpp           -- accessory-upgrade table, no callers
BoothManager.cpp                  -- player shop booths, no callers
CharacterSkill.cpp                -- skill loadout, no callers
GuildAcademy.cpp                  -- guild academy, no callers
KQContribute.cpp                  -- kingdom-quest contribution, no callers
KQRewardDataBox.cpp               -- kq reward table, no callers
MIDRewardDataBox.cpp              -- monster-island reward table, no callers
MapDataBox.cpp                    -- duplicate of MapField.h struct, no callers
MinimonDataBox.cpp                -- minimon table, no callers
MoverDataBox.cpp                  -- mover/teleport table, no callers
MoverUpgradeData.cpp              -- mover upgrade table, no callers
MultiHitTable.cpp                 -- multi-hit lookup, no callers
MysteryVaultTable.cpp             -- mystery vault table, no callers
NearScan.cpp                      -- proximity scan, partially used
PartyContainer.cpp                -- party state, no callers
PetRelatedSkill.cpp               -- pet skill table, no callers
RuleOfEngagement.cpp              -- PvP rules, no callers
SellItemManager.cpp               -- per-NPC sell stock, no callers
SetItemData.cpp                   -- equipment set bonuses, no callers
SkillDataBox.cpp                  -- skill table, no callers
SpamerPenaltyDataBox.cpp          -- spam-penalty table, no callers
SubAbstatePriority.cpp            -- abstate priority, no callers
TargetAnalyser.cpp                -- target picker, no callers
```

The reason they're in anonymous namespaces is to suppress LNK2005
collisions with the same class names that already appear inside
aggregate headers (`ItemSystems.h`, `MapField.h`, etc.). When you
flesh one of these out, lift it OUT of the anonymous namespace and
delete the duplicate declaration in the matching aggregate header.

`MapBlockInformation.cpp` is a no-op TU after the .shbd refactor --
the canonical struct moved to `MapField.h`. Either delete the file or
leave it; it emits no symbols either way.

## 2. Invented constants (I had no source data; placeholder values)

* `Server/Zone/LiveOpsBoosts.h` -- `kGMEvent_LuckyHour=1001`,
  `kGMEvent_DoubleExp=1002`, `kGMEvent_DoubleDrop=1003`,
  `kGMEvent_GoldenHour=1004`. Real GMEvent.shn EventNo values are NOT
  catalogued in this tree. Symptom of mismatch: WM windows fire but
  zones only emit a generic "GM event N has begun" banner instead of
  applying boosts.
* `Server/Zone/MapField.h` -- `kMapBlockCellSize = 16`. World-to-cell
  ratio inferred from a doc comment. Wrong value = players phase
  through walls or get stuck in walkable terrain.
* `Server/Zone/BattleTunables.h` -- `kFailPenaltyAtLevel[]`,
  `kUpgradeCritRollMax`, `kDropRateGlobalScalerX1k`. Header itself
  admits these need community-theorycraft verification.
* `Server/Zone/DeathReviveSystem.cpp` -- `ExpLossOnTownRevive` curve
  is a placeholder.

## 3. Invented packet shapes (no NA2016 capture available)

These three opcodes have body layouts I made up to match what the
existing zone-side handlers expected. They WILL mismatch a real client.

* `NC_ITEM_UPGRADE_OPEN_CMD` (NC_FAMILY_ITEM + 0x13)
  Current: `{ uint32 npcId, uint8 kind, uint8 allowsLuck }`
  Sent from `NPCSystem::HandlePick` on an "Upgrade"/"Enchant" button.
* `NC_NPC_MENU_PICK_ACK` (existing opcode, body shape not from capture)
  Current: `{ uint32 npcId, uint32 viewInfoId, uint8 result, string msg }`
  Used for every Promote/Save/Recall/etc. response.
* `NC_INTER_GMEVENT_TRIGGER_REQ` (NC_FAMILY_INTERSVR + 0x15)
  Current: `{ uint32 eventNo, uint32 durationSec, uint8 action }`
  Internal Zone -> WM admin push. Doesn't need to match a real client
  since both ends are this server, but the opcode number itself
  could collide with an unmapped original-server inter opcode.

## 4. Schema lookups I haven't proven exist

* `Server/Zone/GroupTables.cpp` reads `Money` from MobInfo.shn into
  `MobInfoRow.uiMoney`. ShnGetU32 returns 0 if the column is missing,
  which silently disables the gold-drop path. Verify the column name
  matches the actual NA2016 MobInfo.shn header.
* `Server/Zone/GMEventManager.cpp` reads `EventNo`, `StartTime`,
  `EndTime` from GMEvent.shn. Same risk -- a missing column makes
  every event look like it has a 1-hour duration.

## 5. Functional gaps (will compile + run, but the feature is wrong)

* `ItemUpgrade::Try` now consumes one UpResource stack and one Luck-Stone
  on every attempt (PASS6-007 / PASS6-008). The Luck-Stone identity is
  inferred from `ItemInfo.ItemUseSkill == "LuckStone"`; if the live
  client uses a different anchor (e.g. an `ItemFunc` flag), update the
  predicate in `ItemUpgrade.cpp`.
* `WMClient::OnTakeItem` now removes the in-memory ShineItem matched by
  `uiDbItemKey` (PASS6-009). The CharDB-side row deletion still flows
  through the panel's `NC_OPTOOL_QUERY_REQ` path; the Zone branch keeps
  the connected client's view consistent.
* `NC_INTER_BROADCAST_CMD` now carries a unified kind-byte protocol
  (PASS6-010): 0=ChatShout, 1=WorldYell, 2=GMEvent, 3=DailyReset,
  4=NpcSchedule. Zone consumers register through `DailyResetSink` /
  `NpcScheduleSink`. The current daily/npc handlers in `Main.cpp` log
  the event; subsystems should register their own callbacks for actual
  state mutation.

## 6. Things I'd push back on if I were reviewing

* The 350+ Zone .cpp files list mirrors the PDB, but a lot of those
  files are 5 lines long. Suggest collapsing the dead-stub batch
  into 5-10 thematic aggregate files (one per subsystem) once the
  build is stable -- cuts compile time and link surface meaningfully.
* `Server/Zone/AmpersandCommands.cpp` has a 154-entry default-handler
  table where every command logs and returns true. Until a command
  has a real body, those slots advertise capabilities the server
  doesn't actually deliver. Consider dropping the unimplemented
  entries from the public ampersand list rather than acking them.
* `LiveOpsBoosts::Get()` is a process-wide singleton; if the project
  ever runs multiple Zones in one process (unlikely but technically
  possible per the architecture), boosts would leak across them. The
  proper home is `ZoneServer` instance state.

## 7. Things I'm pretty confident in

* SHN/DAT readers (`Server/DataReader/`) -- typed schemas match the
  observed headers. The .shbd reader was tightened to fail-loud on
  geometry mismatches in this pass.
* `LiveOpsBoosts` core scaler logic -- math is correct, expiry tick
  works, applies cleanly in Battle.cpp and ItemSystems.cpp.
* The GMEventManager_Zone broadcast plumbing -- the wire path is
  consistent end-to-end (Zone -> WM -> all Zones -> LiveOpsBoosts).
* The map-block load failure path is now hard-failure instead of
  silent-corruption.
* Anti-cheat session for ItemUpgrade -- single-shot session keyed by
  CharID, cleared on disconnect, refuses raw NC_ITEM_UPGRADE_REQ
  packets without an open NPC click.

---

If any of the items in sections 1-5 are blockers for your audit pass,
flag them and I'll wire up the data binding (or remove the feature) in
the next round.
