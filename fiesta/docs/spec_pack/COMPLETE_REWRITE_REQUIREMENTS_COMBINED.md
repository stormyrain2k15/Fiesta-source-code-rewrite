

---

<!-- 00_README_ENGINE_HANDOFF.md -->

# Shine Engine Complete Engine Rewrite Pack — Engine Handoff

Generated: 2026-05-09 00:37:38

This replaces the thin first-pass `shine_engine_requirements` pack with a fuller rewrite spec based on:

- NA2016 server/client file layout and service PDB symbol names.
- Documentation-main SHN/Lua/GM docs.
- Existing packet-capture notes and the planned ShineShark/IOCP/Olly/IDA/Hex-Rays capture workflow.
- Current recovered source inventory.
- Official/fan-facing feature references for public names and gameplay meaning.

## Critical instructions for the implementer

1. **Reuse original names whenever possible.** Start with `function_names/full_function_symbols_filtered.csv` and `function_names/original_object_modules_from_pdb.csv` before inventing a class/function name.
2. **Do not parse or rewrite quest SHNs with unsafe tooling.** Quest handling belongs behind a guarded loader. Runtime packet/DB/Hex-Rays evidence should be used first.
3. **Function coverage comes before perfect math.** Unknown formulas are not blockers if the original-style function exists, is named correctly, and contains local tunable constants/terms that can be edited in the function body later.
4. **Do not invent extra tuning docs, JSON files, balance spreadsheets, DB tables, or config layers for unknown math.** If the original data source exists, use it. If it does not, keep provisional values inside the function itself so the rewrite stays original-shaped.
5. **Treat opcodes/packet layouts marked `VERIFY` as unproven.** Packet structs still need proof, but gameplay math can be approximated inside the correct function and tuned later.
6. **Original server evidence points to Lua 5.2, not Lua 5.4.** Use Lua 5.4 only if Josh explicitly chooses a modernized runtime for the rewrite.
7. **GitHub is source of truth.** This pack is a reconstruction guide, not an authority over the repository.

## Pack contents

- `01_MASTER_COMPLETE_REWRITE_REQUIREMENTS.md` — full system-level requirements.
- `02_COMPLETE_SYSTEM_CATALOG.md` — all systems seen in PDB/data/docs and what each does.
- `03_ORIGINAL_NAMING_AND_FUNCTION_REUSE.md` — how to reuse original naming and where to find names.
- `04_BUILD_ORDER_AND_ACCEPTANCE_TESTS.md` — ordered rewrite plan and pass/fail checks.
- `05_CAPTURE_TO_CODE_WORKFLOW.md` — how ShineShark/rat/Olly/IDA/Hex-Rays output becomes source.
- `06_DATA_TABLE_AND_DOC_CROSSWALK.md` — data files by feature family.
- `07_EXTERNAL_REFERENCE_MAP.md` — official/fan reference use rules.
- `08_FUNCTION_LOCAL_MATH_AND_NO_EXTRA_TUNING_DOCS.md` — policy for missing math/formulas: implement functions first, keep tune values in the function body, and do not invent extra balance documents.
- `function_names/*.csv` — PDB-derived full function/symbol lists, including `high_value_original_symbols_for_reuse.csv` for cleaner direct reuse.
- `manifests/*.csv` — SHN/docs/current-source inventories.
- `source_maps/*.md` — concise reconstruction maps.

## Extract counts

```json
{
  "pdb_filtered_symbols": 92563,
  "pdb_protocol_symbols": 31445,
  "original_object_modules": 621,
  "server_shns": 220,
  "client_shns": 130,
  "server_sidecar_files": 1511,
  "documentation_md_files": 91,
  "current_source_files": 61
}
```



---

<!-- 01_MASTER_COMPLETE_REWRITE_REQUIREMENTS.md -->

# Shine Engine — Complete Engine Rewrite Requirements

This is the rewrite target: rebuild the server/client-support code as an original-style Shine engine, not a hacked pile of isolated table readers. The current source is pass 1 scaffolding. This document defines the missing system surface so the implementer can rewrite with all systems accounted for.

## Non-negotiable architecture rules

- Original-style module names and function names should be reused from PDBs where possible.
- Data-driven systems must flow through a shared loader/checksum/table registry layer.
- Packet structures must be proven from PDB names + captures + live IOCP logs + Hex-Rays, not guessed from one capture.
- Missing math/formula details are not blockers for system coverage. Implement the original-style function with the best-known formula shape and local editable constants, then tune output later.
- Do not create new balance/tuning documents, JSON files, SQL tables, or config layers unless the original server/data already had that layer. Unknown values belong inside the function body as local constants/commented terms.
- DB write behavior must be copied from restored `.bak` schemas/procedures and event-specific DB write logs.
- Quest SHNs are protected; do not run generic parser/exporter against them.
- Preserve VS 2010 compatibility unless Josh explicitly upgrades a module.
- Separate `verified`, `probable`, and `placeholder` code paths.

## Required service topology

- Account DB service
- AccountLog DB service
- Character DB service
- GameLog DB service
- Login service
- WorldManager service
- Zone00-Zone04 services
- GamigoZR service/stub/launcher path
- OperatorTool/optool/admin path

## Full feature/system surface

### 00 Bootstrap / Service Host / Windows Services

Account, AccountLog, Character, GameLog, Login, WorldManager, Zone00-Zone04, GamigoZR. Owns process config, WinService lifecycle, crash/minidump/log initialization, service install/start scripts, socket acceptors, and dependencies.

**Original names / evidence anchors:** `WinService, Main, ServerInfo, ShineLogSystem, EHCrashHandler, MiniDump, Socket_Acceptor, IOCP, ServerInfo.txt, ZoneServerInfo.txt`

### 01 Shared Code / Foundation

Shared C++ runtime used across services: timers, packet buffers, random, MD5, crypto, BTree, binary collections, strings, assert/logging, option reader, and token/seed helpers.

**Original names / evidence anchors:** `GTimer, GPacket, PacketBuffer, PacketEncrypt, CToken, CGdp, MD5Checksum, BinaryDataCollection, RandomBox, well512`

### 02 Data Reader / SHN / TXT / Checksum Layer

Original-style loader layer. Must load encrypted SHN, TXT parameter tables, sidecar data, checksum files, index by key, expose CommonData/DataBox lookups, and refuse mutation of quest SHNs. Original server evidence includes DataReader, ShnDataFileCheckSum, CommonData, DataBox, SingleDataMap.

**Original names / evidence anchors:** `DataReader, ShnDataFileCheckSum, CommonData<T>, IndexedCommonData<T>, DataBox, SingleDataMap, Param tables, DefaultCharacterData.txt`

### 03 Database / SQL / Stored Procedure Layer

SQL Server-backed state. Account/AccountLog/Character/GameLog/Options/Statistics/OperatorTool databases, with packet processors and SQLP modules. Must restore schema/procedure behavior from .bak before guessing table names.

**Original names / evidence anchors:** `DataServer, Database, DBRecord, GameDBSession, GameLogSession, WMCharDBSession, SQLP_Account, SQLP_Statistics, World00_Character.bak`

### 04 Login / Account / Authentication

Client version, XTrap token, seed/cipher, account/password auth, world list, token issue for WM. Includes login/account DB and account log writes.

**Original names / evidence anchors:** `LoginClientSession, LoginAccountDBSession, OTPLogin, ClientVersionKeyInfo.txt, NC_USER_*`

### 05 WorldManager / Routing / Session Broker

Traffic cop between Login, client, Zone, DB, optool, party/guild/KQ/prison/instance/ranking servers. Owns zone allocation, character list routing, zone enter/leave, world status, and high-level social/routing systems.

**Original names / evidence anchors:** `WorldManagerServer, WMClientSession, WMZoneSession, WMLoginSession, WMCharDBSession, WMOPToolSession, SendPacket`

### 06 Zone Session / Player Object / Entity Core

Zone login, session attach, player ShineObject lifecycle, map marking, brief info, spawn/despawn, heartbeat/game time, object handles, packet dispatch.

**Original names / evidence anchors:** `ClientSession, ShineObject, ShinePlayer, CharLogin, CharMapMarking, BriefInfoExchange, Manager`

### 07 Network Protocol / Packets / Cipher

Packet struct definitions, NETCOMMAND dispatch, client/server/inter-service opcodes, encryption/decryption, serialization/deserialization, opcode table proof from captures/PDB/Hex-Rays.

**Original names / evidence anchors:** `NETCOMMAND, PROTO_NC_*, ProtocolParser, ProtocolAnalysis, PacketBuffer, SendPacket, PacketEncrypt`

### 08 Character / Class / Param / Stat Compile

Character DB load/save, param tables, level progression, class restrictions, base stats, stat distribution, equipment/buff aggregation, 1035 stat update builder.

**Original names / evidence anchors:** `CharDataLoad, CharSave, CharClass, ParameterContainer, StatDistribute, ClassName.shn, UseClassTypeInfo.shn, Param*.txt`

### 09 Promotion / Job Change / Scenario

Level 20/60/100 class changes, scenario IDs, class update, skill unlocks, stats refresh, DB writes, UI open. Leave quest SHN parsing alone; use runtime/PineScript/docs/captures.

**Original names / evidence anchors:** `ThirdPromote, ScenarioBookShelf, cClassChangeOpen, QuestFramework, QuestZone, ClassName.shn`

### 10 Map / Field / Layer / Collision / Portals

MapInfo/MapViewInfo, layers, shbd/block images, pathfinding, waypoints, town portals, links, map buffs, field level gates, zone-to-zone transfer.

**Original names / evidence anchors:** `Field, Layer, MapDataBox, MapBlockInformation, BlockImage, MapNavigator, FindWay, Link, TownPortal.shn, MapLinkPoint.shn`

### 11 Movement / Action / Visibility

Move/run/walk/stop packets, action state, target validation, near scan visibility, position replication, idle/movement noise classification.

**Original names / evidence anchors:** `MoveManager, Action, ActionTargetTypeValidator, NearScan, NC_ACT_*, BriefInfoExchange`

### 12 Combat / Rule of Engagement / Damage

Normal and skill attacks, Roe dispatch tables, physical/magic damage, hit/miss/crit/block, level gap PVE/PVP/EVP, aggro, death/regen, drops/EXP/level-up. The function surface must exist now; provisional formula values stay inside the Roe/Battle function body and can be edited later to match real-game output.

**Original names / evidence anchors:** `RuleOfEngagement, Battle, BattleObject, DamageByAngle, DamageBySoul, LevelGapDamageTable, AttackRhythm, HitMeList, DamageLvGap*.shn`

### 13 Skills / Cooldowns / MultiHit / AoE / Toggle

Active/passive skills, server skill info, area and multi-hit tables, toggle skills, skill learning, SP/HP cost, target rules, cast/recast cooldowns, skill effects feeding AbState/ItemAction/Combat.

**Original names / evidence anchors:** `Skill, CharacterSkill, SkillDataBox, DefSkillData, MultiHitTable, MultiTypeHandle, ToggleSkill.shn, ActiveSkillInfoServer.shn`

### 14 AbState / Buff / Debuff / State Engine

Abnormal state dictionaries, shelter/save rules, SubAbState stat modifications, tick damage/heal, dispel, duplicate/stack priority, save on link/death/logoff, map/item/mob states.

**Original names / evidence anchors:** `AbnormalState, AbnormalStateDictionary, AbnormalStateShelter, SubAbstatePriority, AbState.shn, SubAbState.shn, StateField.shn, StateMob.shn, StateItem.shn`

### 15 Item / Inventory / Equipment

Inventory DB/memory, equip/unequip, relocation/split/sell/buy/drop/pick, item auth, equip slot conversion, class/level restrictions, durability/endure, money, ground items.

**Original names / evidence anchors:** `Inventory, ItemTotalInformation, ShineItem*, ItemAuthority, EquipEnumChanger, ItemInfoServer.shn, ItemServerEquipTypeInfo.shn`

### 16 Item Enhancement / Upgrade / Random Options / Set Effects

Upgrade/enchant, grade options, random options, accessories, socket/enchant rates, set item/effects, weapon titles, item package and item actions.

**Original names / evidence anchors:** `ShineItemUpgrade, AccUpgradeTable, BRAccUpgradeDataBox, ItemRandomOption, SetItemData, WeaponTitle, ItemUpgrade.shn, GradeItemOption.shn`

### 17 Item Use / Premium / Charged Effects / Mall

Consumables, scrolls/stones, premium items, charged effects, charms/extenders/term extension, icon/message/deletable buff systems, ItemMall.

**Original names / evidence anchors:** `ShineItemUse, ChargedItem, ChargedItemEffectDataBox, ChargedItemEffectList, ItemMall, ItemUseEffect.shn, ChargedEffect.shn, TermExtendMatch.shn`

### 18 Drop / Loot / Dice / Belong / Reward Inventory

Mob drops, item lot inspect/analyze, drop locations, belonging/dice division, reward inventory, chest items, bind/belong rules, shine rewards.

**Original names / evidence anchors:** `ItemDropFromMob, ItemDropTable, DropItemAnalyzer, DropLocationManager, ItemLotInspector, BelongDiceTable, RewardInven, ItemsInChest`

### 19 NPC / Menus / Merchants / Shops

NPC view/actions, menus, dialog, item merchant lists, shops, sell/repurchase, NPC scheduling, server menu actor/current menu.

**Original names / evidence anchors:** `NPCManager, NPCAct, NPCAction, NPCItemList, CurrentMenu, ServerMenuActor, SellItemManager, NpcScheduleServer`

### 20 Quest / PineScript / Quest Diary

Quest framework, events, kill tracking, quest diary, PineScript parser/functions, scenario commands, quest data/dialog/species. Do not parse quest SHNs with unsafe tools; use documentation/runtime and guarded loader only.

**Original names / evidence anchors:** `Quest, QuestData, QuestParserScript, QuestFramework, QuestEvent, CharQuest, ShineQuestDiary, PineScript*`

### 21 Lua Script Runtime / C Bindings

Original Zone embeds Lua 5.2. Lua scripts drive instances/KQ/events/static damage/NPC-like scripted actions through C bindings; only use Lua 5.4 if intentionally modernized.

**Original names / evidence anchors:** `LuaScript, AIScript, ScriptScene, ScriptInterrupt, cDamaged, cStaticDamage, cSetAbstate, cLinkTo, cFinishKey`

### 22 Instance Dungeon / MID

MIDungeon/MIDServer/MIDGroup/MIDReward, role requirements, instance map spin-up/teardown, timers, objectives, static damage, rewards, entry/exit/cleanup.

**Original names / evidence anchors:** `InstanceDungeon, InstanceDungeon_util, MIDRewardDataBox, MatchInstanceDungeonServer, MIDungeon.shn, MIDServer.shn`

### 23 Kingdom Quest / KQ

Scheduled group battles, queue/recruiting/running states, KQ maps, rewards, votes, teams, contribution, death count/removal rules.

**Original names / evidence anchors:** `KingdomQuest, KQServer, KQRewardDataBox, KQContribute, KingdomQuest.shn, KingdomQuestMap.shn, KQIsVote.shn`

### 24 Party / Raid / Party Finder

Party create/join/leave/kick/break, party slots, bonus tables, raid task, party finder service in WM, party buff pulse/range.

**Original names / evidence anchors:** `PartyCreate, PartyJoin, PartyLeave, PartyKickOut, PartyBreak, PartyContainer, RaidSystem, PartyFinderServer`

### 25 Guild / Guild Storage / Academy / War / Tournament

Guild creation/membership/ranks/storage, guild academy rewards/ranks, guild grade/growth, guild war, tournament occupancy/score/skills/rewards/master buff.

**Original names / evidence anchors:** `GuildServer, GuildZone, GuildStorageManager, GuildAcademy, GuildTournamentSystem, GuildWarManager, GuildTournament*.shn`

### 26 HolyPromise / Wedding / Spouse System

Wedding proposal/application/ceremony, Blessed Garden flow, spouse summon, Power of Love buff, wedding pet/reward, party stat bonus, DB state.

**Original names / evidence anchors:** `HolyPromise, HolyPromiseServer, HolyPromiseReward.shn`

### 27 Master-Apprentice / Friend / Social

Master/apprentice relationships/rewards, friends, friend points, social/message systems, rankings/SNS docs where present.

**Original names / evidence anchors:** `Friend, FriendPointReward.shn, SNS*.shn, RankingServer`

### 28 Chat / Chat Restriction / Spam Penalty

Normal/party/guild/whisper/shout/admin chat, chat stealing/restrict in WM, spam report/penalty/rules.

**Original names / evidence anchors:** `Chat, ChatRestrict, ChatStealServer, SpamerPenaltyDataBox, SpamerReport.shn, ChatColor.shn`

### 29 Auction / Market Search / Trade Board

Auction listing/search/bid/buy costs/limits/periods, market search filters, trade board/on/off-board systems.

**Original names / evidence anchors:** `AuctionSystem, TradeBoard, TradeOnBoard, TradeOffBoard, MarketSearchInfo.shn, Auction*.shn`

### 30 Trade / Street Booth / Personal Shop

Player trade, trade misc, booth manager, booth item type matching, street booth buy/sell, booth persistence and packet flow.

**Original names / evidence anchors:** `Trade, TradeMisc, StreetBooth, StreetBoothBuy, StreetBoothSell, BoothManager, Booth.obj`

### 31 Card Collection

Monster card items, card open/register/bookmark/reward, card drop/star rates, titles/rewards, DB ack handlers.

**Original names / evidence anchors:** `CardCollection, CollectCard*.shn, UseItemCardOpen, sp_NC_COLLECT_*`

### 32 Production / Gathering / Dismantle / Mix

Production skill recipes/success, gathering, item dismantle/mix, alchemy-like stone/potion/scroll systems.

**Original names / evidence anchors:** `GatherNProduce, ProdSucRateTable, ShineItemDismantle, ItemMixData, Produce.shn, Gather.shn`

### 33 Minimon / Pets / MiniHouse

Minimon/pet data, auto-use, pet-related skills, mini-house furniture/interior/endure/animation/object effects.

**Original names / evidence anchors:** `PetData, PetProtoFunc, PetRelatedSkill, MinimonDataBox, MiniHouseInterior, MiniHouse*.shn, Minimon*.shn`

### 34 Movers / Riding / Mounts / Rare Movers

Mover main/item/ability/use skill/upgrade, riding, rare mover rates/subrates/effects.

**Original names / evidence anchors:** `MoverDataBox, ShineObjectMover, MoverUpgradeData, Mover*.shn, Riding.shn, RareMover*.shn`

### 35 Gambling / Dice / Slot Machine / God of Slot Machine

Dice Tai Sai and slot-machine systems, NPC/bet/line/card/jackpot tables, DB jackpot/win ranking.

**Original names / evidence anchors:** `GambleSystem, GambleDiceTaiSaiObject, SlotMachine, GBSM*.shn, GBDice*.shn`

### 36 Titles / CharacterTitle / WeaponTitle

Character title data/states, weapon title, title rewards/stat effects, client/server title state sync.

**Original names / evidence anchors:** `CharacterTitle, CharacterTitleZone, WeaponTitle, CharacterTitle*.shn, WeaponTitleData.shn`

### 37 Attendance / Daily / Event Rewards

Attendance schedules/rewards, daily quest timers, event attendance, GM event manager, Facebook event reward, announcement data.

**Original names / evidence anchors:** `EventAttendanceServer, DailyQuestTimer, AttendReward.shn, AttendSchedule.shn, GMEventManager, FacebookEventReward`

### 38 Gambling/Board/House/GB Systems

GB-prefixed event/house/tax/exchange/reward/slot-machine tables; treat as event/gamble/board sub-systems until runtime-labeled.

**Original names / evidence anchors:** `GBHouse.shn, GBTaxRate.shn, GBReward.shn, GBEventCode.shn, GBExchangeMaxCoin.shn`

### 39 Prison / Jail / Admin Enforcement

Prison/jail logic, admin commands, optool commands, silencing/linking/ban/jail behavior, admin level gates.

**Original names / evidence anchors:** `ShinePrison, PrisonServer, AmpersandCommand, OPToolSession, AdminLvSet.shn`

### 40 Anti-cheat / XTrap / Version / Bad Name / Security

XTrap handshake/server DLL, client version key, bad-name filter, IP checker, account auth security, spam/security punishments.

**Original names / evidence anchors:** `XTrap4Server, BadNameFilterData, Login XTrap, SQLP_IPChecker, BadNameFilter.shn`

### 41 Logging / Analytics / Statistics / Reports

AccountLog/GameLog, statistics DB, report/SPAM/GM/event logs, item drop/mob kill/hunt logs, easy log system.

**Original names / evidence anchors:** `AccountLog, GameLogSession, SQLP_Report, SQLP_Statistics, Statistics, EasyLogSys, MobHuntLog, ItemDropLog.shn`

### 42 Client Mirror / UI / Resource Tables

Client-side mirror tables, UI windows, skill/item/mob names, packet send/recv names from Shine.pdb. Used to map InxName/internal names to public names.

**Original names / evidence anchors:** `Shine.pdb, Client/ressystem/*.shn, Recv_NC_*, Send_NC_*, UI resource tables`

## Proof levels

- `PDB_CONFIRMED`: symbol/module exists in matching PDB.
- `DATA_CONFIRMED`: SHN/TXT/Lua/doc file exists.
- `RUNTIME_CONFIRMED`: ShineShark/rat/Olly/IDA capture proves packet/function/DB path.
- `HEX_CONFIRMED`: Hex-Rays pseudocode confirms logic.
- `GAMEPLAY_CONFIRMED`: official/fan-facing source confirms user-facing meaning/name.
- `VERIFY`: evidence exists but exact opcode/packet layout or final formula constants still need proof. Formula uncertainty does not block implementing the function surface.




---

<!-- 02_COMPLETE_SYSTEM_CATALOG.md -->

# Complete System Catalog

Every row below is a system family the implementer must account for. Some can start as stubs, but they should exist in the design so later source recovery has a home.

## 00 Bootstrap / Service Host / Windows Services

**What it does:** Account, AccountLog, Character, GameLog, Login, WorldManager, Zone00-Zone04, GamigoZR. Owns process config, WinService lifecycle, crash/minidump/log initialization, service install/start scripts, socket acceptors, and dependencies.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `WinService, Main, ServerInfo, ShineLogSystem, EHCrashHandler, MiniDump, Socket_Acceptor, IOCP, ServerInfo.txt, ZoneServerInfo.txt`

**Initial status:** Must implement first

## 01 Shared Code / Foundation

**What it does:** Shared C++ runtime used across services: timers, packet buffers, random, MD5, crypto, BTree, binary collections, strings, assert/logging, option reader, and token/seed helpers.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `GTimer, GPacket, PacketBuffer, PacketEncrypt, CToken, CGdp, MD5Checksum, BinaryDataCollection, RandomBox, well512`

**Initial status:** Must implement first

## 02 Data Reader / SHN / TXT / Checksum Layer

**What it does:** Original-style loader layer. Must load encrypted SHN, TXT parameter tables, sidecar data, checksum files, index by key, expose CommonData/DataBox lookups, and refuse mutation of quest SHNs. Original server evidence includes DataReader, ShnDataFileCheckSum, CommonData, DataBox, SingleDataMap.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `DataReader, ShnDataFileCheckSum, CommonData<T>, IndexedCommonData<T>, DataBox, SingleDataMap, Param tables, DefaultCharacterData.txt`

**Initial status:** Must implement first

## 03 Database / SQL / Stored Procedure Layer

**What it does:** SQL Server-backed state. Account/AccountLog/Character/GameLog/Options/Statistics/OperatorTool databases, with packet processors and SQLP modules. Must restore schema/procedure behavior from .bak before guessing table names.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `DataServer, Database, DBRecord, GameDBSession, GameLogSession, WMCharDBSession, SQLP_Account, SQLP_Statistics, World00_Character.bak`

**Initial status:** Must implement first

## 04 Login / Account / Authentication

**What it does:** Client version, XTrap token, seed/cipher, account/password auth, world list, token issue for WM. Includes login/account DB and account log writes.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `LoginClientSession, LoginAccountDBSession, OTPLogin, ClientVersionKeyInfo.txt, NC_USER_*`

**Initial status:** Must implement first

## 05 WorldManager / Routing / Session Broker

**What it does:** Traffic cop between Login, client, Zone, DB, optool, party/guild/KQ/prison/instance/ranking servers. Owns zone allocation, character list routing, zone enter/leave, world status, and high-level social/routing systems.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `WorldManagerServer, WMClientSession, WMZoneSession, WMLoginSession, WMCharDBSession, WMOPToolSession, SendPacket`

**Initial status:** Must implement first

## 06 Zone Session / Player Object / Entity Core

**What it does:** Zone login, session attach, player ShineObject lifecycle, map marking, brief info, spawn/despawn, heartbeat/game time, object handles, packet dispatch.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `ClientSession, ShineObject, ShinePlayer, CharLogin, CharMapMarking, BriefInfoExchange, Manager`

**Initial status:** Must implement first

## 07 Network Protocol / Packets / Cipher

**What it does:** Packet struct definitions, NETCOMMAND dispatch, client/server/inter-service opcodes, encryption/decryption, serialization/deserialization, opcode table proof from captures/PDB/Hex-Rays.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `NETCOMMAND, PROTO_NC_*, ProtocolParser, ProtocolAnalysis, PacketBuffer, SendPacket, PacketEncrypt`

**Initial status:** Must implement first

## 08 Character / Class / Param / Stat Compile

**What it does:** Character DB load/save, param tables, level progression, class restrictions, base stats, stat distribution, equipment/buff aggregation, 1035 stat update builder.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `CharDataLoad, CharSave, CharClass, ParameterContainer, StatDistribute, ClassName.shn, UseClassTypeInfo.shn, Param*.txt`

**Initial status:** Missing/partial; create module page and stub

## 09 Promotion / Job Change / Scenario

**What it does:** Level 20/60/100 class changes, scenario IDs, class update, skill unlocks, stats refresh, DB writes, UI open. Leave quest SHN parsing alone; use runtime/PineScript/docs/captures.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `ThirdPromote, ScenarioBookShelf, cClassChangeOpen, QuestFramework, QuestZone, ClassName.shn`

**Initial status:** Missing/partial; create module page and stub

## 10 Map / Field / Layer / Collision / Portals

**What it does:** MapInfo/MapViewInfo, layers, shbd/block images, pathfinding, waypoints, town portals, links, map buffs, field level gates, zone-to-zone transfer.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Field, Layer, MapDataBox, MapBlockInformation, BlockImage, MapNavigator, FindWay, Link, TownPortal.shn, MapLinkPoint.shn`

**Initial status:** Missing/partial; create module page and stub

## 11 Movement / Action / Visibility

**What it does:** Move/run/walk/stop packets, action state, target validation, near scan visibility, position replication, idle/movement noise classification.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `MoveManager, Action, ActionTargetTypeValidator, NearScan, NC_ACT_*, BriefInfoExchange`

**Initial status:** Missing/partial; create module page and stub

## 12 Combat / Rule of Engagement / Damage

**What it does:** Normal and skill attacks, Roe dispatch tables, physical/magic damage, hit/miss/crit/block, level gap PVE/PVP/EVP, aggro, death/regen, drops/EXP/level-up. Function coverage is required now; provisional formula constants stay inside Roe/Battle functions and are tuned later against real output.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `RuleOfEngagement, Battle, BattleObject, DamageByAngle, DamageBySoul, LevelGapDamageTable, AttackRhythm, HitMeList, DamageLvGap*.shn`

**Initial status:** Missing/partial; create module page and stub

## 13 Skills / Cooldowns / MultiHit / AoE / Toggle

**What it does:** Active/passive skills, server skill info, area and multi-hit tables, toggle skills, skill learning, SP/HP cost, target rules, cast/recast cooldowns, skill effects feeding AbState/ItemAction/Combat.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Skill, CharacterSkill, SkillDataBox, DefSkillData, MultiHitTable, MultiTypeHandle, ToggleSkill.shn, ActiveSkillInfoServer.shn`

**Initial status:** Missing/partial; create module page and stub

## 14 AbState / Buff / Debuff / State Engine

**What it does:** Abnormal state dictionaries, shelter/save rules, SubAbState stat modifications, tick damage/heal, dispel, duplicate/stack priority, save on link/death/logoff, map/item/mob states.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `AbnormalState, AbnormalStateDictionary, AbnormalStateShelter, SubAbstatePriority, AbState.shn, SubAbState.shn, StateField.shn, StateMob.shn, StateItem.shn`

**Initial status:** Missing/partial; create module page and stub

## 15 Item / Inventory / Equipment

**What it does:** Inventory DB/memory, equip/unequip, relocation/split/sell/buy/drop/pick, item auth, equip slot conversion, class/level restrictions, durability/endure, money, ground items.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Inventory, ItemTotalInformation, ShineItem*, ItemAuthority, EquipEnumChanger, ItemInfoServer.shn, ItemServerEquipTypeInfo.shn`

**Initial status:** Missing/partial; create module page and stub

## 16 Item Enhancement / Upgrade / Random Options / Set Effects

**What it does:** Upgrade/enchant, grade options, random options, accessories, socket/enchant rates, set item/effects, weapon titles, item package and item actions.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `ShineItemUpgrade, AccUpgradeTable, BRAccUpgradeDataBox, ItemRandomOption, SetItemData, WeaponTitle, ItemUpgrade.shn, GradeItemOption.shn`

**Initial status:** Missing/partial; create module page and stub

## 17 Item Use / Premium / Charged Effects / Mall

**What it does:** Consumables, scrolls/stones, premium items, charged effects, charms/extenders/term extension, icon/message/deletable buff systems, ItemMall.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `ShineItemUse, ChargedItem, ChargedItemEffectDataBox, ChargedItemEffectList, ItemMall, ItemUseEffect.shn, ChargedEffect.shn, TermExtendMatch.shn`

**Initial status:** Missing/partial; create module page and stub

## 18 Drop / Loot / Dice / Belong / Reward Inventory

**What it does:** Mob drops, item lot inspect/analyze, drop locations, belonging/dice division, reward inventory, chest items, bind/belong rules, shine rewards.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `ItemDropFromMob, ItemDropTable, DropItemAnalyzer, DropLocationManager, ItemLotInspector, BelongDiceTable, RewardInven, ItemsInChest`

**Initial status:** Missing/partial; create module page and stub

## 19 NPC / Menus / Merchants / Shops

**What it does:** NPC view/actions, menus, dialog, item merchant lists, shops, sell/repurchase, NPC scheduling, server menu actor/current menu.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `NPCManager, NPCAct, NPCAction, NPCItemList, CurrentMenu, ServerMenuActor, SellItemManager, NpcScheduleServer`

**Initial status:** Missing/partial; create module page and stub

## 20 Quest / PineScript / Quest Diary

**What it does:** Quest framework, events, kill tracking, quest diary, PineScript parser/functions, scenario commands, quest data/dialog/species. Do not parse quest SHNs with unsafe tools; use documentation/runtime and guarded loader only.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Quest, QuestData, QuestParserScript, QuestFramework, QuestEvent, CharQuest, ShineQuestDiary, PineScript*`

**Initial status:** Missing/partial; create module page and stub

## 21 Lua Script Runtime / C Bindings

**What it does:** Original Zone embeds Lua 5.2. Lua scripts drive instances/KQ/events/static damage/NPC-like scripted actions through C bindings; only use Lua 5.4 if intentionally modernized.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `LuaScript, AIScript, ScriptScene, ScriptInterrupt, cDamaged, cStaticDamage, cSetAbstate, cLinkTo, cFinishKey`

**Initial status:** Missing/partial; create module page and stub

## 22 Instance Dungeon / MID

**What it does:** MIDungeon/MIDServer/MIDGroup/MIDReward, role requirements, instance map spin-up/teardown, timers, objectives, static damage, rewards, entry/exit/cleanup.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `InstanceDungeon, InstanceDungeon_util, MIDRewardDataBox, MatchInstanceDungeonServer, MIDungeon.shn, MIDServer.shn`

**Initial status:** Missing/partial; create module page and stub

## 23 Kingdom Quest / KQ

**What it does:** Scheduled group battles, queue/recruiting/running states, KQ maps, rewards, votes, teams, contribution, death count/removal rules.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `KingdomQuest, KQServer, KQRewardDataBox, KQContribute, KingdomQuest.shn, KingdomQuestMap.shn, KQIsVote.shn`

**Initial status:** Missing/partial; create module page and stub

## 24 Party / Raid / Party Finder

**What it does:** Party create/join/leave/kick/break, party slots, bonus tables, raid task, party finder service in WM, party buff pulse/range.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `PartyCreate, PartyJoin, PartyLeave, PartyKickOut, PartyBreak, PartyContainer, RaidSystem, PartyFinderServer`

**Initial status:** Missing/partial; create module page and stub

## 25 Guild / Guild Storage / Academy / War / Tournament

**What it does:** Guild creation/membership/ranks/storage, guild academy rewards/ranks, guild grade/growth, guild war, tournament occupancy/score/skills/rewards/master buff.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `GuildServer, GuildZone, GuildStorageManager, GuildAcademy, GuildTournamentSystem, GuildWarManager, GuildTournament*.shn`

**Initial status:** Missing/partial; create module page and stub

## 26 HolyPromise / Wedding / Spouse System

**What it does:** Wedding proposal/application/ceremony, Blessed Garden flow, spouse summon, Power of Love buff, wedding pet/reward, party stat bonus, DB state.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `HolyPromise, HolyPromiseServer, HolyPromiseReward.shn`

**Initial status:** Missing/partial; create module page and stub

## 27 Master-Apprentice / Friend / Social

**What it does:** Master/apprentice relationships/rewards, friends, friend points, social/message systems, rankings/SNS docs where present.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Friend, FriendPointReward.shn, SNS*.shn, RankingServer`

**Initial status:** Missing/partial; create module page and stub

## 28 Chat / Chat Restriction / Spam Penalty

**What it does:** Normal/party/guild/whisper/shout/admin chat, chat stealing/restrict in WM, spam report/penalty/rules.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Chat, ChatRestrict, ChatStealServer, SpamerPenaltyDataBox, SpamerReport.shn, ChatColor.shn`

**Initial status:** Missing/partial; create module page and stub

## 29 Auction / Market Search / Trade Board

**What it does:** Auction listing/search/bid/buy costs/limits/periods, market search filters, trade board/on/off-board systems.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `AuctionSystem, TradeBoard, TradeOnBoard, TradeOffBoard, MarketSearchInfo.shn, Auction*.shn`

**Initial status:** Missing/partial; create module page and stub

## 30 Trade / Street Booth / Personal Shop

**What it does:** Player trade, trade misc, booth manager, booth item type matching, street booth buy/sell, booth persistence and packet flow.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Trade, TradeMisc, StreetBooth, StreetBoothBuy, StreetBoothSell, BoothManager, Booth.obj`

**Initial status:** Missing/partial; create module page and stub

## 31 Card Collection

**What it does:** Monster card items, card open/register/bookmark/reward, card drop/star rates, titles/rewards, DB ack handlers.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `CardCollection, CollectCard*.shn, UseItemCardOpen, sp_NC_COLLECT_*`

**Initial status:** Missing/partial; create module page and stub

## 32 Production / Gathering / Dismantle / Mix

**What it does:** Production skill recipes/success, gathering, item dismantle/mix, alchemy-like stone/potion/scroll systems.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `GatherNProduce, ProdSucRateTable, ShineItemDismantle, ItemMixData, Produce.shn, Gather.shn`

**Initial status:** Missing/partial; create module page and stub

## 33 Minimon / Pets / MiniHouse

**What it does:** Minimon/pet data, auto-use, pet-related skills, mini-house furniture/interior/endure/animation/object effects.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `PetData, PetProtoFunc, PetRelatedSkill, MinimonDataBox, MiniHouseInterior, MiniHouse*.shn, Minimon*.shn`

**Initial status:** Missing/partial; create module page and stub

## 34 Movers / Riding / Mounts / Rare Movers

**What it does:** Mover main/item/ability/use skill/upgrade, riding, rare mover rates/subrates/effects.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `MoverDataBox, ShineObjectMover, MoverUpgradeData, Mover*.shn, Riding.shn, RareMover*.shn`

**Initial status:** Missing/partial; create module page and stub

## 35 Gambling / Dice / Slot Machine / God of Slot Machine

**What it does:** Dice Tai Sai and slot-machine systems, NPC/bet/line/card/jackpot tables, DB jackpot/win ranking.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `GambleSystem, GambleDiceTaiSaiObject, SlotMachine, GBSM*.shn, GBDice*.shn`

**Initial status:** Missing/partial; create module page and stub

## 36 Titles / CharacterTitle / WeaponTitle

**What it does:** Character title data/states, weapon title, title rewards/stat effects, client/server title state sync.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `CharacterTitle, CharacterTitleZone, WeaponTitle, CharacterTitle*.shn, WeaponTitleData.shn`

**Initial status:** Missing/partial; create module page and stub

## 37 Attendance / Daily / Event Rewards

**What it does:** Attendance schedules/rewards, daily quest timers, event attendance, GM event manager, Facebook event reward, announcement data.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `EventAttendanceServer, DailyQuestTimer, AttendReward.shn, AttendSchedule.shn, GMEventManager, FacebookEventReward`

**Initial status:** Missing/partial; create module page and stub

## 38 Gambling/Board/House/GB Systems

**What it does:** GB-prefixed event/house/tax/exchange/reward/slot-machine tables; treat as event/gamble/board sub-systems until runtime-labeled.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `GBHouse.shn, GBTaxRate.shn, GBReward.shn, GBEventCode.shn, GBExchangeMaxCoin.shn`

**Initial status:** Missing/partial; create module page and stub

## 39 Prison / Jail / Admin Enforcement

**What it does:** Prison/jail logic, admin commands, optool commands, silencing/linking/ban/jail behavior, admin level gates.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `ShinePrison, PrisonServer, AmpersandCommand, OPToolSession, AdminLvSet.shn`

**Initial status:** Missing/partial; create module page and stub

## 40 Anti-cheat / XTrap / Version / Bad Name / Security

**What it does:** XTrap handshake/server DLL, client version key, bad-name filter, IP checker, account auth security, spam/security punishments.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `XTrap4Server, BadNameFilterData, Login XTrap, SQLP_IPChecker, BadNameFilter.shn`

**Initial status:** Missing/partial; create module page and stub

## 41 Logging / Analytics / Statistics / Reports

**What it does:** AccountLog/GameLog, statistics DB, report/SPAM/GM/event logs, item drop/mob kill/hunt logs, easy log system.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `AccountLog, GameLogSession, SQLP_Report, SQLP_Statistics, Statistics, EasyLogSys, MobHuntLog, ItemDropLog.shn`

**Initial status:** Missing/partial; create module page and stub

## 42 Client Mirror / UI / Resource Tables

**What it does:** Client-side mirror tables, UI windows, skill/item/mob names, packet send/recv names from Shine.pdb. Used to map InxName/internal names to public names.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Shine.pdb, Client/ressystem/*.shn, Recv_NC_*, Send_NC_*, UI resource tables`

**Initial status:** Missing/partial; create module page and stub




---

<!-- 03_ORIGINAL_NAMING_AND_FUNCTION_REUSE.md -->

# Original Naming and Function Reuse Guide

the implementer should not invent clean-room names first. Use the PDB-derived names as the source-name vocabulary.

## Required CSVs

- `function_names/full_function_symbols_filtered.csv` — 92563 filtered function/symbol candidates.
- `function_names/protocol_handlers_and_packet_symbols.csv` — 31445 packet/protocol-related names.
- `function_names/original_object_modules_from_pdb.csv` — 621 object/source modules recovered from PDB path strings.
- `function_names/function_names_by_system.md` — readable grouped subset.

## Naming rules

1. If a PDB symbol exists, reuse it or create a wrapper with the original name visible.
2. Preserve prefixes:
   - `sp_NC_*` = ShinePlayer/client request handler in Zone.
   - `gds_NC_*` = GameDBSession DB ack/result handler.
   - `WM*Session`, `Login*Session`, `PF_*` = service routing/session/parser families.
   - `UseEffect::*` = item-use behavior classes.
   - `Roe_*`, `normalpyRoe*`, `normalmaRoe*` = combat Rule-of-Engagement pipeline.
   - `SHND::CommonData`/`DataBox` = table/index loader layer.
3. Keep original module family names: `ShineItemEquip`, `ShineItemUse`, `MobBrain`, `KingdomQuest`, `InstanceDungeon`, `HolyPromise`, `GuildTournamentSystem`, etc.
4. For unknown packet hex values, name handlers by `NC_*` symbol until capture fills hex.
5. Do not collapse feature systems into giant god classes. Use original object module boundaries from the PDB.

## High-value original module families

- Core/shared: `DataReader`, `ShnDataFileCheckSum`, `GPacket`, `PacketEncrypt`, `IOCP`, `Socket_IOCP`, `WinService`.
- Zone object: `ShineObject`, `ShinePlayerObj`, `ShineObjectMob`, `ShineObjectPet`, `ShineObjectMover`, `BattleObject`.
- Combat: `RuleOfEngagement`, `Battle`, `DamageByAngle`, `DamageBySoul`, `LevelGapDamageTable`, `AttackRhythm`.
- Items: `ShineItem*`, `ItemAction*`, `ItemDrop*`, `ItemMall`, `ChargedItem*`, `SetItemData`, `ItemRandomOption`.
- Script/content: `LuaScript`, `PineScript*`, `QuestFramework`, `QuestEvent`, `ScenarioBookShelf`.
- Social/world: `Guild*`, `GuildAcademy*`, `GuildTournament*`, `HolyPromise*`, `Party*`, `RaidSystem`, `PartyFinderServer`.
- Events: `KingdomQuest`, `InstanceDungeon`, `KQServer`, `MatchInstanceDungeonServer`.



---

<!-- 04_BUILD_ORDER_AND_ACCEPTANCE_TESTS.md -->

# Build Order and Acceptance Tests

This order prevents the implementer from writing gameplay features before the original server framework exists.

## L0 Proof/Ingestion

**Scope:** Import this pack, PDB symbols, current recovered source, docs, data manifests. Generate per-system pages.

**Acceptance:** Every system has owner, data files, original names, proof level, and required function surface. Unknown formulas are marked `VERIFY/TUNE` inside the function body.

## L1 Foundation

**Scope:** WinService/process bootstrap, logging, crash dumps, config, IOCP sockets, packet buffer, random, time, token/cipher.

**Acceptance:** Services start/shutdown cleanly and can echo/test packets.

## L2 Data Layer

**Scope:** SHN/TXT guarded loader, checksums, registry, typed rows, CommonData/DataBox indexes.

**Acceptance:** Loads all safe non-quest data; quest files blocked by guard unless explicitly supported.

## L3 Database Layer

**Scope:** Restore .bak schema/procs, DB connectors/sessions, DB packet processors, state save boundaries.

**Acceptance:** Can load account/character/inventory/skills with no gameplay yet.

## L4 Login/WM/Zone Skeleton

**Scope:** Login handshake, WM token/world/char routing, Zone login/map sync, client session object.

**Acceptance:** Client can login, select char, enter map and idle.

## L5 Character/Stats/Items Basic

**Scope:** Character stats, inventory/equip/use minimal, item restrictions, stat packet builder.

**Acceptance:** Equip/unequip updates DB and 1035/stat packets match capture.

## L6 Map/Movement/Entity

**Scope:** Map manager, field/link/portal, entity visibility, movement and brief info.

**Acceptance:** Movement/portal/NPC approach packet clusters match capture.

## L7 Combat/Skill/AbState

**Scope:** Roe, hit/miss/crit, skills, cooldown, buffs/debuffs, death/respawn, drops/EXP.

**Acceptance:** Mob fight, skill, death, level-up handlers/functions exist with original-style names and local tunable formulas. Exact numbers can be tuned later by editing those functions.

## L8 NPC/Quest/PineScript/Lua

**Scope:** NPC menus, shops, quest framework, PineScript, Lua C bindings, scenario/job change.

**Acceptance:** Job change can be repeated with DB rollback and matches packets/DB writes.

## L9 Social/World Features

**Scope:** Party, raid, guild, academy, wedding/HolyPromise, master/apprentice, friends/chat.

**Acceptance:** Core social flows produce expected packets/DB writes.

## L10 Events/Instances/Gambling/Markets

**Scope:** KQ, MID/instances, auction/market, booths, slot machine/dice, card collection, minimon/pets, mini-house/movers.

**Acceptance:** Dungeon/static-damage/event/market handlers exist with original-style names. Exact reward/static-damage constants can be tuned later inside the owning functions.

## L11 Admin/Optool/GM/Security

**Scope:** Ampersand commands, optool sessions, prison, XTrap/version, spam/bad-name filters, logs/reports.

**Acceptance:** Admin commands gated by AdminLvSet and DB/auth; no accidental unrestricted commands.

## Do not mark complete until

- Original PDB name mapping exists for every implemented handler where available.
- Packet opcode/struct has capture evidence or is explicitly `placeholder`; formula constants may be `VERIFY/TUNE` but the owning function must exist.
- DB write lines are matched for state-changing systems.
- No quest SHN was bulk-parsed or rewritten by unsafe tools.




---

<!-- 05_CAPTURE_TO_CODE_WORKFLOW.md -->

# Capture-to-Code Workflow

## Current rewrite policy update

Runtime captures are useful validation, but they are not required before implementing the missing function surface. the implementer should implement every original-style system/function first using PDB names, docs, SHNs/TXTs, and source/module evidence. Captures/Hex-Rays are then used to tune and verify packet layouts, DB writes, and exact outputs.

Unknown gameplay math should remain inside the owning function body as local `VERIFY/TUNE` constants or terms. Do not add a separate balance document/config system just because a value is unknown.


Josh's weekend run should become source pages, not a giant packet soup.

## Evidence chain

Player action → ShineShark wire packet → rat IOCP/internal packet → Olly live call stack → IDA/Hex-Rays handler → PDB symbol → DB write → server response → client UI/state change.

## Event folder standard

```txt
EVENT_042_job_change_attempt_01/
  notes.txt
  shineshark_both_sides.txt
  rat_zone_iocp.log
  rat_wm_iocp.log
  olly_zone_callstack.log
  ida_hexrays_handler.c
  ida_xrefs.txt
  db_writes.txt
  db_before_after.txt
```

## Repetition rule

Repeat actions 3-5 times inside the same capture with idle gaps. This separates baseline noise from action-triggered packets.

## Noise rule

Do not delete high-frequency packets. Classify them as idle baseline, state tick, heartbeat, entity update, control packet, action trigger, action response, DB-backed state change, or unknown high priority.

## Dungeon/instance special run

Use immortal/static-damage capture to expose: instance create, MID role validation, map allocation, Lua/static damage path, objectives, boss spawn, reward, exit, teardown.

## Hex-Rays export rule

One file per handler/action chain. Export dispatch case, handler, close helpers, table lookups, response builder, SQL call path. Avoid one monster dump.




---

<!-- 06_DATA_TABLE_AND_DOC_CROSSWALK.md -->

# Data Table and Documentation Crosswalk

This maps data files to broad engine families. It is not the final typed-struct map; it is the starting point.

## Auction / Market

**Server SHNs:** `MarketSearchInfo.shn`

**Client SHNs:** `MarketSearchInfo.shn`

**Docs:** `SHN Documentation/AuctionCost.md`, `SHN Documentation/AuctionGroupView.md`, `SHN Documentation/AuctionLimit.md`, `SHN Documentation/AuctionPeriod.md`

## Card Collection

**Server SHNs:** `CollectCard.shn`, `CollectCardDropRate.shn`, `CollectCardGroupDesc.shn`, `CollectCardReward.shn`, `CollectCardStarRate.shn`, `View/CollectCardView.shn`

**Client SHNs:** `CollectCard.shn`, `CollectCardGroupDesc.shn`, `CollectCardReward.shn`, `CollectCardView.shn`

**Docs:** `SHN Documentation/CollectCard.md`, `SHN Documentation/CollectCardDropRate.md`, `SHN Documentation/CollectCardGroupDesc.md`, `SHN Documentation/CollectCardReward.md`, `SHN Documentation/CollectCardStarRate.md`, `SHN Documentation/CollectCardView.md`

## Card Collection; Character / Stats / Class

**Server SHNs:** `CollectCardTitle.shn`

**Client SHNs:** `CollectCardTitle.shn`

**Docs:** `SHN Documentation/CollectCardTitle.md`

## Card Collection; Mob / AI / Spawn

**Server SHNs:** `CollectCardMobGroup.shn`

**Docs:** `SHN Documentation/CollectCardMobGroup.md`

## Character / Stats / Class

**Server SHNs:** `CharacterTitleData.shn`, `CharacterTitleStateServer.shn`, `ChargedDeletableBuff.shn`, `ChargedEffect.shn`, `ClassName.shn`, `UseClassTypeInfo.shn`, `View/CharacterTitleStateView.shn`, `WeaponTitleData.shn`

**Client SHNs:** `BasicInfoTitle.shn`, `CharacterTitleData.shn`, `CharacterTitleStateView.shn`, `ChargedDeletableBuff.shn`, `ChargedEffect.shn`, `ClassName.shn`, `UseClassTypeInfo.shn`, `WeaponTitleData.shn`

**Docs:** `SHN Documentation/BasicInfoTitle.md`, `SHN Documentation/CharacterTitleData.md`, `SHN Documentation/CharacterTitleStateServer.md`, `SHN Documentation/CharacterTitleStateView.md`, `SHN Documentation/ChargedEffect.md`, `SHN Documentation/ClassName.md`

## Character / Stats / Class; Guild / Academy / Tournament

**Server SHNs:** `GuildAcademyLevelUp.shn`, `GuildLevelScoreData.shn`

## Character / Stats / Class; Master Apprentice / Social

**Docs:** `SHN Documentation/SNSRWEXP.md`

## Database / SQL / Logging

**Docs:** `database.md`

## GM / Admin / Optool / Prison

**Server SHNs:** `AdminLvSet.shn`

**Docs:** `GM_Commands.md`, `SHN Documentation/AdminLvSet.md`

## Gamble / Slot Machine

**Server SHNs:** `GBDiceDividind.shn`, `GBDiceGame.shn`, `GBDiceRate.shn`, `GBSMAll.shn`, `GBSMBetCoin.shn`, `GBSMCardRate.shn`, `GBSMCenter.shn`, `GBSMGroup.shn`, `GBSMJPRate.shn`, `GBSMLine.shn`

**Client SHNs:** `GBDiceDividind.shn`

## Gamble / Slot Machine; NPC / Dialog / Shops

**Server SHNs:** `GBSMNPC.shn`

## Gather / Produce

**Server SHNs:** `Gather.shn`, `Produce.shn`, `View/ProduceView.shn`

**Client SHNs:** `Gather.shn`, `Produce.shn`, `ProduceView.shn`

**Docs:** `SHN Documentation/ProdSucRate.md`

## Guild / Academy / Tournament

**Server SHNs:** `GuildAcademy.shn`, `GuildAcademyRank.shn`, `GuildGradeData.shn`, `GuildGradeScoreData.shn`, `GuildTournament.shn`, `GuildTournamentLvGap.shn`, `GuildTournamentOccupy.shn`, `GuildTournamentRequire.shn`, `GuildTournamentReward.shn`, `GuildTournamentScore.shn`

**Client SHNs:** `GuildTournamentRequire.shn`

**Docs:** `SHN Documentation/GuildTournament.md`

## Guild / Academy / Tournament; Master Apprentice / Social

**Server SHNs:** `GuildTournamentMasterBuff.shn`

## Item / Inventory

**Server SHNs:** `AccUpGradeInfo.shn`, `AccUpgrade.shn`, `ActionEffectItem.shn`, `BRAccUpgrade.shn`, `BRAccUpgradeInfo.shn`, `BelongDice.shn`, `BelongTypeInfo.shn`, `GradeItemOption.shn`, `ItemAction.shn`, `ItemActionCondition.shn`, `ItemActionEffect.shn`, `ItemDismantle.shn`, `ItemInfo.shn`, `ItemInfoServer.shn`, `ItemInvenDel.shn`, `ItemMix.shn`, `ItemMoney.shn`, `ItemOptions.shn`, `ItemPackage.shn`, `ItemServerEquipTypeInfo.shn`, `ItemShop.shn`, `ItemSort.shn`, `ItemUpgrade.shn`, `ItemUseEffect.shn`, `JobEquipInfo.shn`, `RandomOption.shn`, `RandomOptionCount.shn`, `SetItem.shn`, `SetItemEffect.shn`, `UpgradeInfo.shn`, `View/ItemShopView.shn`, `View/ItemViewDummy.shn`, `View/ItemViewEquipTypeInfo.shn`, `View/ItemViewInfo.shn`, `View/SetItemView.shn`

**Client SHNs:** `ActionEffectItem.shn`, `BRAccUpgradeInfo.shn`, `BelongTypeInfo.shn`, `ChrBasicEquip.shn`, `ChrCreateEquip.shn`, `GradeItemOption.shn`, `ItemAction.shn`, `ItemActionCondition.shn`, `ItemActionEffect.shn`, `ItemActionEffectDesc.shn`, `ItemDismantle.shn`, `ItemInfo.shn`, `ItemMix.shn`, `ItemMoney.shn`, `ItemShopView.shn`, `ItemViewDummy.shn`, `ItemViewEquipTypeInfo.shn`, `ItemViewInfo.shn`, `SetItemName.shn`, `UpgradeInfo.shn`

**Docs:** `SHN Documentation/ActionEffectItem.md`, `SHN Documentation/GradeItemOption.md`, `SHN Documentation/ItemAction.md`, `SHN Documentation/ItemActionCondition.md`, `SHN Documentation/ItemActionEffect.md`, `SHN Documentation/ItemActionEffectDesc.md`, `SHN Documentation/ItemInfo.md`, `SHN Documentation/ItemInfoServer.md`, `SHN Documentation/ItemUseEffect.md`, `SHN Documentation/ItemViewInfo.md`, `SHN Documentation/SetItemName.md`

## Item / Inventory; Character / Stats / Class

**Server SHNs:** `StateItem.shn`

**Client SHNs:** `ChargedIconItem.shn`, `ChargedMessageItem.shn`

**Docs:** `SHN Documentation/ChargedIconItem.md`, `SHN Documentation/ChargedMessageItem.md`

## Item / Inventory; Database / SQL / Logging

**Server SHNs:** `ItemDropLog.shn`

## Item / Inventory; Lua / Instance / KQ

**Server SHNs:** `KQItem.shn`

## Item / Inventory; Map / Field / Movement; Pet / Minimon / MiniHouse / Mover

**Server SHNs:** `MoverItem.shn`, `MoverUpgradeEffect.shn`

**Client SHNs:** `MoverItem.shn`, `MoverUpgradeEffect.shn`

## Item / Inventory; Master Apprentice / Social

**Docs:** `SHN Documentation/SNSRWITEM.md`

## Item / Inventory; NPC / Dialog / Shops

**Server SHNs:** `ItemMerchantInfo.shn`

**Docs:** `SHN Documentation/ItemMerchant.md`

## Item / Inventory; Pet / Minimon / MiniHouse / Mover

**Server SHNs:** `MinimonAutoUseItem.shn`

**Client SHNs:** `MinimonAutoUseItem.shn`

## Lua / Instance / KQ

**Server SHNs:** `KQIsVote.shn`, `KQTeam.shn`, `KQVoteMajorityRate.shn`

**Client SHNs:** `KQIsVote.shn`, `KQTeam.shn`, `KQVoteDesc.shn`, `KQVoteMajorityRate.shn`

**Docs:** `Lua Scripting/documentation.md`, `Lua Scripting/setup.md`, `SHN Documentation/MIDDesc.md`, `SHN Documentation/MIDGroup.md`, `SHN Documentation/MIDServer.md`, `SHN Documentation/MIDungeon.md`

## Map / Field / Movement

**Server SHNs:** `FieldLvCondition.shn`, `MapBuff.shn`, `MapInfo.shn`, `MapLinkPoint.shn`, `MapWayPoint.shn`, `TownPortal.shn`, `View/MapViewInfo.shn`

**Client SHNs:** `BasicInfoLink.shn`, `MapInfo.shn`, `MapLinkPoint.shn`, `MapViewInfo.shn`, `MapWayPoint.shn`, `TownPortal.shn`, `WorldMapAvatarInfo.shn`

**Docs:** `SHN Documentation/BasicInfoLink.md`, `SHN Documentation/MapInfo.md`, `SHN Documentation/MapViewInfo.md`

## Map / Field / Movement; Character / Stats / Class

**Server SHNs:** `StateField.shn`

**Docs:** `SHN Documentation/StateField.md`

## Map / Field / Movement; Character / Stats / Class; Pet / Minimon / MiniHouse / Mover

**Client SHNs:** `MoverSlotCharAni.shn`

## Map / Field / Movement; Pet / Minimon / MiniHouse / Mover

**Server SHNs:** `MoverAbility.shn`, `MoverHG.shn`, `RareMoverEachRate.shn`, `RareMoverRate.shn`, `RareMoverSubRate.shn`, `View/MoverSlotView.shn`, `View/MoverView.shn`

**Client SHNs:** `MoverAbility.shn`, `MoverHG.shn`, `MoverSlotView.shn`, `MoverView.shn`

## Map / Field / Movement; Quest / PineScript / Scenario; Lua / Instance / KQ

**Server SHNs:** `KingdomQuestMap.shn`

## Master Apprentice / Social

**Server SHNs:** `FriendPointReward.shn`

**Docs:** `SHN Documentation/SNSCT.md`, `SHN Documentation/SNSGT.md`, `SHN Documentation/SNSJC.md`, `SHN Documentation/SNSLVUP.md`, `SHN Documentation/SNSPVP.md`, `SHN Documentation/SNSVIEW.md`

## Mob / AI / Spawn

**Server SHNs:** `HairColorInfo.shn`, `HairInfo.shn`, `MobAutoAction.shn`, `MobConditionServer.shn`, `MobInfo.shn`, `MobInfoServer.shn`, `MobKillAble.shn`, `MobKillAnnounce.shn`, `MobLifeTime.shn`, `MobNoFadeIn.shn`, `MobRegenAni.shn`, `MobResist.shn`, `MobSpecies.shn`, `MobWeapon.shn`, `PupMain.shn`, `View/MobConditionView.shn`, `View/MobViewInfo.shn`

**Client SHNs:** `HairColorInfo.shn`, `HairInfo.shn`, `MobConditionView.shn`, `MobCoordinate.shn`, `MobInfo.shn`, `MobKillAnnounceText.shn`, `MobNoFadeIn.shn`, `MobViewInfo.shn`, `PupMain.shn`

**Docs:** `SHN Documentation/HairInfo.md`, `SHN Documentation/MobInfo.md`, `SHN Documentation/MobInfoServer.md`, `SHN Documentation/MobResist.md`, `SHN Documentation/MobViewInfo.md`, `SHN Documentation/MobWeapon.md`

## Mob / AI / Spawn; Character / Stats / Class

**Server SHNs:** `StateMob.shn`

**Docs:** `SHN Documentation/StateMob.md`

## Mob / AI / Spawn; Database / SQL / Logging

**Server SHNs:** `MobKillLog.shn`

## Mob / AI / Spawn; Lua / Instance / KQ

**Client SHNs:** `MobRandomIdleAni.shn`

**Docs:** `SHN Documentation/MIDWaitTime.md`

## Mob / AI / Spawn; Map / Field / Movement; Pet / Minimon / MiniHouse / Mover

**Server SHNs:** `MoverMain.shn`

**Client SHNs:** `MoverMain.shn`

## Mob / AI / Spawn; Master Apprentice / Social

**Docs:** `SHN Documentation/SNSMAIN.md`, `SHN Documentation/SNSMOBKILL.md`

## NPC / Dialog / Shops

**Server SHNs:** `NpcSchedule.shn`, `View/NPCViewInfo.shn`

**Client SHNs:** `NPCViewInfo.shn`

**Docs:** `SHN Documentation/NPCViewInfo.md`

## NPC / Dialog / Shops; Database / SQL / Logging

**Client SHNs:** `NpcDialogData.shn`

## NPC / Dialog / Shops; Quest / PineScript / Scenario; Database / SQL / Logging

**Server SHNs:** `QuestDialog.shn`

**Client SHNs:** `QuestDialog.shn`

## Network / Protocol / IOCP

**Server SHNs:** `EnchantSocketRate.shn`, `MsgWorldManager.shn`

## Party / Raid / Party Finder

**Server SHNs:** `PartyBonusByLvDiff.shn`, `PartyBonusByMember.shn`, `PartyBonusLimit.shn`

## Pet / Minimon / MiniHouse / Mover

**Server SHNs:** `MiniHouse.shn`, `MiniHouseDummy.shn`, `MiniHouseEndure.shn`, `MiniHouseFurniture.shn`, `MiniHouseFurnitureObjEffect.shn`, `MiniHouseObjAni.shn`, `MinimonInfo.shn`, `Riding.shn`

**Client SHNs:** `MiniHouse.shn`, `MiniHouseEndure.shn`, `MiniHouseFurniture.shn`, `MiniHouseFurnitureObjEffect.shn`, `MiniHouseObjAni.shn`, `MinimonInfo.shn`, `Riding.shn`

## Quest / PineScript / Scenario

**Server SHNs:** `QuestData.shn`, `QuestScript.shn`, `QuestSpecies.shn`

**Client SHNs:** `QuestData.shn`

**Docs:** `SHN Documentation/QuestData.md`, `SHN Documentation/QuestSpecies.md`

## Quest / PineScript / Scenario; Lua / Instance / KQ

**Server SHNs:** `KingdomQuest.shn`, `KingdomQuestRew.shn`

**Client SHNs:** `KingdomQuestDesc.shn`

## Security / Filters / Anti-spam

**Server SHNs:** `BadNameFilter.shn`, `SpamerPenalty.shn`, `SpamerPenaltyRule.shn`, `SpamerReport.shn`

**Client SHNs:** `BadNameFilter.shn`

## Skill / Battle / Roe

**Server SHNs:** `ActiveSkill.shn`, `ActiveSkillGroup.shn`, `ActiveSkillInfoServer.shn`, `AreaSkill.shn`, `DamageLvGapEVP.shn`, `DamageLvGapPVE.shn`, `DamageLvGapPVP.shn`, `MultiHitType.shn`, `PassiveSkill.shn`, `ToggleSkill.shn`, `View/ActiveSkillView.shn`, `View/PassiveSkillView.shn`

**Client SHNs:** `ActiveSkill.shn`, `ActiveSkillGroup.shn`, `ActiveSkillView.shn`, `DamageEffect.shn`, `DamageSoundInfo.shn`, `PassiveSkill.shn`, `PassiveSkillView.shn`

**Docs:** `SHN Documentation/ActiveSkill.md`, `SHN Documentation/ActiveSkillServer.md`, `SHN Documentation/ActiveSkillView.md`

## Skill / Battle / Roe; Character / Stats / Class

**Server SHNs:** `AbState.shn`, `ActionEffectAbState.shn`, `GroupAbState.shn`, `PSkillSetAbstate.shn`, `SubAbState.shn`, `View/AbStateView.shn`

**Client SHNs:** `AbState.shn`, `AbStateMsg.shn`, `AbStateView.shn`, `SubAbState.shn`

**Docs:** `SHN Documentation/AbState.md`, `SHN Documentation/AbStateView.md`, `SHN Documentation/ActionEffectAbState.md`, `SHN Documentation/SubAbState.md`

## Skill / Battle / Roe; Character / Stats / Class; Database / SQL / Logging

**Server SHNs:** `AbStateSaveTypeInfo.shn`

## Skill / Battle / Roe; Guild / Academy / Tournament

**Server SHNs:** `GuildTournamentSkill.shn`

**Client SHNs:** `GuildTournamentSkill.shn`, `GuildTournamentSkillDesc.shn`

## Skill / Battle / Roe; Map / Field / Movement; Pet / Minimon / MiniHouse / Mover

**Server SHNs:** `MoverUseSkill.shn`

**Client SHNs:** `MoverUseSkill.shn`

## Skill / Battle / Roe; Mob / AI / Spawn; Character / Stats / Class

**Server SHNs:** `MobAbStateDropSetting.shn`

## Unclassified / shared / STL / helper

**Server SHNs:** `ActionRangeFactor.shn`, `ActionViewInfo.shn`, `AnnounceData.shn`, `AttendReward.shn`, `AttendSchedule.shn`, `BMP.shn`, `ChatColor.shn`, `ColorInfo.shn`, `DiceDividind.shn`, `DiceGame.shn`, `DiceRate.shn`, `ErrorCodeTable.shn`, `FaceInfo.shn`, `GBBanTime.shn`, `GBEventCode.shn`, `GBExchangeMaxCoin.shn`, `GBHouse.shn`, `GBJoinGameMember.shn`, `GBReward.shn`, `GBTaxRate.shn`, `GTIBreedSubject.shn`, `GTIGetRate.shn`, `GTIGetRateGap.shn`, `GTIServer.shn`, `GTWinScore.shn`, `LCGroupRate.shn`, `LCReward.shn`, `MysteryVaultServer.shn`, `PupCase.shn`, `PupCaseDesc.shn`, `PupFactorCondition.shn`, `PupMind.shn`, `PupPriority.shn`, `PupServer.shn`, `RaceNameInfo.shn`, `ReactionType.shn`, `SetEffect.shn`, `ShineReward.shn`, `SingleData.shn`, `TermExtendMatch.shn`, `View/ActionViewInfo.shn`, `View/EffectViewInfo.shn`, `View/GTIView.shn`, `View/PupView.shn`, `WeaponAttrib.shn`

**Client SHNs:** `ActionEffectInfo.shn`, `ActionViewInfo.shn`, `AttendReward.shn`, `BasicInfoFind.shn`, `BasicInfoFindUI.shn`, `BasicInfoFindWord.shn`, `BasicInfoHelp.shn`, `BasicInfoTip.shn`, `BasicInfoTipCycle.shn`, `ChatColor.shn`, `ColorInfo.shn`, `DiceDividind.shn`, `EffectViewInfo.shn`, `FaceCutMsg.shn`, `FaceInfo.shn`, `FontSet.shn`, `GBHouse.shn`, `GTIView.shn`, `MHEmotion.shn`, `PupView.shn`, `RaceNameInfo.shn`, `ScriptMsg.shn`, `SetEffect.shn`, `SingleData.shn`, `SlanderFilter.shn`, `TermExtendMatch.shn`, `TermExtendMatchGroupDesc.shn`, `TextData.shn`, `TextData2.shn`, `TextData3.shn`, `UpEffect.shn`, `WeaponAttrib.shn`

**Docs:** `SHN Documentation/ActionRangeFactor.md`, `SHN Documentation/ActionViewInfo.md`, `SHN Documentation/AnnounceData.md`, `SHN Documentation/AttendReward.md`, `SHN Documentation/AttendSchedule.md`, `SHN Documentation/BasicInfoTip.md`, `SHN Documentation/BasicInfoTipCycle.md`, `SHN Documentation/GTIBreedSubJect.md`, `SHN Documentation/GTIGetRate.md`, `SHN Documentation/GTIGetRateGap.md`, `SHN Documentation/GTIServer.md`, `SHN Documentation/GTIView.md`, `SHN Documentation/MHEmotion.md`, `SHN Documentation/ScriptMsg.md`, `SHN Documentation/SetEffect.md`, `server_and_client_setup.md`

## Wedding / HolyPromise

**Server SHNs:** `HolyPromiseReward.shn`




---

<!-- 07_EXTERNAL_REFERENCE_MAP.md -->

# External Reference Map

Use external references only to fill public-facing names/meanings, not to override packet/PDB/runtime truth.

## Official Gamigo pages to use for public feature names

- Shine homepage/about: confirms high-level systems such as quests, KQ, guilds, master/apprentice, wedding, daily quest/premium reward loops.
- Character Classes: public names and role descriptions for Fighter, Cleric, Archer, Mage, Trickster, Crusader.
- Class Change: public job-change levels and NPC/map flow: level 20, 60, 100.
- Dungeons: public dungeon names and level ranges.
- Kingdom Quests: KQ list, levels, scheduling/join/death behavior.
- Guilds: guild ranks/storage/academy public behavior.
- Master Apprentice: level-gap relationship and rewards.
- Wedding Guide: proposal/ring/application/ceremony/spouse buff/reward behavior.
- Realm of the Gods: later-map/event/class/level context; use only if target build includes it.

## Fan/community references

- Shine Wiki / Shine-Wiki: use for skill names, item names, mob names, card names, production/gathering names, and public stats when internal InxName is cryptic.
- RageZone / EPVP / Shine Heroes: use for emulator behavior comparisons, packet folklore, and tool references, but mark as community evidence unless matched by PDB/capture/data.

## Rule

Public name sources help translate `InxName` to human meaning. They do not prove packet layouts, formulas, or DB behavior.



---

<!-- 08_FUNCTION_LOCAL_MATH_AND_NO_EXTRA_TUNING_DOCS.md -->

# Function-Local Math Policy — No Invented Tuning Documents

This rewrite should look like original Shine server source, not a modern balance framework layered on top of it.

## Core rule

If a system/function exists in the original game, the implementer must implement the function even when the exact formula constants are not known yet.

Unknown math is handled inside the function body:

- use the original-style function/module/class name from PDBs when available;
- place the provisional formula directly in the owning function;
- keep uncertain constants as local `const` values or local variables;
- mark them with `// VERIFY/TUNE:` comments;
- tune them later by editing that function, not by creating new external tuning files.

## Do not invent new tuning layers

Do not create any of these unless original evidence shows the real server had them:

- new balance markdown docs;
- new JSON/YAML/INI tuning files;
- new SQL balance tables;
- new generated spreadsheets;
- new global formula registry;
- new script-only formula override layer.

Original Shine already has many data sources: SHN, TXT, Lua, PineScript, SQL, service config, and hardcoded C++ functions. Use those original-style locations only.

## What belongs in code

These can be provisional inside the owning function:

- Roe damage constants;
- hit/miss/crit/block curves;
- stat compile coefficients;
- level-gap modifiers;
- item upgrade chance math;
- random option roll weights;
- set-effect aggregation terms;
- premium/charm/extender modifiers;
- mob AI weights;
- dungeon reward/static-damage values;
- class/job stat recalculation terms.

## Example

```cpp
int normalpyRoe_damage(/* original-ish args */)
{
    // VERIFY/TUNE: provisional physical damage curve. Do not move to external config unless original evidence is found.
    const int kMinimumDamage = 1;
    const int nAttack = Roe_getattack(/*...*/);
    const int nDefense = Roe_defendpower(/*...*/);

    int nDamage = (nAttack * nAttack) / max(1, nAttack + nDefense);
    if (nDamage < kMinimumDamage)
        nDamage = kMinimumDamage;

    return nDamage;
}
```

## Acceptance rule

A system is acceptable for Pass 2 if:

1. the function/class/module exists with original-style naming;
2. the function is in the correct subsystem;
3. it uses original SHN/TXT/SQL/Lua data when known;
4. any unknown math is implemented locally with `VERIFY/TUNE` comments;
5. no invented external tuning document/config layer was created.

Exact numeric output can be tuned later. Missing function coverage cannot.
