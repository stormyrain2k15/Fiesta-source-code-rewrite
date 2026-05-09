# Pass 1.21 -- Runtime hooks: Mob spawning, Aura propagation, CharDB persistence

**User trigger (verbatim):**
> "Do all of it, and the folder path is 9Data/Shine/"

This pass turns the four "stubs" left at the end of Pass 1.20 into
working runtime: the Mob spawn driver actually instantiates mobs, the
Estate aura and Expedition master-buff actually push abstates, and
Marriage / Estate state actually round-trips through CharDB.

## What changed

### 1. Data root convention -- `9Data\Shine\`

`DataReader` default and `Main.cpp` `Data.Root` default both updated to
`9Data`. Per-folder loaders continue to receive `<dataRoot>\Shine`.

### 2. `MobSpawnSystem.h / .cpp` (new)

Driver on top of the now-populated `MobRegenBox`. At boot,
`LoadAll()` walks every `MapInfoRow`, looks up the matching
`MobRegenTable`, and builds one `MobSpawnGroup` per `MobRegen` row
(species via `MobTables::FindByInx`, area via the referenced
`MobRegenGroup`).

Per tick, each group below its `MobNum` target spawns a fresh
`ShineMob` inside the `(CenterX,CenterY)` rectangle, assigns a new
`Handle` from `ZoneServer::NewObjectHandle()`, and adds it to the
matching `Field`.

`OnMobDied(Handle)` is called from `Battle::Kill` when an `OT_MOB`
target's HP hits zero; the slot is freed and the per-group
`uiNextRollMs` is bumped by `RegStandard` seconds before the next
spawn attempt.

### 3. `EstateSystem.cpp` -- furniture aura propagation

`EstateServer::Tick(uiNowMs)` now does two things per estate:

1. **Hourly endure decay** (unchanged).
2. **Per-tick furniture aura**: for every placed furniture row that has
   a `MiniHouseFurnitureObjEffect` entry, the abstate is applied to the
   owner via `pkOwner->AbState().Apply(uiAbStateID, 90s)` and to every
   `OT_PLAYER` in the same `Field` within `Range` cells of the
   placement's `(X,Y,Z)`.

The owner is resolved via `ZoneServer::FindPlayerByCharID(uiOwner)`;
if they're offline the aura simply isn't applied.

### 4. `ExpeditionSystem.cpp` -- master buff propagation

`ExpeditionSystem::Tick()` walks every active expedition with a
non-zero `uiMasterBuffID` and re-applies the abstate to every member
of every sub-party. `PartyContainer::Get(uiPartyId)` was added (was
missing) so we can iterate `Party::kMembers` and resolve each
`ShinePlayer` via `ZoneServer::FindPlayerByCharID`.

### 5. `CharDBClient.h / .cpp` -- new persistence opcodes

Adds fire-and-forget routes for Marriage / HolyPromise / Estate.
Opcodes share the existing `NC_INTER_CHAR_DB_QUERY` envelope:

| Op | Procedure                       | Args                              |
|----|---------------------------------|-----------------------------------|
| 10 | `p_Char_Wedding_Propose`        | (cFrom, cTo)                      |
| 11 | `p_Char_Wedding_Cancel_Propose` | (cFrom, cTo)                      |
| 12 | `p_Char_Wedding_Do`             | (cA, cB)                          |
| 13 | `p_Char_Wedding_Divorce_Do`     | (c)                               |
| 20 | `p_HolyPromise_Set`             | (cA, cB)                          |
| 21 | `p_HolyPromise_SetDate`         | (c)                               |
| 22 | `p_HolyPromise_DelChar`         | (c)                               |
| 30 | `p_Estate_Create`               | (owner, houseId, tier)            |
| 31 | `p_Estate_Demolish`             | (owner)                           |
| 32 | `p_Estate_SaveFurniture`        | (owner, hex blob of placements)   |
| 33 | `p_Estate_LoadFurniture`        | (owner) -> rows                   |

`MarriageServer::Propose / Reject / Wed / Divorce` now call the
matching CharDB route. `EstateServer::Create / Demolish / Place /
PickUp` autosave the current placement vector via op 32 (24 bytes per
placement, hex-encoded into the proc-arg buffer).

### 6. `SQLP.h / SQLP.cpp` -- new `SQLP_Estate`

New facade over `p_Estate_*`. The `Save` builder hex-encodes the
placement blob inline -- a 30-furniture house fits in 720B + 4B header,
well within ODBC `varbinary(8000)`.

### 7. CharDB exe dispatcher

`Server/DataServer/Character/Main.cpp` instantiates `SQLP_Estate` at
boot and the `NC_INTER_CHAR_DB_QUERY` switch now also routes ops
10..33 through a single `HandleSocialOp` function. Op 33
(`Estate_Load`) responds with a length-prefixed DBRecord array; the
others are fire-and-forget.

### 8. `Battle.cpp`

`Battle::Kill` now notifies `MobSpawnSystem::OnMobDied(handle)` when
the target was an `OT_MOB`, so the spawn slot frees up and the
respawn timer arms.

## Coverage matrix (post-1.21)

Same data coverage as Pass 1.20, plus *real runtime usage*:

| System              | Backed by data?                  | Ticks live? |
|---------------------|----------------------------------|-------------|
| MobSpawnSystem      | MobRegenBox + MobTables + Maps   | yes         |
| EstateServer        | MiniHouse* + EstateExtraTables   | yes (60s aura, 1h decay) |
| ExpeditionSystem    | GuildTournamentMasterBuff (when set) | yes (every Zone tick) |
| MarriageServer      | HolyPromiseReward                | yes (events round-trip to CharDB) |

## Known follow-up

* Estate furniture rehydration on player login: `EstateServer::Create`
  creates the in-memory record but doesn't yet pull existing placements
  from the CharDB via op 33 (the route is wired, the Zone-side response
  decoder still needs to populate `EstateRec::kPlacements`).
* `Battle::Kill` placeholder for drop / EXP / quest credit fanout.
* Per-class param tables (`World/ParamFighterServer.txt` etc) loaded
  but the level-up / free-stat path doesn't consult them yet.

No "Elle AI" hooks were re-introduced. No client version-check was
re-introduced. No standalone `OperatorTool` or `GamigoZR` was
re-introduced.
