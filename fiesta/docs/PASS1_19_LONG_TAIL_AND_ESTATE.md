# Pass 1.19 -- Long-tail data ingest + Estate / Marriage / Expedition systems

**User trigger (verbatim):**
> "I want you to keep going through all of the files and getting them being
> used by the server. ... Also estates are part of the mini house/vendor
> system. ... Marriage system. Guild system expedition system academy
> system which is part of the guild system."

**Goal:** every data file under `Data\Shine[-1]\*.shn` and
`Data\World\*.txt` must be loaded and reachable by the runtime. The
estate, marriage, and expedition systems must exist as runtime objects
backed by those tables.

## What changed

### 1. `ExtendedTables.h / .cpp` (new)

Typed accessors for the 110 SHN files that Pass 1.18 didn't already
group. Each table loads once at boot via `BindAllExtendedTables()`,
projects the columns it needs into a row POD, and indexes by primary
key. Coverage by group:

| Group                           | SHNs covered                                                                   |
|---------------------------------|--------------------------------------------------------------------------------|
| AccUpgradeTables                | AccUpgrade, AccUpGradeInfo, BRAccUpgrade, BRAccUpgradeInfo                     |
| ActionExtraTables               | ActionEffectAbState, ActionViewInfo, ActiveSkillGroup                          |
| AdminLvSet / AnnounceData / BMP | AdminLvSet, AnnounceData, BMP                                                  |
| BelongTables                    | BelongDice, BelongTypeInfo                                                     |
| CharTitleStateServerTable       | CharacterTitleStateServer                                                      |
| ChatColorTable                  | ChatColor                                                                      |
| ColorInfoTables                 | ColorInfo, HairColorInfo                                                       |
| CollectExtrasTables             | CollectCardDropRate / GroupDesc / MobGroup / StarRate / Title                  |
| DiceTables                      | DiceGame, DiceRate, DiceDividind                                               |
| EnchantSocketTable / ErrorCodeTable | EnchantSocketRate, ErrorCodeTable                                          |
| EstateExtraTables               | MiniHouseDummy, MiniHouseEndure, MiniHouseFurnitureObjEffect, MiniHouseObjAni  |
| FieldLvCondition / FriendPointReward / SetItemEffect | FieldLvCondition, FriendPointReward, SetItemEffect    |
| GroupAbStateTable               | GroupAbState                                                                   |
| GBTables                        | GBBanTime, GBExchangeMaxCoin, GBHouse, GBTaxRate, GBJoinGameMember, GBDice*, GBSM*, GBEventCode, GBReward |
| GTITables                       | GTIBreedSubject, GTIGetRate, GTIGetRateGap, GTIServer, GTWinScore              |
| GuildAcademyExtraTables         | GuildAcademyLevelUp, GuildAcademyRank, GuildGradeScoreData, GuildLevelScoreData|
| GuildTournamentExtraTables      | GuildTournamentLvGap / MasterBuff / Occupy / Require / Score / Skill           |
| ItemExtraTables                 | ItemDismantle, ItemDropLog, ItemInvenDel, ItemMerchantInfo, ItemMix, ItemMoney, ItemOptions, ItemPackage, ItemServerEquipTypeInfo, ItemShop, ItemSort, ItemUseEffect, JobEquipInfo |
| KQExtraTables                   | KQIsVote, KQItem, KQTeam, KQVoteMajorityRate                                   |
| LuckyCoinTables                 | LCGroupRate, LCReward                                                          |
| MapExtraTables                  | MapBuff, MapLinkPoint, MapWayPoint                                             |
| MarketSearchInfoTable           | MarketSearchInfo                                                               |
| MinimonTables                   | MinimonInfo, MinimonAutoUseItem                                                |
| MobExtraTables                  | MobAbStateDropSetting, MobConditionServer, MobKillAble, MobKillAnnounce, MobKillLog, MobNoFadeIn, MobRegenAni |
| MoverExtraTables                | MoverHG, MoverUpgradeEffect, MoverUseSkill, RareMoverEachRate, RareMoverSubRate|
| MsgWorldManager / MultiHitType / MysteryVault / NpcSchedule | MsgWorldManager, MultiHitType, MysteryVaultServer, NpcSchedule  |
| PSkillSetAbstate / QuestData / Attendance | PSkillSetAbstate, QuestData, AttendReward, AttendSchedule           |

After this pass, **199 / 199** SHN files in the data drop are wired into
typed accessors.

### 2. `EstateSystem.h / .cpp` (new)

Mini-house ownership runtime. Each character owns at most one estate;
the deed item creates the room and seeds endure from
`MiniHouseEndure[Tier]`. Furniture placement validates against
`MiniHouseDummy[HouseID]`, hourly decay runs in `Tick(uiNowMs)`, and
flipping vendor mode hands the player off to the existing
`StreetBooth::Open` / `Close` path so the estate doubles as a personal
shop. Furniture auras (`MiniHouseFurnitureObjEffect`) and animation
loops (`MiniHouseObjAni`) are looked up via `EstateExtraTables`.

### 3. `MarriageSystem.h / .cpp` (new)

`HolyPromise` lifecycle backed by `MarriageRecord`: PROPOSED ->
ENGAGED -> MARRIED -> DIVORCED. Wedding ceremony is gated on the
`Wedding.ps` script (loaded via PineScriptTable expansion below).
Promise-count anniversary tick consults `HolyPromiseReward.shn` (already
wired in MiscTables) for the gift bag; a 30-minute cooldown gates
`Summon`.

### 4. `ExpeditionSystem.h / .cpp` (new)

Multi-party raid: an `ExpeditionRec` holds up to **6 sub-parties**,
each a `Party` from `Party.h`. Loot modes FREE / MASTER / TURN; in
MASTER mode the leader gets the drop, in TURN mode a round-robin cursor
walks across the sub-parties. A master-buff field lines up with
`GuildTournamentMasterBuff` so expeditions formed for guild events can
inherit the data-driven buff.

### 5. `GuildSystem.cpp` updates

`GuildTournamentSystem::LvGapMul` and `OccupyPoints` now query
`GuildTournamentExtraTables` instead of the hand-tuned constants.
`GuildAcademy::GrantApprenticeReward` reaches into
`GuildAcademyExtraTables` for the level-up row.

### 6. `WorldTables.h / .cpp` updates

* `LoadAllPineScripts(rRoot)` -- walks `World/PineScript.txt`, splits
  each entry into (category, name) and feeds it to
  `ScenarioBookShelf::Load`. Always-on `Wedding.ps`,
  `Guild/GuildTournament.ps`, and `Guild/GuildTournament1.ps` are
  appended even if absent from the manifest.
* `KarenAttackTable` -- ingests `World/Karen.txt` (boss attack
  sequence; one column "AttSeq", indexed by Order).
* `MobChatTable` -- ingests `World/MobChat.txt` (PIECE / ATTACK /
  DAMAGED / DEAD / HELPMAIN / HELPSUB) and offers a `Pick(bucket,
  mobIndex)` weighted-sample helper.

After this pass, **48 / 48** `World/*.txt` files in the data drop are
loaded by the zone server.

### 7. `Zone/Main.cpp` boot wiring

```
DataBox::Get().LoadAll(reader);
ShnRegistry::Get().LoadAll(reader.GetRoot());
BindAllGroupTables();
BindAllMiscTables();
BindAllMoreTables();
BindAllExtendedTables();        // NEW
LoadAllPineScripts(...);        // NEW
KarenAttackTable::Get().Load(...);   // NEW
MobChatTable::Get().Load(...);       // NEW
ChargedEffectTable::Get().Load(...);
ChargedEffectManager::Get().LoadDeletable(...);
```

Tick adds:

```
EstateServer::Get().Tick(::GetTickCount64());
ExpeditionSystem::Get().Tick();
```

## Coverage matrix (post-pass)

| Drop folder              | Files | Wired |
|--------------------------|-------|-------|
| `Data\Shine-1\*.shn`     | 199   | 199   |
| `Data\Shine\World\*.txt` | 48    | 48    |
| `Data\Shine\ScenarioBookShelf\<cat>\*.ps` | (manifest-driven) | via PineScript + always-on Wedding/Guild |
| `Data\Shine\MobAttackSequence\*.txt` | (per-mob)        | per-MobInfo (Pass 1.10)        |
| `Data\Shine\MobBehaviorDescript\*.txt` | (per-mob)      | per-MobInfo (Pass 1.10)        |
| `Data\Shine\MobRegen\*.txt` / `MobRoam\*.txt` / `MobSetting\*.txt` | (per-zone) | per-zone (Pass 1.10) |
| `Data\Shine\NPCItemList\*.txt` / `AbState\*.txt` / `AreaBMP\*.txt` / `BlockInfo\*.txt` / `View\*.txt` | (per-zone or per-NPC) | already loaded through per-zone parsers (Pass 1.10) |
| `Data\Shine\LuaScript\**\*.lua` | (per-system)            | LuaRuntime (Pass 1.5)          |

## Known follow-up

* AbState integration for `EstateExtraTables::FurnEffRow` aura
  application and `ExpeditionSystem::ApplyMasterBuff` propagation is a
  no-op stub; the abstate dispatcher needs a hook in
  `ZoneServer::Tick`.
* `StreetBoothSell::ListInBooth` / `BuyFromBooth` should consult the
  EstateRec to surface the listing under the owner's estate when
  vendor mode is open. Plumbing exists via `StreetBooth::Open(owner,
  title)` but the cross-link is informational right now.
* CharDB persistence for `MarriageRecord` and `EstateRec` (we need
  `tEstate`, `tEstateFurniture`, `tHolyPromise` writebacks via
  `CharDBClient`). Schemas are already declared in
  `DBSchemaConstants.h`; the SQLP routes are next.

No "Elle AI" hooks were re-introduced. No client version-check was
re-introduced. No standalone `OperatorTool` or `GamigoZR` was
re-introduced.
