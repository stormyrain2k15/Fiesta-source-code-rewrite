# Pass 1.11.1 -- Soul Stone Correction

## Correction

Earlier pass had soul stones modeled as a *revive* currency consumed at
the death dialog. The project owner clarified this is wrong:

> "Soul stones are an active use item kinda like pots but take 0
>  inventory space; the interface has separate counters for them HP and
>  SP soul stones."

## What changed

### `Server/Zone/SoulStoneSystem.{h,cpp}` -- rewritten

Soul stones are now what they actually are: **active in-combat consumables
that live on the character row (NOT in inventory) and have a dedicated
UI counter per tier**.

* Per-character `SoulStoneCounts` struct with two arrays of tiered
  counters (HP and SP) plus a shared cooldown clock.
* `SoulStoneSystem::Use(player, counts, bIsHp, iTier)` -- the canonical
  active-use entry point. Validates (alive / cooldown / has stones /
  not at max), applies the flat heal, decrements one stone, sets the
  cooldown timer.
* `Grant(counts, bIsHp, iTier, qty)` -- adds to a tier counter, capped
  at `kSoulStoneMaxPerTier`.
* `RemainingCooldownMs()` / `TotalHp()` / `TotalSp()` for UI hooks.

### Tunables (edit-in-place)

```cpp
const int    kSoulStoneTierCount     = 6;
const int32  kHpSoulHeal[6]          = {100, 500, 1000, 3000, 8000, 20000};
const int32  kSpSoulHeal[6]          = {100, 500, 1000, 3000, 8000, 20000};
const char*  kHpSoulName[6]          = { "HP100", "HP500", ... };
const uint16 kSoulStoneMaxPerTier    = 9999;
const uint32 kSoulStoneCooldownMs    = 1500;     // shared with potion bar
```

Add or remove tiers by editing these arrays and bumping
`kSoulStoneTierCount`.

### `Server/Zone/DeathReviveSystem.{h,cpp}` -- new (extracted)

The death-dialog flow lives here, separate from soul stones:

* `ShineSoulPouch` -- a separate counter for Shine Soul Stones, the
  resurrection token consumed at the dialog.
* `eReviveChoice` -- `SHINE` / `TOWN` / `RESURRECT`.
* `DeathReviveSystem::Choose(player, pouch, choice)` -- validates the
  player is dead, then either consumes a Shine Soul (full HP/SP, no XP
  penalty), accepts a town respawn (free, XP penalty curve), or
  defers to a queued resurrect skill effect.
* `ExpLossOnTownRevive(level)` returns the per-mille penalty curve.

## Why the split matters

* HP / SP soul stones are interactive combat economy. They have
  cooldown + counter UI + must not be consumed when the character
  is dead.
* Shine Soul Stones are revive currency. They only exist in the death
  dialog branch and never tick down during combat.

The two files now correctly model that boundary so the next pass can
wire each one to its own NETCOMMAND opcode without code overlap.
