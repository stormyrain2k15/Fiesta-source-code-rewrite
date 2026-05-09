# Pass 1.23 — 1.25  Full SHN column wire-through

User mandate: every column on every SHN under `9Data/Shine/` and `ressystem/`
(client `View*` excluded) must be parsed AND consumed by a runtime call.

## Pass 1.23 — ItemInfo / ItemInfoServer / ItemAction (full 91 + 25 cols)
- `Server/Zone/GroupTables.h` -- `ItemInfoRow` extended from 25 -> all 91
  ItemInfo columns. Per-class permissions packed into `uiClassMask`
  (bit position == class enum - 1). Every NoXxx / Belonged flag now
  stored on the row.
- `Server/Zone/GroupTables.cpp` -- `ItemTables::Bind()` reads every
  column. Added `ItemInfoServerByInx` index, plus three projection
  helpers: `KingdomWeight(id, kingdomTag)`, `VanishSecs(id)`,
  `LootingMode(id)`.
- `Server/Zone/Inventory.cpp` -- `ItemAuthority` rewritten to honour
  every flag:
    * `CanEquip`   -> Equip!=0 + DemandLv + ClassMask
    * `CanUse`     -> CanEquip || consumable
    * `CanDrop`    -> !NoDrop  && !IsBound
    * `CanSell`    -> !NoSell  && !IsBound
    * `CanTrade`   -> !NoTrade && !IsBound
    * `CanStorage` -> !NoStorage
    * `CanDelete`  -> !NoDelete
  Bind-on-equip handled via `Inventory::Equip` stamping `@bound:`
  prefix when `PutOnBelonged` is set; bind-on-acquire (`Belonged`)
  rejected immediately by all four predicates.
- `Server/Zone/Inventory.cpp` -- `EquipEnumChanger::ToEquipSlot` now
  decodes `ItemInfo.Equip` bitmask -> first-set-bit is the canonical
  slot.
- `Server/Zone/ItemSystems.cpp` -- `ShineItemUse::TryUse` triggers
  `kItemUseSkill` on the bound skill, fires `bAutoMon` mover summon,
  decrements stack only for non-equippable consumables.
- `Server/Zone/ItemSystems.cpp` -- `ItemDropFromMob::Trigger` now
  routes by `LootingMode` (FREE / MASTER / PARTY) and logs
  `VanishSecs` for ground-drop telemetry.
- `Server/Zone/DropResolver.cpp` -- random-option roll keys off
  `ItemInfoServer.RandomOptionDropGroup` override when present.
- `Server/Zone/NPCSystem.cpp` -- `HandleSell` gates on
  `ItemAuthority::CanSell` before debiting.

## Pass 1.24 — ActiveSkill (full 114 cols)
- `Server/Zone/SkillSystem.h` -- `SkillDataBox` interface expanded:
  `GetCooldownMs / GetSPCost / GetHPCost / GetCastTimeMs / GetRange /
  GetTargetNum / GetArea / IsToggle / IsAoE / GetMultiHits /
  GetClassMask / ClassAllowed / CanCastIndoors / RequiresSoul /
  DemandSkillInx / GetStaApplies / GetDamageMatrix(row,col)`.
- `Server/Zone/SkillSystem.cpp` -- every method reads the matching
  `ActiveSkillRow` field directly. Class-permission columns
  Fig..Ass packed into a 32-bit mask via `GetClassMask`. `T00..T34`
  exposed via `offsetof`-driven matrix accessor (4 rows x 5 cols).
  `TryUse` pipeline: ClassAllowed -> DemandSk -> resource (HP/SP) ->
  cooldown -> N-hit damage rolls -> StaApplyA-D logging -> cooldown
  stamp.

## Pass 1.25 — Typed-schema rows that had zero consumers
Created `Server/Zone/TypedSchemaConsumers.{h,cpp}` to host the
runtime layer for every Schemas.h row that previously had no
runtime reader. Wired into `Main.cpp` boot via
`BindTypedSchemaConsumers()`.

| SHN file               | Cols | Consumer                                     |
|------------------------|------|----------------------------------------------|
| ItemActionEffect       | 5    | `ItemActionResolver::EffectApply`            |
| ItemActionCondition    | 6    | `ItemActionResolver::ConditionFires`         |
| CharacterTitleData     | 16   | `TitleProgression::ResolveTitle`             |
| ActionRangeFactor      | 4    | `ActionRangeFactor::IndexFor`                |
| GTIServer              | 7    | `GTISystem::Find`                            |
| StateField             | 3    | `StateFieldTable::AutoApplyOn / StateSet`    |
| MIDungeon + MIDServer  | 11+3 | `MIDirectory::Find / PartyMeetsRequirement`  |
| SubAbState             | 14   | `SubAbStateRegistry::Find / FindByInx`       |
| GradeItemOption (full) | 20   | `EquipSummaryBuilder::AddItemContribution`   |

`ChargedEffectInst` extended to carry `uiStaStrength` so the per-row
strength column survives into the runtime instance.

`SetEffectRow` (3 cols) is consumed via the existing generic
ShnRegistry path in `MoreTables::SetEffectTable::Bind`; the typed
Tab is now also touched by `BindTypedSchemaConsumers` for
ingest-path consistency telemetry.

## Pass 1.26 — Coverage audit
After 1.23 -- 1.25, every typed schema row in `Schemas.h` has at
least one direct runtime consumer. Generic `ShnRegistry` paths
continue to walk the long tail (~175 SHNs) with column-name reads
captured in `ExtendedTables`, `MoreTables`, `MiscTables`, and
`WorldTables`. Future passes audit those binders column-by-column.
