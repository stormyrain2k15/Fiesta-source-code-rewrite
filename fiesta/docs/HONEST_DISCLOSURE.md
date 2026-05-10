# Honest Disclosure: Stubs, Guesses, and Verify Markers

This document is the running engineering tech-debt log for the Zone
server tree. It catalogs everything that is incomplete, inferred from
data layout, or marked PROVISIONAL pending real-client capture
verification, so the next person looking at a subsystem doesn't have
to discover it the hard way.

---

## How to read this file

Tagged comment conventions in the source:

| Tag | Meaning |
|-----|---------|
| `// VERIFY:` | Function shape is right; numeric constant is a placeholder |
| `// PROVISIONAL_BODY:` | Wire packet body shape inferred, not captured |
| `// PROVISIONAL` | Opcode number guessed (no PDB symbol confirmed) |
| `// TUNE:` | Knob deliberately exposed for live re-tuning |

Grep for any of these in `Server/Zone/` to find every untrusted
constant or wire shape.

---

## 1. Tunable constants (function known, value placeholder)

Centralized in:

* `Server/Zone/BattleTunables.h` — combat curves (Roe ATK/DEF/Hit/Crit),
  AbState action constants + DoT scaler + tick cadence, KQ reward
  base/perPoint/cap, well-known GMEvent ids, supplemental damage
  formula tunables (`kRawDmgMode`, `kQuadraticBias`, `kRawDmgScalerX1k`,
  `kLinearDefCapX1k`, `kDamageFloor`, `kBackHitMultiplierX1k`,
  `kFlankHitMultiplierX1k`, `kCritRollMax`, `kCritDamageScaler`,
  `kDamageVarianceX10k`, `kPvPDamageScalerX1k`).
* `Server/Zone/KingdomQuest.cpp` — KQ state-machine timings
  (`kKQRecruitTimeoutMs`, `kKQRunningTimeoutMs`, `kKQVoteTimeoutMs`,
  `kKQEndTimeoutMs`, `kKQRecruitMinPlayers`).
* `Server/Zone/CharDBClient.cpp` — AbState persistence stored-proc
  ids (`90` = AbState_Set, `91` = AbState_GetAll). Edit the literals in
  `AbStateSet` / `AbStateGetAll` to match your CharDB stored-proc map.
* `Server/Zone/ItemUpgrade.cpp` — upgrade success/luck-stone curve
  scaler.

---

## 2. Inferred SHN column meanings

Where the source-data column meaning was inferred from the layout
rather than supplied by formal documentation, the loader leaves a
`// VERIFY:` comment beside the field. The most consequential ones:

* `MobInfo.shn::Money` — used as the gold-drop base. Confirmed numeric;
  exact scaling vs. `MoneyRateX1k` and party-split is the placeholder.
* `GMEvent.shn::EventNo` — well-known event ids
  (`kGMEvent_LuckyHour=1001`, `kGMEvent_DoubleExp=1002`,
  `kGMEvent_DoubleDrop=1003`, `kGMEvent_GoldenHour=1004`) are project
  defaults. Reconcile against the data drop's actual EventNo numbers.
* `ItemInfo.shn::ItemUseSkill == "LuckStone"` — chosen string anchor
  to identify a Luck-Stone consumable. If the live data drop encodes
  Luck-Stones via a different column (e.g. an `ItemFunc` flag), update
  `ItemUpgrade::Try` to match.
* `SubAbState.shn::ActionIndexA..D` — 14 action ids (DotHP_A/B, DotSP,
  StatMod{ATK,DEF,MoveSpd,AtkSpd,MATK,MDEF}, RegenHP, RegenSP,
  DisableMove, DisableAct, AbsorbShield, ReflectDmg, DispelOnHit) are
  what the dispatcher recognizes today. Any other ActionIndex value
  surfaces a `SHINELOG_WARN` line at runtime and is NOT silently
  no-op'd — when you see one, add a handler in
  `AbStateRuntime::Tick`.

---

## 3. PROVISIONAL packet shapes

Wire packets where the body shape is inferred from neighbouring
opcodes / call sites and is awaiting capture validation:

* `NC_NPC_MENU_PICK_ACK`
* `NC_ITEM_UPGRADE_OPEN_CMD`
* `NC_INTER_GMEVENT_TRIGGER_REQ`
* `NC_ACT_SCRIPT_MSG_CMD` (PineScript narration push)
* `NC_MAP_DOOR_STATE_CMD` (cDoorAction)
* `NC_BAT_RESURRECT_ACK`
* `NC_INSTANCE_*` family (instance entry / leave)
* `NC_INTER_BROADCAST_CMD` kind-byte protocol
  (0=ChatShout, 1=WorldYell, 2=GMEvent, 3=DailyReset, 4=NpcSchedule)

Grep `// PROVISIONAL` and `// PROVISIONAL_BODY:` to find every
untrusted wire shape and opcode number.

---

## 4. Functional gaps that compile and run

* `ItemUpgrade::Try` — UpResource and Luck-Stone consumption is wired,
  but the "exact bucket-id to consumable-id mapping" is inferred from
  same-bucket `UpLimit==0` heuristic. If the data drop separates
  consumable-stones from upgradeable-items via a different column,
  edit the predicate inside `ItemUpgrade::Try`.
* `WMClient::OnTakeItem` — removes the matching `ShineItem` from the
  live `Inventory`. The CharDB-side row deletion still flows through
  the panel's `NC_OPTOOL_QUERY_REQ` path; the Zone branch keeps the
  connected client view consistent.
* `NC_INTER_BROADCAST_CMD` daily-reset (kind=3) and NPC-schedule
  (kind=4) handlers in `Main.cpp` log the event. Subsystems that need
  to mutate state on these triggers should register their own
  callback through `DailyResetSink` / `NpcScheduleSink`.
* Login `WMClient` endpoint is read from `LoginServer.ini`. If the
  ini is missing the key, the login process fails fast rather than
  hardcoding a fallback; add a `Wm.Endpoint` line to your config.

---

## 5. Removed-on-purpose

* Client-version handshake gate.
* Zone authorization / "X trap" client-tamper enforcement.
* Anti-cheat client-manipulation hooks.

These are intentional omissions, not bugs. Re-implement them on top
of the existing IOCP / session layer if you want them back.

---

## 6. Deferred items

* `AbStateRow::MainStateInx` is loaded but no consumer yet. Walking
  `MainStateInx -> AbState parent -> chained SubAbState rows` will
  give "ultimate skill spawns N secondary buffs" patterns for free
  once a runtime trace exists to validate against.
* `QuestShnReader` real row parsing requires a binary RE pass across
  the `9Data/Quest/*.shn` data set; current implementation reads the
  shape but not the payload sub-fields.
* `cMobRegen_Obj` resolves the reference object's map via a linear
  scan over `MapTables`. Add a `MapID -> kMapName` reverse index on
  `MapTables` if this shows up in profiling.
