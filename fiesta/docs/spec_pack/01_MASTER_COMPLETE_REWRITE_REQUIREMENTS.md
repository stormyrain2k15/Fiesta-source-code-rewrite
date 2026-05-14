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

