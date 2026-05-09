# Pass 1.17 -- Tying off the next-action / backlog list (2026-02)

This pass closes the remaining `Next Action Items` and `Backlog` entries
flagged at the end of Pass 1.16. The user explicitly opted **out** of an
in-engine cash shop / item mall flow -- the original game's billing was
website-side and the engine only consumes the resulting time-limited
**charged effects** that arrive in inventory.

## Files added

- `Server/Shared/Socket_Connector.{h,cpp}` -- outbound IOCP client socket
  (counterpart to `Socket_Acceptor`). Single-shot synchronous connect,
  bind to IOCPManager, owns the session.
- `Server/Zone/CharDBClient.{h,cpp}`       -- Zone-side singleton link
  to the Character DB exe. Issues `NC_INTER_CHAR_DB_QUERY {op=Login}`,
  parses `NC_INTER_CHAR_DB_RESPONSE`, and dispatches the result row to
  `ShinePlayer::LoadFromCharDBRow`. CharLogout is fire-and-forget.
- `Server/Zone/WMClient.{h,cpp}`           -- Zone-side singleton link to
  WorldManager. Sends `NC_INTER_ZONE_REGISTER_REQ` on connect, periodic
  heartbeat from the Zone tick. Inbound dispatcher routes the entire
  `NC_INTER_OPTOOL_*_PUSH` family into local helpers (kick / jail / unjail
  / sysmsg broadcast / give-item / take-item).
- `Server/Zone/AggroList.{h,cpp}`          -- per-mob hate accumulator
  with sat-add, decay, Top() resolver, and the **20-level-span**
  `LevelGapMulX1k(playerLv, mobLv)` scaler:
  player at-or-below mob level → 1.000x; +20 levels above → 0; linear
  fade in between. `AddScaled()` is the convenience push that applies
  the scaler and rejects out-of-span attackers entirely.
- `Server/Zone/PotSystem.{h,cpp}`          -- inventory-consumed potion
  effect dispatcher. Cooldowns are kept per (CharID, kind) so HP and SP
  pots don't gate each other. Distinct from `SoulStoneSystem`.
- `Server/Zone/ChargedEffect.{h,cpp}`      -- loader for
  `ChargedEffect.shn` + `ChargedDeletableBuff.shn`. `OnItemReceived(item)`
  arms a typed booster instance for the configured `KeepTime_Hour`. The
  per-tick reaper drops expired entries.

## Files updated

- `Server/Common/NETCOMMAND.h`             -- added the
  `NC_INTER_OPTOOL_*_PUSH` family (`+0x0A..0x10`).
- `Server/Shared/Socket_Acceptor.{h,cpp}`  -- already had loopback flag
  from pass 1.16; no change here, just documented for reference.
- `Server/WorldManager/WorldManagerServer.{h,cpp}` -- WM now keeps a
  zone-id -> WMZoneSession map. New helpers
  `RegisterZoneSession`, `UnregisterZoneSession`,
  `BroadcastToZones`, `SendToZone`. The OPTool dispatcher now actually
  calls `BroadcastToZones` instead of just logging.
- `Server/Zone/Main.cpp`                   -- on boot connects
  `CharDBClient` and `WMClient`, loads `ChargedEffectTable` +
  `ChargedDeletableBuff`, ticks `BoothManager` and
  `ChargedEffectManager::Tick`, calls `WMClient::Heartbeat` from the
  zone tick.
- `Server/Zone/CharLogin.cpp`              -- on `NC_CHAR_LOGIN_REQ`
  fires the real CharDB login query and returns immediately; the
  asynchronous response handler overwrites the player's provisional
  fill via `LoadFromCharDBRow`.
- `Server/Zone/Battle.cpp`                 -- `Battle::Apply` now pushes
  the dealt damage as hate onto the target mob's `AggroList`, scaled by
  the 20-level-span rule (closer-or-below mob level == max aggro;
  > +20 above == zero).
- `Server/Zone/ShineObject.h`              -- `ShineMob` now owns
  `AggroList m_kAggro` and a copy of `m_uiLevel` (sourced from
  `MobInfo.shn` at spawn time).
- `Server/Zone/MobAISystem.{h,cpp}`        -- new
  `IsInAggroRange(mob, player, bChasing)` helper consults
  `MobInfoServer.shn DetectCha / FollowCha / EnemyDetectType`. Passive
  mobs never aggro on proximity; `bChasing` switches the radius
  test to `FollowCha`.
- `Server/DataReader/Schemas.{h,cpp}`      -- `MobInfoServerRow` got the
  three aggro-relevant columns wired up: `EnemyDetectType` (col 7),
  `DetectCha` (col 11), `FollowCha` (col 15). The earlier column
  guesses (DropItemListIdx, ResistanceIdx, ...) are now `memset(0)` at
  load -- they were reading wrong columns and the look-up-by-string
  path replaces them in pass 2.
- `Server/Zone/MarketSystems.cpp`          -- `StreetBooth` /
  `StreetBoothSell` / `StreetBoothBuy` / `BoothManager` got real
  storage: a per-owner `BoothRec { kListings, kTitle, uiCreatedMs,
  uiOwnerLastSeenMs }` map, plus a 15-minute stale-owner reaper in
  `BoothManager::Tick`.
- `Server/Zone/GuildSystem.{h,cpp}`        -- `GuildWarManager` got the
  declaration window. Default Fri/Sat/Sun 19:00..23:00 local; editable
  via `GuildWarManager::SetWindow`. `Declare` now rejects out-of-window
  requests.
- `Lua/LuaEnums_Generated.cpp`             -- replaced the placeholder
  with full enum tables: `ReturnAI`, `ObjectType`, `ObjectMode`,
  `BasicClass`, `Classes`, `EFFECT_MSG_TYPE`, `CAMERA_STATE`, `KQ_TEAM`,
  `PlayerDamage`, `PlayerList`, `StatsEnum`. Values mirror the
  documented numeric assignments.
- `Server/DataReader/DataReader.h`         -- added `GetRoot()` accessor
  used by ChargedEffectTable load.

## Items NOT done in this pass

- **In-engine ItemMall billing flow** -- explicitly off the table per the
  user. The game's cash shop is website-side; the only engine surface is
  `ChargedEffectTable` consumption.
- **NC opcode numeric pinning** from F2/Battle.zip captures -- not
  surfaced yet.
- **`.dat` animation sub-field offsets** -- still need sample dumps.
- **AbState save-on-link** -- the abstate is in-memory only; durable
  persistence is a CharDB schema extension (would need a `tAbState`
  table, not present in the .bak strings).

## Total source files
**~199** (10 added, several updated).
