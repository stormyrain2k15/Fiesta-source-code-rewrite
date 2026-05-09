# Honest Disclosure: Stubs, Guesses, and Verify Markers

This document is for the next reviewer (codex audit pass, opus static
debug, sonnet runtime debug, opus function verification). It catalogs
everything I know to be incomplete, invented, or stubbed in the Zone
server tree so you don't have to discover it the hard way.

I'd rather you find this list and triage it than spend cycles
chasing ghosts in a 350-file directory.

---

## Audit pass log

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

* `ItemUpgrade::Try` does NOT consume the `UpResource` material from
  the player's inventory. The original game removes one upgrade stone
  per attempt regardless of outcome. Players currently get unlimited
  free attempts. Marked with VERIFY in the code.
* `ItemUpgrade::Try` IGNORES `bUseLuckStone` (logs a warning and
  treats it as false) because the Luck-Stone item id binding isn't
  configured. Marked with VERIFY in the code.
* `WMClient::OnTakeItem` does not actually delete the item from the
  player's Inventory; it only logs. The OPTool DB-side delete works
  through a separate path. Marked in the existing comment.
* `WorldManager::DailyQuestTimer` and friends fan out via
  NC_INTER_BROADCAST_CMD kind=1 but the Zone side does not yet handle
  kind=1 (only kind=2 GM events are wired). Daily resets are dropped
  on the floor.

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
