# Pass 1.5 — Documentation integration pass

Date: 2026-05-09. The user provided `Documentation-main` + `fiesta_documentation_integration_pack`
(987 SHN field rows, 279 Lua API entries, 11 Lua enum classes, 155 GM commands, 56 DB fields).
Code-generated five new files from the structural facts (names, types, ordinals, admin levels)
and wired them into pass-1 stubs.

## New files

| File | Purpose | Items |
|---|---|---|
| `Server/DataReader/Schemas.h`               | 20 headline `*Row` structs + matching `*Tab` ITableBase classes | ItemInfoRow (91 fields), AbStateRow (18), SubAbStateRow (14), ActiveSkillRow (114), MobInfoRow (15), ItemActionEffectRow, ItemActionConditionRow, ItemInfoServerRow (25), CharacterTitleDataRow (16), CollectCardRow, ActionRangeFactorRow, GTIServerRow, GuildTournamentRow (73), StateFieldRow, MIDungeonRow, MIDServerRow, ChargedEffectRow, AbStateViewRow, plus SetEffect/GradeItemOption/MultiHitTable variants where present |
| `Server/DataReader/Schemas.cpp`             | binary/text row decoders + `RegisterAllSchemaTabs()` | one Load() per Tab |
| `Lua/LuaAPI_Generated.cpp`                  | 272 documented Lua C bindings, all callable stubs | one `static int Lua_<name>(lua_State*)` each + `RegisterAllLuaAPIs()` |
| `Lua/LuaEnums_Generated.cpp`                | 11 Lua enum tables exposed as globals | ReturnAI / ObjectType / ObjectMode / BasicClass / EFFECT_MSG_TYPE / CAMERA_STATE / KQ_TEAM / PlayerDamage / PlayerList / StatsEnum / Classes |
| `Server/Zone/AmpersandCommands_Generated.cpp` | dispatch table for all 154 GM commands with admin-level gates | `kAmpersandCmds[]` + `DispatchAmpersand()` |
| `Server/DataServer/Common/DBSchemaConstants.h` | inline accessors for 56 documented DB column names | `DBField::tUser_nUserNo()` etc. |

## Wiring

- `Server/Zone/Main.cpp` now calls `RegisterAllSchemaTabs()` instead of registering five example tables.
- `Lua/LuaCBindings.cpp` now installs all 272 generated stubs first, then overrides the five real
  bindings (cDamaged / cStaticDamage / cSetAbstate / cLinkTo / cFinishKey) on top.
- `Server/Zone/AdminSecLog.cpp` `AmpersandCommand::Execute` forwards to `DispatchAmpersand`.
- `Server/DataReader/Tables.h` is now a 13-line back-compat shim aliasing the new schema row types.
- `Server/Zone/{AbState,Inventory,ItemSystems,SkillSystem}.cpp` updated to reference the new `*Row` types.
- `Server/DataServer/Common/SQLP.cpp` references the documented `tUser` columns via DBSchemaConstants.

## Source provenance

All generated code is fresh C++. Inputs were the **structural** CSVs:
- field name + ordinal + type (no descriptive prose copied)
- function name + arg count (no descriptions)
- command name + admin level (no descriptions)
- DB section / table / column name

Lua function bodies are uniform stubs that accept their declared arg count and push no value;
real bodies wire in pass 2 keyed off the original-name surface.
