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

**What it does:** Client-side mirror tables, UI windows, skill/item/mob names, packet send/recv names from Fiesta.pdb. Used to map InxName/internal names to public names.

**Implement as:**
- Data ownership/loader mapping.
- Packet request/response handlers when applicable.
- DB read/write boundary if stateful.
- Function implemented with original-style name/module; uncertain math uses local `// VERIFY/TUNE:` constants inside the function, not external invented tuning docs.

**Original names to reuse/search:** `Fiesta.pdb, Client/ressystem/*.shn, Recv_NC_*, Send_NC_*, UI resource tables`

**Initial status:** Missing/partial; create module page and stub

