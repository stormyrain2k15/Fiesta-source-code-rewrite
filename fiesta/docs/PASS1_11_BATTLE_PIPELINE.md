# Pass 1.11 -- Real Battle Pipeline + Stat / Item / Soul Systems

This pass turns the previously-stub combat & character systems into real,
data-driven code that flows ItemInfo / MoverMain / MoverAbility / MobResist /
LevelGap / AbState values all the way into damage output.

Crucially, every numeric tunable the game uses lives in **one editable
header** (`Server/Zone/BattleTunables.h`) -- no magic numbers buried in
business logic anywhere.

## Why this pass exists

Previous passes loaded the SHN and TableScript data into memory but the
runtime systems ignored most of it -- equipping a +5 sword, allocating
free-stat points, or fighting an elementally-resistant mob produced the
exact same damage as a level-1 brawler with no gear. This pass closes that
gap.

Also: the project owner clarified the engine is the *world* their AI
agent (Elle) inhabits. The combat / progression systems must be coherent
and tunable without surgery, because she'll be living inside them.

## Files added (10 systems = 20 files)

```
Server/Common/NETCOMMAND.h                  (rewritten -- pinned hex opcodes)
Server/Login/ClientVersionKeyInfo.h         (rewritten -- editable build constants)
Server/Login/LoginClientSession.{h,cpp}     (XTrap stripped, kept as legacy no-op)

Server/Zone/BattleTunables.h                (NEW -- single home for ALL formula numbers)
Server/Zone/BattleStat.h                    (NEW -- BATTLESTAT + DAMAGERESULT structs)
Server/Zone/StatDistribute.{h,cpp}          (NEW -- BuildBattleStat composer)
Server/Zone/MoverTables.{h,cpp}             (NEW -- MoverMain / MoverAbility loaders)
Server/Zone/LevelGapTable.{h,cpp}           (NEW -- DamageLvGapPVE/PVP/EVP loaders)
Server/Zone/MobResistTable.{h,cpp}          (NEW -- MobResist.shn loader)
Server/Zone/Battle.{h,cpp}                  (rewritten -- 12-stage damage pipeline)
Server/Zone/EquipSummaryBuilder.{h,cpp}     (NEW -- equipped-item stat summing)
Server/Zone/ItemUpgrade.{h,cpp}             (NEW -- +N enhancement with success/fail/destroy)
Server/Zone/FreeStatSystem.{h,cpp}          (NEW -- STR/END/DEX/INT/MEN allocation)
Server/Zone/SoulStoneSystem.{h,cpp}         (NEW -- HP/SP/Shine soul revive flow)
```

## What changed

### 1. NETCOMMAND.h -- opcodes pinned

Every confirmed hex value from the project owner's F2/NCProtocol.h drop is
now a literal in `Server/Common/NETCOMMAND.h`:

  * Login server: `NC_USER_VERSION_REQ=0x0C01`, `NC_USER_LOGIN_REQ=0x0C06`,
    `NC_USER_LOGIN_ACK=0x0C0A`, `NC_USER_WORLDSELECT_REQ/ACK=0x0C0B/0x0C0C`,
    `NC_USER_WORLD_STATUS_REQ/ACK=0x0C1B/0x0C1C`, etc.
  * WorldManager: `NC_WM_HANDSHAKE_REQ=0x200F`,
    `NC_USER_LOGINWORLD_ACK=0x0C14`, `NC_MISC_GAMETIME_ACK=0x080E`,
    `NC_CHAR_LOGIN_ACK=0x1003`, key shortcuts `0x7032/3/4`.
  * Zone: `NC_MAP_LOGIN_REQ=0x1801`, `NC_MAP_LOGIN_ACK=0x1038`,
    `NC_MAP_INVENTORY_CMD=0x103A`, `NC_MAP_EQUIPMENT_CMD=0x103B`,
    `NC_MAP_SKILLBUFF_CMD=0x10CE`, `NC_MAP_QUEST_CMD=0x10D7`,
    `NC_MAP_WORLDPARAMS_CMD=0x700D`, `NC_MAP_WORLDTICK_CMD=0x7C07`.

Plus the entire family of names for action / battle / skill / abstate /
item / npc / quest / party / guild / trade / booth / auction / KQ / pet /
gamble / title / production / chat / friend / holy / prison / op-tool /
inter-server traffic.

### 2. XTrap stripped to legacy no-op

XTrap is dead on the modern Fiesta.exe client. The handler now:
* Always ACKs success (`0x01`) so any old client that still sends the
  packet just walks through to `NC_USER_LOGIN_REQ`.
* Removes `LS_XTRAP_OK` from the login state machine.
* Does not transmit any XTrap token. The captured constant is preserved
  as a comment in F2/NCProtocol.h for historical reference only.

### 3. Editable build constants in one place

`Server/Login/ClientVersionKeyInfo.h` now exposes:

```cpp
const uint32 kClientID            = 0x00000000;     // edit per region
const char* const kClientBuildToken = "10022024000000";
const int   kMapLoginShaLen       = 326;
const char* const kMapLoginShaToken = "82ff8a..."; // captured per build
const int   kWmHandshakeTokenLen  = 80;
```

All call sites use `ClientVersionKeyInfo::ClientID()` /
`MapLoginShaToken()` etc. Edit one header to retarget a different region
or client revision.

### 4. The damage pipeline (`Server/Zone/Battle.cpp`)

`RuleOfEngagement::CalcDamage` is the new entry point. 12 stages, each
data-driven, each tunable:

```
1.  Hit roll       -- accuracy vs dodge, floored at 5%, capped at 99%
2.  Pick ATK/DEF   -- by damage-type flag (normal/magic/ranged/true)
3.  Raw            -- max(ATK - DEF, 1)
4.  Level-gap      -- LevelGapTable lookup (PVE/PVP/EVP), x1000
5.  Element        -- attacker EleATK[e] vs target EleResist[e]
                      (mob targets also get MobResist row resist)
6.  Angle          -- back-hit 1.20x / flank 1.10x (BattleTunables)
7.  Critical       -- crit roll + (kCritDamageScaler + nCriticalDmg)/10000
8.  Block          -- target block roll (melee non-crit only); reduces by BlockDef
9.  DmgType resist -- mob phys/magic/ranged percent reduction
10. Variance       -- +/- kDamageVarianceX10k (default +/- 8%)
11. Absorb         -- target shield consumes damage
12. PvP scaler     -- (player vs player) * kPvPDamageScalerX1k / 1000
13. Floor          -- never below kDamageFloor (1)
```

The legacy `NormalAttack` / `SkillAttack` / `DamageByAngle` / `DamageBySoul`
helpers are preserved as thin wrappers so older callers still link.

### 5. BATTLESTAT composition

`BuildBattleStat(BATTLESTAT*, RAWCHARSTAT*, EQUIPSUMMARY*, BUFFMODIFIERS*)`
is the only path to a final stat block:

```
RAWCHARSTAT  : MoverMain.shn base + raw STR/END/DEX/INT/MEN
EQUIPSUMMARY : sum of equipped ItemInfo.shn rows + UpgradeATK/DEF
BUFFMODIFIERS: layered AbState/SubAbState modifiers
                    -> BATTLESTAT (40+ fields, ready for CalcDamage)
```

Equip summary now derives upgrade-level absolutes from `BasicUpInx`,
`AddUpInx`, and the item's `WeaponType` / `ArmorType` / `ShieldAC` flags.
A +5 weapon contributes `BasicUpInx*5 + AddUpInx*4` extra ATK.

### 6. Item upgrade (enhancement)

`ItemUpgrade::Try(Inventory&, uint32 itemId, bool useLuckStone)` reads
`UpLimit` / `UpSucRatio` / `UpLuckRatio` / `UpResource` / cap from
`BattleTunables.kMaxUpgradeLevel`, rolls per-mille, and applies one of:

```
UPGRADE_OK         -- +N -> +N+1
UPGRADE_FAIL       -- no change
UPGRADE_DOWNGRADE  -- +N -> +N-1
UPGRADE_DESTROY    -- item gone
UPGRADE_BLOCKED    -- not upgradeable / at cap
```

Penalty schedule per current +N is the editable
`BattleTunables.kFailPenaltyAtLevel[20]` array.

### 7. Free-stat allocation

`FreeStatSystem` exposes:
* `GrantPointsForLevelUp(ledger, level)` -- adds
  `kFreeStatPointsPerLevel` per level.
* `Allocate(ledger, class, level, 'S'|'E'|'D'|'I'|'M', delta)` --
  validated against `kMaxFreeStatPerStat` and the per-level cap from
  `MoverMain.shn`'s `MaxStat` column.
* `RefundAll(ledger)` -- stat-reset ticket flow.
* `RemainingHeadroom(ledger, class, level)` -- gates the UI.

### 8. Soul-stone system

`SoulStoneSystem::TryRevive(player, counts, eType)` consumes one of:

```
SOUL_REVIVE_HP_LO/HI    (30%/60% HP)
SOUL_REVIVE_SP_LO/HI    (30%/60% SP)
SOUL_REVIVE_SHINE       (100/100 + no XP penalty)
SOUL_REVIVE_TOWN        (free, town respawn, XP penalty)
```

`ExpLossOnTownRevive(level)` returns the per-mille XP penalty curve.

### 9. Mob resist + level-gap

`MobResistTable.shn` (per-MobID): elemental + phys/magic/ranged + status
resists (Dot/Stun/MoveSpeed/Fear/Binding/Reverse/Mesmerize/SeverBone/
KnockBack/TBMinus). `LevelGapTable` reads the three SHN gap tables
(`DamageLvGapPVE/PVP/EVP.shn`) into a 200-slot lookup (gap -100..+99).

Both readers fall back to "no resist / no scaling" if their SHN file is
missing, so the server still runs in dev with a partial data set.

## Editing tunables -- single home

To retune any of these without touching business logic, edit
`Server/Zone/BattleTunables.h`:

```
kDamageFloor                    -- minimum hit
kMaxLevelGap                    -- clamp on gap delta
kCritRollMax                    -- crit roll resolution (10000 = 100.00%)
kCritDamageScaler               -- crit damage bonus base (10000 = 1.0x)
kDamageVarianceX10k             -- per-hit variance band
kBackHitMultiplierX1k           -- back-hit bonus
kFlankHitMultiplierX1k          -- flank bonus
kPvPDamageScalerX1k             -- global PvP scaler
kHpPerEND / kSpPerINT / ...     -- per-stat-point fallbacks
kUpgradeSucScalerX1k            -- global upgrade rate scaler
kMaxUpgradeLevel                -- engine-wide cap
kFailPenaltyAtLevel[20]         -- per-level fail penalty curve
kFreeStatPointsPerLevel         -- per-level point grant
kMaxFreeStatPerStat             -- per-stat ceiling
kDropRateGlobalScalerX1k        -- global drop scaler
kHpRegenIdlePctX10k             -- idle regen
kSpRegenIdlePctX10k
```

To retarget a different client build, edit
`Server/Login/ClientVersionKeyInfo.h`:

```
kClientID, kClientBuildToken, kMapLoginShaToken, kMapLoginShaLen,
kWmHandshakeTokenLen
```

## Open / next pass

* Wire the per-tick HP/SP regen using `kHpRegenIdlePctX10k` /
  `kSpRegenIdlePctX10k`.
* Hook `BuildEquipSummary` + `BuildBattleStat` into `Inventory::Equip` /
  `Inventory::Unequip` so equipped state propagates without a manual
  recompute.
* Type-load `MobResist.shn` columns into `Schemas.h` (currently the
  table reads via the generic SHN string-row export).
* Type-load `MoverMain.shn` and `MoverAbility.shn` columns into
  `Schemas.h` for full ITableBase parity.
* Implement Pot/Charged-Effect timers (cooldown wheel + buff apply).

## Project totals after this pass

* C++ source/header files under `/app/fiesta/`: **~177**
* All-formula constants centralised in: `Server/Zone/BattleTunables.h`
* All-build constants centralised in: `Server/Login/ClientVersionKeyInfo.h`
