# 2026-02 wiring sprint — changelog snapshot

This document records the per-session deltas during the data-wiring
sprint kicked off after the user's "every column has a purpose"
review. Earlier sessions are summarized in PRD.md.

## 2026-02-XX — wiring + casino + portals + capsules

### Convention adopted
**One `.cpp` + `.h` per `.shn` file.** NA2016 canon. Rationale: avoids
the static-initialization-order trap that consolidated mega-binders
hide. Going forward, every new SHN binder lives in its own translation
unit. Legacy mega-binders (`ExtendedTables.cpp` 38 binders,
`GroupTables.cpp` 13, `MoreTables.cpp` 16, `MiscTables.cpp` 7,
`WorldTables.cpp` 7) flagged for a future split sprint.

### Boot-wiring fixes (silent fallback bugs)
14 tables were declared with `Load(rRoot)` / `Load()` but never
called from any boot path; their lookups silently returned the
hard-coded neutral fallback. Wired all 14 from `Zone/Main.cpp`:

- `LevelGapTable` (PvP/PvE/EvP damage scaling) -- previously every
  level-gap multiplier returned 1000 (1.0x), silently disabling
  per-level PvP scaling. The PvP path was the worst: the SHN ships
  as a 151x151 matrix (`MyLv` x `TargetLv1..150`) but the loader was
  looking for `LevelDiff` / `Multiplier` columns that don't exist.
  Rewrote both loader and storage (1-D PvE/EvP curve + 2-D PvP
  matrix).
- `MobResistTable`, `ExpRecalcTable`, `MoverMainTable`,
  `MoverAbilityTable` -- combat hot-path data; same silent-fallback
  class.
- `QuestTable`, `RecallCoordTable`, `DamageByAngleTable`,
  `DamageBySoulTable`, `ItemUseFunctionTable`, `RandomOptionTable`,
  `ItemDropGroupTable`, `PineScriptTable`, `SubLayerInteractTable`
  -- World/*.txt singletons.
- `AbnormalStateDictionary`, `SetItemData`, `MultiHitTable`,
  `GuildAcademy`, `MinimonDataBox`, `SpamerPenaltyDataBox` --
  ShnRegistry-derived singletons.

### New features (per-SHN file convention from day one)
Three feature buckets the user explicitly called out as missing:

1. **Portals** (`Link.cpp/h`, `TownPortalSystem.cpp/h`)
   - Map portal + Karen portal: contact-trigger via `MapWayPoint.shn`
     gates joined against the `MapLinkPoint.shn` graph.
     `Link::OnPlayerStep(pk, map, x, y)` is the runtime entry.
   - Town portal: scroll item-use opens picker UI listing rows from
     `TownPortal.shn` filtered by the scroll's group + player level;
     `H_TownPortalReq` consumes scroll on successful pick.

2. **Lucky Capsules** (`LuckyCapsuleSystem.cpp/h`)
   - Three-color (red/blue/yellow) random-item capsules.
   - `LCGroupRate.shn` -> per-capsule probability over reward
     groups (per-100k weights).
   - `LCReward.shn` -> per-group reward bag, uniform pick within
     the chosen group.
   - Hooked to `NC_ITEM_USE_REQ` via `LuckyCapsuleSystem::IsCapsule`
     dispatch.

3. **Casino / Gamble House** (1 orchestrator + 18 per-SHN binders)
   - Currency: gold <-> Lucky Coin chips via `GBHouse.shn`
     (rate, tax, daily reset clock) + `GBExchangeMaxCoin.shn`
     (per-tier exchange cap).
   - Dice (Tai Sai / sic-bo): three-d6 roll resolved via
     `DiceGame.shn` (per-bowl tunables) + `DiceRate.shn` (win/loss
     wheel) + `DiceDividind.shn` (16-tier payout ladder).
   - Slots (3-reel): symbol-weighted reel pick from `GBSMCardRate`,
     scored against `GBSMLine`/`GBSMCenter`/`GBSMAll` group rules,
     JP bonus from `GBSMJPRate` when pool is hot. NPC -> bet-tier
     mapping from `GBSMNPC`. Min/max seat from `GBJoinGameMember`.
     Per-game-type tax/JP-save from `GBTaxRate`. Item-prize drops
     from `GBReward`. Bet-cooldowns from `GBBanTime`. Animation
     event codes from `GBEventCode`.
   - Hooked to `NC_GAMBLE_DICE_REQ` and `NC_GAMBLE_SLOT_REQ`
     handlers in `ZoneHandlers.cpp`.

### CI / static audits added
- `Build/CI/audit_shn_columns.py` -- per-(table,column) classifier
  (GREEN/YELLOW/RED). Run with `--system <substr>` to slice by
  feature.
- `Build/CI/audit_unwired_loads.py` -- catches the LevelGapTable
  bug class (Load defined but never called from boot). Runs in
  the canary workflow.
- Existing `Build/CI/audit_shn_wiring.py` extended with the
  CLIENT_ONLY_VIEWS whitelist for the 18 client-only View tables.
- All three audits are wired into `.github/workflows/canary.yml`
  shn-integrity job. Push to main = automatic verification.

### CI infra fixes
- `windows-2019` runner image was retired by GitHub mid-2025;
  bumped to `windows-2022` (v141 toolset still preinstalled).
- Repo name ends in `.` which Windows strips from directory paths;
  workflow now checks out into a `src/` subfolder to sidestep.
- Recommended one-time fix is to rename the GitHub repo (drop
  trailing `.`); the workflow workaround can stay regardless.

### Canon documentation refresh
- `docs/SCHEMA_COVERAGE.md` -- canonical wiring report.
- README updated to point at the data-dictionary + audit toolchain.

### Backlog tracked elsewhere
- Mega-binder split (5 files -> ~80 per-SHN files). Risky; needs
  full re-audit. Not safe to do under credit pressure.
- Slot reel-symbol probe currently scans card ids 1..32; tighten
  once the GBSMCardRate ingest exposes the iteration helper.
- TownPortalSystem.ResolveItemToGroup uses a string-substring
  heuristic for scroll -> group; replace with a server-owned table
  once the canon scroll-id mapping is verified against live data.
- 12 file-walked Load() methods (MobAttackSequence, MobBehavior,
  MobRegen, MobRoam, MobSettingAction, NPCItemList, PsScript,
  AIScript, ScenarioScript, ScriptStringTable, FieldTable,
  AbnormalStateShelter) are correctly wired via ZoneAssetLoader's
  per-file walker; whitelisted in `audit_unwired_loads.py`.

### Coverage signals after this sprint
- Server-side SHN coverage:    201/201 (100%) -- 18 client-View
  tables intentionally unread.
- Unwired Load/Bind orphans:   0
- New `// FEATURE: <bucket>` tags shipped: portals, lucky-capsule,
  casino-house, casino-exchange, casino-tax, casino-dice,
  casino-slot, casino-reward, casino-bantime, casino-seatcap,
  casino-eventcode, combat-data-load, world-data-load,
  shn-derived-data-load, lvgap-damage-curve.
