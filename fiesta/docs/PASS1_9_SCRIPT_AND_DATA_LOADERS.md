# Pass 1.9 — Game-data tree consumers (LuaScript / MobAttackSequence / AreaBMP)

Date: 2026-05-09. User clarified that `Shine.zip` is the runtime data tree
(configs + Lua content scripts + game data), not server source. Used the
zip's central directory listing to gap-check the engine's data consumers --
no script bodies pulled, only the directory layout / naming convention.

## Verified file inventory

```
LuaScript/common.lua
LuaScript/KQ/<Name>/{Routine,Progress,SubFunc}.lua + Data/{Name,Boss,Regen,Process,NPC,Chat,Stuff,ItemDrop,Servant,Monster}.lua
LuaScript/ID/<Name>/{Routine,Progress,SubFunc}.lua + Data/...
LuaScript/Promote/Job2_Forest/Functions/{Routine,Progress,SubFunc}.lua
MobAttackSequence/<MobName>.txt          (200+ files)
AreaBMP/<Skill>_*.bmp
ScenarioBookShelf/.../*.ps               (compiled PineScript)
DefaultCharacterData.txt
```

Canonical KQ names (16): KDArena, AntiHenis, KDSoccer, Kingkong, KDMine,
KDSoccer_W, KDEgg, KDWater, MaraPirate, MiniDragon, KDFargels, GoldHill,
HMiniDragon, KDCake, LegendOfBijou, KingSlime.

Canonical Instance names (11+): CrystalCH, IyzelTower, IyzelTowerH, SecretLab,
SecretLabH, Siren, SirenH, Leviathan, GraveYard, CrystalCastle, Bla.

## New files

| File | Purpose |
|---|---|
| `Server/Zone/ScriptLoader.{h,cpp}`        | Scans `LuaScript/` and loads each event in canonical order: `Data/*.lua` -> `SubFunc.lua` -> `Progress.lua` -> `Routine.lua`. Exposes `LoadCommon`, `LoadKQ`, `LoadInstance`, `LoadPromote`, plus `LoadAllKQ` / `LoadAllInstances` driven by the verified name registries. |
| `Server/Zone/MobAttackSequence.{h,cpp}`   | Loads `MobAttackSequence/<MobName>.txt` line-based attack patterns into `MobAttackSequenceBox` for AI consumption. |
| `Server/Zone/AreaBMP.{h,cpp}`             | Minimal BMP loader (BI_RGB 8 / 24 / 32 bpp, top-down or bottom-up) -> `AreaMask` grids for AOE skill footprints. |

## Wiring

Call sites for pass 2 (one-line additions):
- `MIDServer::SpinUp(template, group)` -> `ScriptLoader::LoadInstance(g_kLua, root, name)`.
- `KQServer::Tick()` state transition into `KQS_RUNNING` -> `ScriptLoader::LoadKQ(...)`.
- `MobInfoTab` row populate -> `MobAttackSequenceBox::Get().Load(root, mobName)`.
- Skill cast time -> if `IsAoE` -> `AreaBMPBox::Get().Load(root, skillKey)` lazy-load on first cast.

## Source provenance

Only the structural layout of the zip (file paths and naming convention) was
used. No Lua script content, no `.txt` mob sequence content, no BMP pixels were
copied into code. The loaders read the project owner's runtime data files at
service start; if the user runs the resulting binaries with the data tree in
place, the engine consumes the data unchanged.

## Total file count

129 (pass 1.8) + 6 = **135** source files.
