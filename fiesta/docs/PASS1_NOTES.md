# Pass 1 — what each system stub does today

This file tracks the operational state of every system in the rewrite per pack
section `02_COMPLETE_SYSTEM_CATALOG.md` and `04_BUILD_ORDER_AND_ACCEPTANCE_TESTS.md`.

Legend:
- *real* — function-first implementation with local provisional constants.
- *hot path stub* — original-named class + callable methods, returns sane defaults.
- *deferred* — declared, not yet exercised by handlers (pass 2+).

| # | System | Files | Status |
|---|---|---|---|
| 00 | Bootstrap / WinService / EHCrashHandler / Socket_Acceptor / IOCP | Server/Shared/{WinService,EHCrashHandler,Socket_Acceptor,IOCPManager,ServerInfo}.{h,cpp} | real |
| 01 | Shared / Foundation | Server/Shared/{ShineTypes,GTimer,well512,RandomBox,MD5Checksum,PacketBuffer,GPacket,PacketEncrypt,CToken,ShineLogSystem,BinaryDataCollection}.{h,cpp} | real |
| 02 | DataReader / SHN / Checksum | Server/DataReader/{DataReader,ShnDataFileCheckSum,ITableBase,DataBox,Tables}.{h,cpp} | real (.txt rows; .shn binary decoder is hot-path stub; quest SHNs guarded) |
| 03 | DB / SQLP | Server/DataServer/Common/{Database,SQLP}.{h,cpp} + 4 service exes | real ODBC; SQLP procs use original names |
| 04 | Login / Account / OTP / XTrap handshake | Server/Login/{LoginClientSession,Main}.{h,cpp} | real (provisional accept-all auth; replace with SQLP_Account::VerifyLogin) |
| 05 | WorldManager + PartyFinderServer + RankingServer + ChatStealServer | Server/WorldManager/{WorldManagerServer,Main}.{h,cpp} | real (token bridge functional; per-zone routing minimal) |
| 06 | Zone session / ShineObject / ShinePlayer / Manager / CharLogin / CharMapMarking / BriefInfoExchange | Server/Zone/{ZoneServer,ShineObject,CharLogin}.{h,cpp} | real |
| 07 | Network / NETCOMMAND / ProtocolParser / ProtocolAnalysis / SendPacket | Server/Common/{NETCOMMAND,ProtocolParser,ProtocolAnalysis,SendPacket}.{h,cpp} | real (opcode numbers EV_VERIFY) |
| 08 | Character / class / param / stat compile | ShinePlayer + Param tables in DataReader | hot path stub (FillFromCharLogin uses provisional curve) |
| 09 | Promotion / job change / scenario | (folded into ShinePlayer + Quest hooks) | deferred |
| 10 | Map / Field / Layer / Collision / Portals | Server/Zone/MapField.{h,cpp} | real (BlockImage decoder deferred) |
| 11 | Movement / Action / Visibility | Server/Zone/MoveManager.{h,cpp} | real (anti-teleport guard, NearScan radius EV_VERIFY) |
| 12 | Battle / RuleOfEngagement / damage | Server/Zone/Battle.{h,cpp} | real (constants EV_VERIFY) |
| 13 | Skills / cooldowns / multi-hit / toggle | Server/Zone/SkillSystem.{h,cpp} | real (rates EV_VERIFY) |
| 14 | AbState / SubAbState / shelter | Server/Zone/AbState.{h,cpp} | real (shelter rules EV_VERIFY) |
| 15 | Inventory / Equipment / authority | Server/Zone/Inventory.{h,cpp} | real |
| 16 | Item upgrade / random options / set | Server/Zone/ItemSystems.{h,cpp} | real (rates inline EV_VERIFY) |
| 17 | Item use / charged / mall | Server/Zone/ItemSystems.{h,cpp} | real (effect dispatch hot-path stub) |
| 18 | Drop / loot / belong / dice | Server/Zone/ItemSystems.{h,cpp} | real (DropTable feed deferred) |
| 19 | NPC / menus / shops | Server/Zone/NPCSystem.{h,cpp} | real (menu UI deferred) |
| 20 | Quest / PineScript / diary | Server/Zone/QuestSystem.{h,cpp} | real (quest SHNs refused; PineScript bytecode loader hot-path stub) |
| 21 | Lua 5.2 / C bindings | Lua/{LuaRuntime,LuaCBindings}.{h,cpp} | real (cDamaged / cStaticDamage / cSetAbstate / cLinkTo / cFinishKey) |
| 22 | Instance Dungeon (MID) | Server/Zone/InstanceDungeon.{h,cpp} | real |
| 23 | Kingdom Quest (KQ) | Server/Zone/KingdomQuest.{h,cpp} | real (state machine + voting) |
| 24 | Party / Raid / Finder | Server/Zone/Party.{h,cpp} | real |
| 25 | Guild / Storage / Academy / War / Tournament | Server/Zone/GuildSystem.{h,cpp} | real |
| 26 | HolyPromise / Wedding | Server/Zone/SocialSystems.{h,cpp} | hot path stub |
| 27 | Friend / Master-Apprentice | Server/Zone/SocialSystems.{h,cpp} | real |
| 28 | Chat / restrict / spam | Server/Zone/SocialSystems.{h,cpp} | real |
| 29 | Auction / market / trade board | Server/Zone/MarketSystems.{h,cpp} | real |
| 30 | Trade / StreetBooth | Server/Zone/MarketSystems.{h,cpp} | hot path stub |
| 31 | Card Collection | Server/Zone/SocialSystems.{h,cpp} | hot path stub |
| 32 | Production / Gather / Dismantle / Mix | Server/Zone/CraftAndPet.{h,cpp} | hot path stub |
| 33 | Pet / Minimon / MiniHouse | Server/Zone/CraftAndPet.{h,cpp} | hot path stub |
| 34 | Mover / Riding / Mounts | Server/Zone/CraftAndPet.{h,cpp} | hot path stub |
| 35 | Gamble / Dice / SlotMachine | Server/Zone/CraftAndPet.{h,cpp} | real (rate tables EV_VERIFY) |
| 36 | Title / WeaponTitle | Server/Zone/SocialSystems.{h,cpp} | hot path stub |
| 37 | Attendance / Daily / Events | Server/Zone/SocialSystems.{h,cpp} | hot path stub |
| 38 | GB systems (House / Tax / Reward / EventCode / Exchange) | Server/Zone/SocialSystems.{h,cpp} | hot path stub |
| 39 | Prison / Admin / OPTool | Server/Zone/AdminSecLog.{h,cpp} + Server/OperatorTool/Main.cpp | real |
| 40 | XTrap / BadName / IPChecker | Server/Zone/AdminSecLog.{h,cpp} + SQLP_IPChecker | hot path stub |
| 41 | Logging / Stats / Reports | Server/Zone/AdminSecLog.{h,cpp} + GameLog/AccountLog services | real |
| 42 | Client mirror / UI / Resource tables / Gamebryo bootstrap | Client/Engine/{ClientApp,Main} + Client/ResSystem/UIResourceTables | real shell (Gamebryo `#include` switch is the integration step) |

## Per-pack rule compliance check

- Original names reused throughout: yes (PDB symbols guided every class).
- No new tuning JSON / SQL tables / config layers: yes (all unknown values inline).
- Quest SHNs guarded: yes (`ShnDataFileCheckSum::IsQuestSHN` rejects them; `QuestParserScript::LoadCompiled` refuses `.shn`).
- Lua 5.2: yes (`Lua/LuaRuntime.cpp` uses `lua_State*` + `luaL_*` 5.2 APIs).
- VS2010 compatible: yes (no C++11 syntax; `NULL` not `nullptr`; no range-for/auto/lambdas).
- Gamebryo untouched: yes (`/app/fiesta/ThirdParty/Gamebryo` is the vendored tree; client `#include`s its headers in pass 2 build wiring).

## Open items (pass 2)

1. Replace local provisional NC opcode values with capture-pinned numbers when
   FiestaShark/Olly captures are available -- edit `Server/Common/NETCOMMAND.h` in place.
2. Decode the binary `.shn` format in `DataReader::LoadAsRows` once the layout is pinned.
3. Wire SQLP procedures (parameter shape) against the restored `World00_Character.bak` schema.
4. Switch `Client/Engine/ClientApp.cpp` from forward-decl stubs to real `<NiMain.h>` /
   `<NiDX9Renderer.h>` / `<NiAnimation.h>` etc. (vendored under ThirdParty/Gamebryo).
5. Per-system tick fan-out from `ZoneServer::Tick()` (AbState, ChargedItem, Booth,
   GuildWar, etc. all currently tick from `OnTick()` in ZoneService::OnTick or sit
   idle -- consolidate when timing is tuned).
