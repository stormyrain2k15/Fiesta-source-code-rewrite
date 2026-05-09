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

