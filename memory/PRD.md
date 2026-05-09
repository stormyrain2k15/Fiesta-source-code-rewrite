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

## Pass 6 (Feb 2026) — Lyra Pass 5 cleanup + functional gap closure

* Fixed Lyra Pass 5 compile blockers in `Lua/LuaCBindings.cpp`:
  `pk->Abstate()` -> `pk->AbState()`, `MapField::NameToID` -> new
  `ResolveMapByName`, `ClientSession::Send(pkt)` -> `SendPacket(...)`,
  `Well512::Get()` -> process-local `well512` seeded from GTimer.
* Added `KingdomQuest::End(const char*)` static API + `NC_KQ_END_CMD`,
  `NC_ACT_SCRIPT_MSG_CMD` opcodes so the LUA-17 / LUA-18 bindings link.
* Built the unified `Handle -> ShineObject` registry (PASS3-005).
  `ZoneServer::RegisterObject` / `UnregisterObject` / `FindObject`,
  with type-narrowing accessors `FindMob` / `FindNPC` / `FindPet`.
  `MobSpawnSystem::Tick` and `ShineNPCTable::SpawnAll` now register
  every spawn; LUA-02..LUA-04, cDamaged, cNPCVanish, cObjectCount route
  through the unified registry instead of player-only maps.
* `ItemUpgrade::Try` now consumes UpResource (one stack per attempt,
  bucket-matched against same-bucket UpLimit==0 consumables) and
  Luck-Stone (`ItemInfo.ItemUseSkill == "LuckStone"`).
* `WMClient::OnTakeItem` now removes the matching ShineItem from the
  live `Inventory` (length-tolerant body parse for legacy 8-byte form).
* Unified inter-broadcast kind protocol (kind=0..4 ChatShout / WorldYell
  / GMEvent / DailyReset / NpcSchedule). New `DailyResetSink` /
  `NpcScheduleSink` registries route each kind to subscribed callbacks;
  the WM senders now prepend the kind byte everywhere.
* Added `PacketBuffer::Remaining()` for length-tolerant body parses.

See `/app/fiesta/docs/HONEST_DISCLOSURE.md` Pass 6 section for the
full audit log of what was fixed and what remains deferred.

