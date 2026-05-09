# Pass 1.11.2 -- Soul Stone Final Model

## Final correction (per project owner)

> "Soul stones are pretty much pots, same effect, same cool timer,
>  instant -- 1 heals HP, 1 heals SP. If u use 1 the other timer does
>  not start. They are separate like that. The stones scale in tier as
>  u level up; at lvl 1 u use a tier-1 pot so u use a tier-1 stone.
>  But that's all back end. The user just sees in the NPC 'buy soul
>  stones' -- look in the NPC.txt docs, some are set to be soul-stone
>  vendors. Like potions that don't use inventory space. Just a number
>  in the db."

## What this nailed down

| Aspect | Final model |
|---|---|
| Storage | **Two** `uint16` counters per character row (`uiHpCount`, `uiSpCount`). Not inventory. Not per-tier. |
| Cooldown | **Two independent** clocks (HP / SP). Using HP doesn't gate SP. |
| Tier | Computed from level via `kSoulStoneTierByLevel[level]`. Player never sees tier. |
| Vendor UX | NPC shop shows two SKUs: "HP Soul Stone" + "SP Soul Stone" at level-appropriate price. |
| Vendor flag | `World/NPC.txt` rows with `Role=Merchant RoleArg0=SoulStone` |
| Inventory cost | 0 |
| Death use | Disallowed (must use the death-revive dialog -- `DeathReviveSystem`). |

## Vendors confirmed in `World/NPC.txt`

```
RouSoulMctJulia       (RouN)
EldSoulMctAvon        (Eld)
UruSoulPooring        (Urg)
HednisSoulKeroll      (EldGbl02)
StoneMctTomson        (UrgFire01)
SoulMctChloe          (UrgSwa01)
AlruinSoulRunadilla   (Urg_Alruin)
BeraSoulOlivia        (Bera)
TempSoul              (RouVal01)
... (every Merchant row whose RoleArg0 is "SoulStone")
```

`SoulStoneVendor::IsVendor(npcMobName)` performs the lookup against
`ShineNPCTable` and returns true on the role/arg match.

## API now in place

```cpp
// Use (in combat).
eSoulStoneUseResult SoulStoneSystem::Use(ShinePlayer*, SoulStoneCounts&, bool bIsHp);
// Buy (at NPC).
eSoulStoneBuyResult SoulStoneVendor::Buy(ShinePlayer*, npcName, Inventory&,
                                          SoulStoneCounts&, bool bIsHp, uint16 qty);
// Read.
SoulStoneSystem::TierForLevel  (level)
SoulStoneSystem::HealForLevel  (level, bIsHp)
SoulStoneSystem::PriceForLevel (level)
SoulStoneSystem::RemainingHpCooldownMs (counts)
SoulStoneSystem::RemainingSpCooldownMs (counts)
```

## Editable tunables (single header, `SoulStoneSystem.h`)

```cpp
const int    kSoulStoneTierCount    = 6;
const int32  kHpSoulHealByTier[6]   = {100, 500, 1000, 3000, 8000, 20000};
const int32  kSpSoulHealByTier[6]   = {100, 500, 1000, 3000, 8000, 20000};
const uint8  kSoulStoneTierByLevel[200] = { ... }; // tier per level
const uint16 kSoulStoneMaxCount     = 9999;
const uint32 kSoulStoneCooldownMs   = 1500;        // independent per HP/SP
const int32  kSoulStonePriceByTier[6] = {50, 200, 800, 3000, 10000, 40000};
```

## Files

```
Server/Zone/SoulStoneSystem.{h,cpp}     (rewritten -- 2 counters, 2 cooldowns, level-driven tier)
Server/Zone/SoulStoneVendor.{h,cpp}     (NEW -- soul-stone merchant flow)
Server/Zone/DeathReviveSystem.{h,cpp}   (kept from 1.11.1 -- separate concern)
```
