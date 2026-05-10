# Fiesta Online Server + Client Engine

## Goal

Reproduce the NA2016-era Fiesta Online server and client engine from the
project owner's documentation pack and the vendored Gamebryo 2.3 CoreLibs.
Targets: VS2010 (`v100` / Win32), Lua 5.2, Trinigy/Gamebryo as the
read-only client renderer.

## Layout

```
/app/fiesta/
├── Server/
│   ├── Shared/              foundation: timers, packet, crypto, IOCP,
│   │                         WinService, ShineLog, well512 RNG
│   ├── Common/              NETCOMMAND, ProtocolParser, GPacket
│   ├── DataReader/          SHN / TS / BMP / DAT loaders, ITableBase
│   ├── DataServer/
│   │   ├── Account/         AccountDB exe (SQLP_Account / SQLP_IPChecker)
│   │   ├── AccountLog/      AccountLog exe (SQLP_AccountLog)
│   │   ├── Character/       CharDB exe (Item / Quest / Skill / Friend /
│   │   │                     Estate / Wedding / HolyPromise / Guild
│   │   │                     stored procs)
│   │   └── GameLog/         GameLog exe (kill / drop / trade / report)
│   ├── Login/               credential authority, AccountLog writer
│   ├── WorldManager/        world router, OPTool loopback
│   └── Zone/                gameplay engine + 43 subsystems
├── Lua/                     Lua 5.2 host + cBindings
├── Client/                  client shell + Gamebryo bootstrap
└── ThirdParty/Gamebryo/     read-only vendored renderer
```

## Persistent state -- what writes to the DB and from where

| Event                       | Source                          | DB target / proc                           |
|-----------------------------|---------------------------------|--------------------------------------------|
| Login attempt               | `Login::HandleLogin`            | Account.p_VerifyLogin + AccountLog append  |
| Char select / spawn         | `CharDBClient::QueryCharLogin`  | Character.p_Char_Login                     |
| Char logout / quit          | `CharDBClient::QueryCharLogout` | Character.p_Char_Logout                    |
| Inventory add               | `Inventory::Add`                | Character.p_Item_Create (returns ItemKey)  |
| Inventory remove            | `Inventory::Remove`             | Character.p_Item_Delete (by ItemKey)       |
| Bind-on-equip               | `Inventory::Equip`              | Character.p_Item_SetOption                 |
| Item upgrade success/dn     | `ItemUpgrade::ResolveForPlayer` | Character.p_Item_SetOption (type=1 enchant)|
| Item upgrade destroy        | `ItemUpgrade::ResolveForPlayer` | Character.p_Item_Delete                    |
| Quest accept / abandon      | `QuestRuntime::StartQuest`      | Character.p_Quest_Set                      |
| Quest complete              | `QuestRuntime::CompleteQuest`   | Character.p_Quest_Set                      |
| Quest list reload           | `CharDBClient::QuestGetDoing`   | Character.p_Quest_GetAllDoing              |
| Skill learn / upgrade       | `CharacterSkill::Learn`         | Character.p_Skill_SetPower                 |
| Friend purge                | `CharDBClient::FriendDelAll`    | Character.p_Friend_Del_All                 |
| Guild tournament outcome    | `CharDBClient::GuildTournament` | Character.p_GuildTournament_Set            |
| Estate save / load          | `CharDBClient::Estate*`         | Character.p_Estate_Save / p_Estate_Load    |
| Wedding propose / vow / etc | `CharDBClient::Wedding*`        | Character.p_Wedding_*                      |
| Holy Promise (oath)         | `CharDBClient::HolyPromise*`    | Character.p_HolyPromise_*                  |
| Mob kill                    | `Battle::Kill` -> GameLogClient | GameLog.p_Log_Combat                       |
| Drop pickup                 | `ItemDropFromMob::Trigger`      | GameLog.p_Log_Drop                         |
| Trade flush                 | `GameLogClient::LogTrade`       | GameLog.p_Log_Trade                        |
| Report ticket               | `GameLogClient::Report`         | GameLog.p_Report_File                      |
| AccountLog login / logout   | `Login` co-loaded SQLP          | AccountLog.p_AppendLogin / Logout          |

Every wrapper has a matching dispatcher case in the corresponding DB-exe,
and every dispatcher case calls a real `SQLP_*` method on a connected
`Database` (ODBC). The Item-key reconciliation is asynchronous: the
`p_Item_Create` ack carries the SQL IDENTITY back which is stamped onto
the in-flight `ShineItem` so the matching `p_Item_Delete` /
`p_Item_SetOption` calls hit the right row.

## Data ingest

* SHN registry: 199 typed + generic-path tables, every column either
  hand-bound or audited by `ShnAudit_EmitReport` at boot.
* TS-format `World/*.txt`: BlockInfo, MobRegen, MobRoam, MobSetting,
  MobAttackSequence, NPCAction, NPCItemList, ScenarioBookShelf -- all
  parsed; column-level audit emits a WARN line for any unread column.
* `9Data/Shine/<Map>/Lua/*.lua` and `*.ps` -- ZoneAssetLoader walks the
  per-folder tree and registers every script with the Lua host.

## Subsystems (Zone)

Account / Character / Inventory / Equip / Stat / Battle / Skill / AbState /
Quest / NPC / Mob / MobAI / Drop / Item / Wedding / HolyPromise / Estate /
Marriage / Expedition / Guild / GuildAcademy / GuildTournament / KQ / KQ
Reward / Auction / TradeBoard / Trade / StreetBooth / Mover / Pet / Mining
/ Production / Title / Friend / Party / PartyMatch / Bookshelf / Promote /
PromoteScenario / MIDungeon / WMRouting / OPTool / Scheduler / Lua /
ChargedEffect.

## Client integration

* The renderer is the read-only vendored Gamebryo 2.3 CoreLibs; the engine
  shell links against it but does not modify the source.
* NPC dialog box layout / button list is built server-authoritatively from
  `NpcDialogData.shn` + `NPCViewInfo.shn` and dispatched via
  `NC_NPC_MENU_OPEN_CMD`.
* Map auto-effects (StateField.shn) apply on player spawn through
  `StateFieldTable::OnPlayerEnter`.

## Build

VS2010 toolset, see `docs/BUILD.md` for project file layout. Compilation is
performed locally on Windows; this repo is C++ source only.

## WorldManager file layout (canonical NA2016 mapping)

```
Server/WorldManager/
├── Main.cpp                          service entry; boots DataFileServer,
│                                     opens 5 acceptors, ticks WMServices
├── WorldManagerServer.{h,cpp}        zone registry, token bridge, fanout,
│                                     WMClient/WMLogin/WMCharDB sessions
├── WMServices.{h,cpp}                14 cross-zone subsystem singletons +
│                                     WMServicesTickAll() master ticker
├── DataFileServer.cpp                SHN ingest + column auditor
├── GuildServer.{h,cpp}               cross-zone guild authority
├── KQServer.cpp                      Kingdom Quest voting / lifecycle
├── FriendServer.cpp                  presence + friend-list deltas
├── ChatStealServer.cpp               world-wide chat fanout + mute
├── PartyFinderServer.cpp             cross-zone LFG board
├── RankingServer.cpp                 leaderboard merge + hourly publish
├── PrisonServer.cpp                  PK sentencing + decrement timer
├── HolyPromiseServer.cpp             couple registry
├── GambleSystemServer.cpp            dice / slot-machine / coin window
├── EventAttendanceServer.cpp         daily login attendance
├── DailyQuestTimer.cpp               daily/weekly/monthly reset broadcast
├── NpcScheduleServer.cpp             time-windowed NPC spawn/despawn
├── GMEventManager.cpp                GM event start/end fanout
├── MatchInstanceDungeonServer.cpp    MID party queue
├── PF_Zone.cpp                       protocol family: WM<->Zone register/HB
├── PF_OPTool.cpp                     protocol family: OPTool admin
├── PF_AccountDB.cpp                  outbound proxy: AccountDB queries
├── PF_AccountLogDB.cpp               outbound proxy: AccountLog audit
├── PF_CharDB.cpp                     inbound dispatcher for CharDB acks
├── PF_GameLogDB.cpp                  outbound proxy: GameLog audit
├── WMCharDBClient.{h,cpp}            outbound: guild/friend/mail/KQ/event/
│                                     daily/ranking writes -> CharDB exe
```



## Live-ops boost system (Lucky Hour)

Zone-side `LiveOpsBoosts` singleton (`Server/Zone/LiveOpsBoosts.{h,cpp}`)
holds three multiplicative scalers (x1000, 1000 == stock):

* `ExpRateX1k()`   -> applied in `Battle.cpp` after `ExpRecalcTable` scaler
* `DropRateX1k()`  -> overrides `DropContext.nGlobalRateX1k` in
  `ItemSystems.cpp::ItemDropFromMob::Trigger`
* `MoneyRateX1k()` -> applied in `Battle.cpp::Kill` against
  `MobInfoRow.uiMoney` on every mob-kill grant

Well-known event ids (extends GMEvent.shn):

| Id   | Name        | Effect                         |
|------|-------------|--------------------------------|
| 1001 | LuckyHour   | EXP +100% AND Drop +100%       |
| 1002 | DoubleExp   | EXP +100%                      |
| 1003 | DoubleDrop  | Drop +100%                     |
| 1004 | GoldenHour  | Money +100%                    |

Trigger paths:
1. WM-side `GMEventManager.cpp` walks `GMEvent.shn` once per tick and
   broadcasts `NC_INTER_BROADCAST_CMD { kind=2, eventNo, action }` on
   every window enter / exit.
2. Zone-side `WMClient::OnInterBroadcast` decodes kind=2 and calls
   `GMEventManager_Zone::OnEventBroadcast(eventNo, bStart)`, which
   resolves the duration from the same `GMEvent.shn` row and forwards
   to `LiveOpsBoosts::StartEvent / StopEvent`.
3. GM ad-hoc trigger: `&luckyhour <minutes> [expX1k] [dropX1k]` via
   `AmpersandCommands.cpp` (admin level 100). `&luckyhour stop` ends.
   The command sends `NC_INTER_GMEVENT_TRIGGER_REQ` to WM via
   `WMClient::SendGMEventTrigger`; WM (`PF_Zone.cpp`) re-broadcasts
   `NC_INTER_BROADCAST_CMD` kind=2 to every connected zone, so a single
   GM click hits every shard. WM-offline fallback runs the boost on the
   issuing zone only.

Tick: `LiveOpsBoosts::Tick()` runs from `ZoneServer::Tick()` and
auto-reverts when `m_uiEndsAtMs` elapses.

## NPC enchanter (+N upgrade) wiring

NA2016-style flow:
1. Player clicks an enchanter NPC. NPCSystem renders the click menu via
   `NPCAction.txt` + `NPCViewInfo.shn`.
2. Player selects an "Upgrade" / "Enchant" / "Refine" /
   "EquipmentUpgrade" button. `NPCSystem::HandlePick` recognises the
   action tag, opens an enchanter session via
   `ItemUpgrade::OpenSession(player, npcId, kind)`, and sends
   `NC_ITEM_UPGRADE_OPEN_CMD { npcId, kind, allowsLuck }` to the client
   so the upgrade UI opens.
3. Player chooses an item; client sends `NC_ITEM_UPGRADE_REQ { itemId,
   useLuckStone }` -> ZoneHandlers -> `ItemUpgrade::ResolveForPlayer`.
4. `ResolveForPlayer` requires an open session (anti-cheat against raw
   packet injection); the session is single-shot and is closed before
   the roll, regardless of outcome.
5. `ClientSession::OnDisconnect` calls `ItemUpgrade::CloseSession` so
   abandoned sessions don't leak memory.

`kind`: 0=any, 1=weapon, 2=armour, 3=accessory; resolved from the
`NPCViewInfo` row's `Arg0`.

## Map block reader (BlockInfo/.shbd)

* `Server/DataReader/BlockInfoFile.{h,cpp}` parses the three terrain
  binary formats (`.shbd` cell grid, `.aid` area-id table, `.sbi`
  sub-block rects). The .shbd payload is one byte per cell, 0xFF
  blocked / 0x00 walkable; size = width*height + 8.
* `Server/Zone/MapField.h` is now the canonical home of the
  `MapBlockInformation` struct (one per `Field`). It exposes:
    * `IsBlockedCell(cx, cy)` -- raw cell-grid lookup
    * `IsBlockedWorld(x, y)`  -- world-coord lookup with the universal
      `kMapBlockCellSize=16` world-units-per-cell scale
* `MapBlockInformation.h` is now a thin facade that just includes
  `MapField.h`; the previous duplicate class (singleton) has been
  removed to fix the ODR conflict.
* `MapNavigator::FindWay` now samples at one-cell granularity instead
  of a fixed 32-step lerp, so single-cell blockers are no longer skipped.
* `BlockingDistribute::ResolveNearby(Field&, x, y)` spirals outward by
  cell increments to find the nearest walkable world coord, capped at
  ~512 world units.


Announcements: `AnnounceSystem::Broadcast(level, text)` -- now declared
in `Server/Zone/AnnounceSystem.h`, called from LiveOpsBoosts on every
state transition.


## Recent engineering log (Feb 2026)

This is a condensed engineering changelog -- see `docs/HONEST_DISCLOSURE.md`
for the per-subsystem audit notes.

### AbState as the central effect engine
* Decoded `AbState.shn` (777 rows, 19 cols) and `SubAbState.shn`
  (2041 rows, 14 cols) directly off the data drop.
* New `Server/Zone/AbStateRuntime.{h,cpp}` central dispatcher: Resolve
  (AbState row + Strength), ResolveFromSub (SubAbState InxName direct),
  Tick (per-second action handlers), OnApply / OnRemove,
  AbsorbIncoming (shield gate), ReflectIncoming (reflect gate).
* `AbState.cpp` real ledger: Apply / ApplyAt / ApplySubByName / Remove /
  RemoveBySubInxName / DispelByCategory / Tick / Has / HasInxName /
  StatModX1k. Replace/stack rule reads `AbStateRow.Duplicate` +
  `StateGrade`.
* AbnormalState ledger lives on base `ShineObject` so mobs carry
  status states too (CC chains, debuff stacks). Player alias
  `pk->AbState()` preserved.
* 14 action constants in `BattleTunables.h::kAbAction_*`.
* `Battle::Apply` (both overloads) routes through `AbStateRuntime`
  for shield absorb and damage reflect.
* `ZoneServer::Tick` walks the unified object registry and ticks
  every ledger uniformly.
* `SkillSystem::Use`, `StateFieldTable::OnPlayerEnter`, and
  `ItemActionResolver::EffectApply` all route through
  `ApplySubByName(InxName, Strength, dur)`.
* `AbnormalStateShelter` real save/load with `AbStateSaveTypeInfo`-
  driven trigger gating (link/die/logoff).

### Damage formula pipeline
* `Server/Zone/Battle.cpp` full `BATTLESTAT` damage formula
  (linear/quadratic DEF curves, element resist, hit/dodge, crit,
  block, level-gap, MobResist, PvP scaler, variance).
* Tunables in `BattleTunables.h`: `kRawDmgMode`, `kQuadraticBias`,
  `kRawDmgScalerX1k`, `kLinearDefCapX1k`, `kDamageFloor`,
  `kBackHitMultiplierX1k`, `kFlankHitMultiplierX1k`, `kCritRollMax`,
  `kCritDamageScaler`, `kDamageVarianceX10k`, `kPvPDamageScalerX1k`,
  Roe ATK/DEF/Hit/Crit constants.

### MobSpawnSystem / handle registry
* Unified `Handle -> ShineObject` registry on ZoneServer
  (RegisterObject / UnregisterObject / FindObject / FindMob / FindNPC /
  FindPet). MobSpawnSystem and ShineNPCTable register every spawn.
* MobSpawnSystem script API: `SpawnAt`, `SpawnRectangle`, `SpawnCircle`,
  `SpawnGroup`. All Lua `cMobRegen_*` and `cGroupRegenInstance` produce
  real spawns.

### Inter-broadcast unified protocol
* `NC_INTER_BROADCAST_CMD` carries a kind-byte protocol
  (0=ChatShout, 1=WorldYell, 2=GMEvent, 3=DailyReset, 4=NpcSchedule).
* `DailyResetSink` / `NpcScheduleSink` registries route each kind to
  subscribed callbacks. WM senders prepend the kind byte.

### Subsystem real-impl sweep
* `BoothManager.{h,cpp}` -- per-zone live booth registry with
  Open/Close/Add/Remove/Buy/Find/ListNearby.
* `GuildAcademy.{h,cpp}` -- reads `GuildAcademy.shn`, applies academy
  buff via AbState only after the join-time threshold passes.
  GuildServer `GuildOf` / `JoinedMs` exposed.
* `ActionTargetTypeValidator` -- real Self/Single/Ally/Enemy/AOE/
  Cone/Line + faction gates.
* `KQServer::ForceEnd` -- real broadcast to queued participants.
* `NpcScheduleServer::Tick` -- actual `Field::AddObject`/`RemoveObject`
  on flip events.
* `Lua_cFinishKey` -- real `CharQuest::FinishKeyAdd/Has/Count` ledger.
* `Lua_cDoorAction` -- routes through `NC_MAP_DOOR_STATE_CMD`.
* `Lua_cResetAbstate` -- returns the actual remove outcome.
* `ItemUpgrade::Try` -- consumes UpResource + Luck-Stone on attempt.
* `WMClient::OnTakeItem` -- removes the matching `ShineItem` from the
  live `Inventory`.
* `ItemActionResolver::EffectApply` -- EffectActivity=4 routes
  Value->SubAbState id, Area->Strength via `ApplySubByName`.

### CharLogin / instance entry / handler table
* `CharLogin.cpp` -- real `NC_CHAR_INFO_CMD` send on world entry.
* `InstanceDungeon.cpp` -- real instance entry warp.
* `ZoneHandlers.cpp` -- full opcode dispatch table (login, logout,
  skill, attack, NPC menu/buy/sell, item-upgrade, free-stat, instance
  enter, chat, move).

### Build artifacts
* `Build/gen_vcxproj.py` re-generates the 13-project VS2010 solution
  on every change. Wrote `Fiesta.sln` + `Gamebryo.props` + 13 vcxproj
  files. Toolset `v100`, `/MT`, MBCS, Win32.
* DataReader project now ships **368 files (184 per-SHN .h/.cpp pairs)**
  under `Server/DataReader/SHN/` plus the existing readers.
* Zone project ships **36 files (17 engines + orchestrator)** under
  `Server/Zone/Engines/` and **4 files** under `Server/Zone/World/`.
* Engines load via a single `BindAllEngines(shineRoot)` call from
  `Zone/Main.cpp`, running in parallel with the legacy `Bind*`
  mega-binder chain until full migration.
