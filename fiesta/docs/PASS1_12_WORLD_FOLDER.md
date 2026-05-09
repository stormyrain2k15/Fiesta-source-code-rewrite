# Pass 1.12 -- World/ Folder Done Right

## What I had wrong before reading every file in `World/`

* Treated soul-stone tier / heal / cap / price as my own made-up table.
  In reality they are per-(class, level) columns inside
  `Param<Class>Server.txt` (22 files, ~125 rows each).
* Knew of only ~3 NPC roles. Real roster is **10 roles** plus 14 merchant
  sub-types via `RoleArg0`.
* Hadn't surfaced the ChrCommon free-stat tables (`FreeStatStrTable`,
  `FreeStatIntTable`, `FreeStatDexTable`, `FreeStatConTable`,
  `FreeStatMenTable`) -- 181 rows each, defining what each STR/INT/DEX/
  CON/MEN point actually grants.
* Had `Quest.txt` as parsed-but-unused string rows. It's actually 6
  sub-tables wired together by `QuestHandle`: Header / Reward / Looting
  / Hunting / Produce / Meeting.
* Hadn't loaded any of: `ExpRecalculation`, `RecallCoord`,
  `DamageByAngle`, `DamageBySoul`, `ItemUseFunction`,
  `RandomOptionTable`, `ItemDropGroup`, `PineScript`,
  `SubLayerInteract`, `NPCAction`, `MiscDataTable`.

## What this pass adds

### `Server/Zone/ClassParamTable.{h,cpp}`

Loads all 22 `World/Param<Class>Server.txt` files. Per (class, level) row
it surfaces the full 35-column shape:

```
Level Strength Constitution Intelligence Wisdom Dexterity MentalPower
SoulHP MAXSoulHP PriceHPStone
SoulSP MAXSoulSP PriceSPStone
AtkPerAP DmgPerAP
MaxPwrStone NumPwrStone PricePwrStone PwrStoneWC PwrStoneMA
MaxGrdStone NumGrdStone PriceGrdStone GrdStoneAC GrdStoneMR
PainRes RestraintRes CurseRes ShockRes
MaxHP MaxSP CharTitlePt SkillPwrPt JobChangeDmgUp
```

Plus a class-name <-> enum / file mapping (`SC_FIGHTER` ..
`SC_SENTINEL`, 27 classes). The enum mirrors `QuestParser.txt`.

Convenience getters used by `SoulStoneSystem` / `PowerGuardStoneSystem`:

```
SoulHpHealForLevel / SoulSpHealForLevel        -- SoulHP / SoulSP
SoulHpCapForLevel  / SoulSpCapForLevel         -- MAXSoulHP / MAXSoulSP
SoulHpPriceForLevel / SoulSpPriceForLevel      -- PriceHPStone / PriceSPStone
PwrStoneCapForLevel / PerUseForLevel / PriceForLevel / WcForLevel / MaForLevel
GrdStoneCapForLevel / PerUseForLevel / PriceForLevel / AcForLevel / MrForLevel
```

### `Server/Zone/SoulStoneSystem.{h,cpp}` -- rewritten on real data

* Heal amount, cap, vendor price now come from `ClassParamTable` --
  no fake constants left.
* `Grant(rC, class, level, bIsHp, qty)` clamps to the per-(class, level)
  cap (`MAXSoulHP` / `MAXSoulSP`).
* `Use` returns `SS_USE_NO_PARAM` if the class file isn't loaded for the
  player's (class, level).

### `Server/Zone/PowerGuardStoneSystem.{h,cpp}` -- new

Same shape as soul stones, but bumps WC+MA (Power Stone) or AC+MR (Guard
Stone) for `kStoneBuffSec` (default 1800s = 30 min) per stone consumed.
Independent cooldowns. Consumes from `MaxPwrStone` / `MaxGrdStone` caps.

`PowerGuardStoneVendor::IsVendor` checks for `Role=Merchant` with
`RoleArg0` in {`PowerStone`, `GuardStone`, `Stone`}. Buy debits Vis at
`PricePwrStone` / `PriceGrdStone`.

### `Server/Zone/NPCRole.{h,cpp}` -- the real role roster

```cpp
enum eNpcRole {
    NPC_ROLE_CLIENT_MENU, NPC_ROLE_NPC_MENU, NPC_ROLE_QUEST,
    NPC_ROLE_STORE,       NPC_ROLE_MERCHANT, NPC_ROLE_GATE,
    NPC_ROLE_ID_GATE,     NPC_ROLE_MODE_ID_GATE,
    NPC_ROLE_RANDOM_GATE, NPC_ROLE_GUARD
};
enum eMerchantKind {
    MK_WEAPON, MK_WEAPON_TITLE, MK_ITEM, MK_SKILL, MK_GUILD,
    MK_SOUL_STONE, MK_POWER_STONE, MK_GUARD_STONE, MK_GENERIC_STONE,
    MK_MOUNT, MK_PET, MK_COSTUME, MK_TITLE, MK_ITEM_CRAFT
};
```

`NPCRole::ParseRole` / `ParseMerchantKind` resolve the strings out of
`ShineNPCRow.kRole` and `kRoleArg0`.

### `Server/Zone/ChrCommonTable.{h,cpp}` -- new

Loads the 7 sub-tables of `World/ChrCommon.txt`:

* `Common`             : key/value engine constants (RunSpeed, WalkSpeed,
                         AttackSpeed, ShoutLevel, ShoutDelay, LevelLimit,
                         MaxExpBonus, RestExpRate, MinNeedTime,
                         DefaultBonusTime, IntervalTime, AddBuffTime,
                         MaxBuffTime, LostExpLevel, **MaxFreeStat**).
* `StatTable`          : per-level `NextExp` + the 30-column class checksum row.
* `FreeStat{Str,Int,Dex,Con,Men}Table` : 181 rows each, with named columns
  (WC, MA, MaxSP, ToHit, ToBlock, AC, MaxHP, MR, ...). Convenience
  accessors `WCPerStr / MAPerInt / SPPerInt / HitPerDex / BlockPerDex /
  ACPerCon / HPPerCon / MRPerMen` read named columns by header.

This replaces the placeholder per-stat constants in `BattleTunables.h`
for character math. (Tunables remain authoritative for non-character
formulas like crit roll, variance, level-gap clamp.)

### `Server/Zone/WorldTables.{h,cpp}` -- new

Single TU containing typed loaders for the remaining World/ files:

| Class | File | Tables |
|---|---|---|
| `QuestTable` | Quest.txt | Header / Reward / Looting / Hunting / Produce / Meeting |
| `ExpRecalcTable` | ExpRecalculation.txt | ByPartyMem / ByLevelDiff |
| `RecallCoordTable` | RecallCoord.txt | RecallPoint |
| `DamageByAngleTable` | DamageByAngle.txt | Chr / Mob (with linear interpolation) |
| `DamageBySoulTable` | DamageBySoul.txt | 8x8 DemandSoul x SoulNN matrix |
| `ItemUseFunctionTable` | ItemUseFunction.txt | item -> UseFunction dispatch name |
| `RandomOptionTable` | RandomOptionTable.txt | drop random option ranges |
| `ItemDropGroupTable` | ItemDropGroup.txt | named group -> item + 16 upgrade chances |
| `PineScriptTable` | PineScript.txt | server-load .ps script list |
| `SubLayerInteractTable` | SubLayerInteract.txt | SendMyBrief / CanAttack visibility matrices |
| `NPCActionTable` | NPCAction.txt | NPCCondition rule list |

## Files added in this pass

```
Server/Zone/ClassParamTable.{h,cpp}       (NEW -- 22 class param files)
Server/Zone/SoulStoneSystem.{h,cpp}       (rewritten -- data-driven)
Server/Zone/SoulStoneVendor.{h,cpp}       (rewritten -- prices from ClassParamTable)
Server/Zone/PowerGuardStoneSystem.{h,cpp} (NEW)
Server/Zone/NPCRole.{h,cpp}               (NEW -- 10 roles + 14 merchant kinds)
Server/Zone/ChrCommonTable.{h,cpp}        (NEW -- 7 sub-tables)
Server/Zone/WorldTables.{h,cpp}           (NEW -- 11 typed loaders)

docs/PASS1_12_WORLD_FOLDER.md             (this doc)
```

## Project totals

* C++ source/header files: **190**
* All combat tunables centralised in `Server/Zone/BattleTunables.h`
* All soul-stone / power-stone / guard-stone tunables now data-driven
  from `World/Param<Class>Server.txt`
* All free-stat per-point gains data-driven from `World/ChrCommon.txt`

## Open / next pass

* Wire `ClassParamTable` into character login: when a character logs
  into a zone, look up the (class, level) row and use its `MaxHP` /
  `MaxSP` / stat bases to fill `RAWCHARSTAT`.
* Replace `BattleTunables` per-stat fallbacks (`kHpPerEND` etc.) with
  reads from `ChrCommonTable::HPPerCon / SPPerInt / ...`.
* Wire `ItemUseFunctionTable` into `Inventory::UseItem` dispatch.
* Wire `RandomOptionTable` + `ItemDropGroupTable` into the drop
  resolver.
* Wire `RecallCoordTable` into recall-scroll consumption.
* Wire `DamageByAngleTable` + `DamageBySoulTable` into the damage
  pipeline (replacing the hard-coded `kBackHitMultiplierX1k` /
  `kFlankHitMultiplierX1k` constants).
* Wire `SubLayerInteractTable` into visibility / attackability checks.
* Wire `QuestTable` into the quest dispatcher (start/progress/complete).
* Wire `NPCActionTable` into NPC tick condition evaluation.
