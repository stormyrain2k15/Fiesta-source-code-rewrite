# Pass 1.20 -- Per-folder asset ingest + NPC spawn pipeline

**User trigger (verbatim):**
> "Just keep going with adding the files and what each function behind
> them is and making sure the server does the same thing."

This pass closes the gap between "the data is on disk" and "the server
is actually running on it." Pass 1.18-1.19 wired the headline SHN tables
and the ScenarioBookShelf scripts. Pass 1.20 adds the per-folder
walkers, the per-NPC item list -> merchant pipeline, and the per-class
stat ladder.

## What changed

### 1. `ZoneAssetLoader.h / .cpp` (new)

Single boot-time pass that walks every per-folder asset under
`Data\Shine\` and feeds each file to its existing parser / Box. The
walker uses `FindFirstFile` (Win32) so it stays VS2010-clean.

| Folder                    | Pattern    | Routed to                        |
|---------------------------|------------|----------------------------------|
| `AbState/`                | `*.dat`    | `AbStateBinaryBox` (new, in this file) |
| `AreaBMP/`                | `*.bmp`    | `AreaBMPBox::Load`               |
| `BlockInfo/`              | `*.shbd`   | `LoadShbd` -> `Field::Blocks()`  |
| `BlockInfo/`              | `*.aid`    | `LoadAid`                        |
| `BlockInfo/`              | `*.sbi`    | `LoadSbi`                        |
| `MobAttackSequence/`      | `*.txt`    | `MobAttackSequenceBox::Load`     |
| `MobBehaviorDescript/`    | `*.txt`    | `MobBehaviorBox::LoadDefault` / `LoadKQ` |
| `MobRegen/`               | `*.txt`    | `MobRegenBox::Load`              |
| `MobRoam/`                | `*.txt`    | `MobRoamBox::Load`               |
| `MobSetting/Action/`      | `*.txt`    | `MobSettingActionBox::Load`      |
| `NPCItemList/`            | `*.txt`    | `NPCItemListBox::Load`           |
| `Script/`                 | `*.txt`    | `ScriptStringBox::LoadAll`       |
| `LuaScript/**/`           | `*.lua`    | `ScriptLoader::Load*`, `MobAISystem::LoadAll`, `InstanceSystem::LoadAll`, `PromoteSystem::LoadAll` |

The BlockInfo `.shbd` callback resolves the file's basename via
`MapTables::FindByName` and stamps the loaded `BlockGrid` into the
matching `Field`'s blocked-cell array, so collision is live the moment
boot completes.

`AbStateBinaryBox` holds an in-memory name-only index of every entry in
`AbState/*.dat` so the server can validate references by string without
keeping the rendering data hot.

`Pump LuaRuntime` is also done here: the ZoneService now opens a
`LuaRuntime` member and calls `m_kLua.Open()` before handing it to the
asset loader. Shutdown closes it.

### 2. `ShineNPCTable.cpp` -- new `SpawnAll()`

Walks every `ShineNPC` row from `World/NPC.txt`, allocates a unique
`uiNpcId`, instantiates a `ShineNPC`, registers it with `NPCManager`
(by id and by mob-name), and adds it to the resolved `Field`. NPC rows
whose Map column doesn't resolve to a numeric `MapID` are still
registered (so cross-zone admin tools can address them) but skip the
field insertion.

### 3. `NPCSystem.cpp` -- merchant pipeline

`NPCManager::RegisterKey(npcId, mobName)` and `KeyOf(npcId)` provide
the bridge between the runtime NPC handle and the `NPCItemList/*.txt`
filename.

`NPCItemList::GetForShop` now actually walks the loaded
`NPCItemListFile` -> `Tabs()` -> `kRows` -> `kColumns` and resolves each
33-char ItemIndex to an `ItemInfoRow` (price = `uiBuyPrice`).

`SellItemManager::BuyFromNpc` validates the buy against the resolved
inventory, debits gold via `ShinePlayer::AddMoney`, and hands off to
the existing `CMSG_BUY_FROM_NPC` flow for the actual inventory insert
(the inventory subsystem owns slot allocation and stack merge logic).

### 4. `Zone/Main.cpp`

Adds new boot calls in dependency order:

```
LoadAllPineScripts(...)
KarenAttackTable / MobChatTable / ChargedEffect tables
m_kLua.Open(); m_kLua.RegisterCBindings();
ZoneAssetLoader::LoadAll(...)
ShineNPCTable::Load(...) / SpawnAll()
ClassParamTable::Load(...)
ChrCommonTable::Load(...)
```

Shutdown adds `m_kLua.Close()`.

### 5. `ShnRegistry.cpp`

Now also walks `Data\Shine\View\*.shn` (the 21 view-shape files). Even
though those are client-targeted, several server runtime paths look
them up by name (e.g. `MapViewInfo`, `NPCViewInfo`).

## Coverage matrix (post-1.20)

| Source                                   | Files | Wired |
|------------------------------------------|-------|-------|
| `Data\Shine\AbState\*.dat`               | 11    | 11    |
| `Data\Shine\AreaBMP\*.bmp`               | 53    | 53    |
| `Data\Shine\BlockInfo\*.{shbd,aid,sbi}`  | 301   | 301   |
| `Data\Shine\MobAttackSequence\*.txt`     | per-mob | walked |
| `Data\Shine\MobBehaviorDescript\*.txt`   | per-mob | walked |
| `Data\Shine\MobRegen\*.txt`              | per-zone| walked |
| `Data\Shine\MobRoam\*.txt`               | per-zone| walked |
| `Data\Shine\MobSetting\Action\*.txt`     | per-mob | walked |
| `Data\Shine\NPCItemList\*.txt`           | 73    | 73    |
| `Data\Shine\Script\*.txt`                | 52    | 52    |
| `Data\Shine\View\*.shn`                  | 21    | 21    |
| `Data\Shine\LuaScript\*.lua` (every dir) | per-system | walked |
| `Data\Shine\ScenarioBookShelf\*.ps`      | manifest-driven | walked (Pass 1.19) |
| `Data\Shine[-1]\*.shn`                   | 199   | 199   (Pass 1.18 + 1.19) |
| `Data\Shine\World\*.txt`                 | 48    | 48    (Pass 1.19) |

## Functional consequences

Now that the per-folder ingest is wired, the existing systems gain real
behaviour at boot:

* **Mob spawning**: `MobRegenBox` is populated, so per-zone spawn lists
  exist; `MobRoamBox` provides waypoints; `MobBehaviorBox` provides AI
  trees; `MobAttackSequenceBox` provides per-species attack patterns.
  The MobSpawn helper from Pass 1.18 can now consult all four.
* **Collision**: `Field::Blocks()` is real, so movement validation in
  `MoveManager` actually resolves to blocked cells per `*.shbd`.
* **Merchants**: clicking a vendor NPC routes through
  `NPCItemList::GetForShop` -> the per-NPC `*.txt` -> live item list.
* **Shop tabs**: `NPCShopTab::kTabName` is preserved so the client UI
  can render the multi-tab merchant the original game uses (Tab00 =
  weapons, Tab01 = armour, etc.).
* **AreaBMP**: AOE skills now resolve their per-skill mask via
  `AreaBMPBox::Load(skillKey)`.
* **Lua**: AI scripts, KQ scripts, ID instances, Promote ceremonies,
  PetSystem, Tutorial, common.lua are all loaded before the first
  client connect.

## Known follow-up

* `MobSpawnSystem::Tick` should iterate `MobRegenBox` and instantiate
  Mobs per row; today the Box is populated but there's no driver yet.
* `EstateSystem::Tick` records the desired aura abstate for placed
  furniture; the AbState dispatcher hook to actually push it to nearby
  ShinePlayers needs to land in the next pass.
* `ExpeditionSystem::ApplyMasterBuff` is similarly a record-only
  operation; AbState propagation across all sub-party members ditto.
* CharDB persistence for Marriage / Estate is still pending.

No "Elle AI", no client version-check, no `GamigoZR`, no standalone
`OperatorTool` were re-introduced.
