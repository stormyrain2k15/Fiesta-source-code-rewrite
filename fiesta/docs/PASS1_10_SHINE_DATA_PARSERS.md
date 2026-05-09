# Pass 1.10 -- Shine.zip Data Format Integration

This pass digests the project owner's `Shine.zip` data archive and adds first-
class C++ readers for every shipped format that wasn't yet covered.

## Source data (verified locally before writing code)

```
Shine/
├── AbState/                 11 binary  (.dat)        -- abnormal-state timelines
├── AreaBMP/                 53 binary  (.bmp)        -- AOE skill footprints   [already done]
├── BlockInfo/              301 binary  (.shbd/.aid/.sbi)
├── DefaultCharacterData.txt    text   (#DEFINE)     -- already covered by ConfigParser
├── LuaScript/              406 lua    (KQ/ID/AIScript/Promote/Tutorial/PetSystem)
├── MobAttackSequence/      297 text   (TableScript)
├── MobBehaviorDescript/      8 text   (.ps DSL: Default + KQ/*.ps)
├── MobRegen/               126 text   (TableScript)
├── MobRoam/                 28 text   (TableScript)
├── MobSetting/              57 text   (Action/, .xls + per-mob TableScript)
├── NPCItemList/             73 text   (TableScript -- TabXX shop inventories)
├── ScenarioBookShelf/       46 text   (.ps DSL  ID/Promote/Wedding/Guild/Map)
├── Script/                  52 text   (TableScript -- NPC dialog + system msgs)
├── View/                    21 binary (.shn)        -- already covered by ShnFile
└── World/                   48 text   (TableScript -- Field/NPC/Quest/Drop/Param*)
```

## What was added

### 1. Generic TableScript text parser

* **Server/DataReader/TableScriptFile.{h,cpp}** -- recognises the canonical
  `#Ignore / #Exchange / #delimiter / #Table / #ColumnType / #ColumnName /
   #Record / #recordin / #End` directive set used by ~95% of the supplied
  `.txt` data files.
* Multi-table per file. Supports `STRING[N]`, `BYTE`, `WORD`, `DWORD/DWRD`,
  `INDEX`, `FLOAT` types. Honours quoted strings and per-file
  `#exchange`/`#ignore` rewrites.
* Critical correctness fix for files that use `#exchange # \x20`: when an
  exchange targets the space character, the default field delimiter set
  drops `' '` so dialog strings don't get tokenised on word boundaries.
* Validated against:
  * `World/Field.txt`  -- 50 columns x 153 rows (FieldList) +3 sub-tables
  * `World/NPC.txt`    -- 530 ShineNPC + 233 LinkTable rows
  * `World/ChrCommon.txt`  -- 7 sub-tables (Common, StatTable, FreeStat*).
  * `Script/Event.txt` -- dialog strings preserved verbatim (single STRING cell).
  * `MobAttackSequence/AdlFH_Eglack.txt` -- 20 step + 21 SkillChange rows.
  * `NPCItemList/AdlAertsina.txt`        -- multiple TabXX shop tables.

### 2. Typed loaders that wrap TableScriptFile

* **Server/Zone/FieldTable.{h,cpp}**   -- World/Field.txt (FieldList).
* **Server/Zone/ShineNPCTable.{h,cpp}** -- World/NPC.txt (ShineNPC + LinkTable).
* **Server/Zone/MobRegenTable.{h,cpp}** -- MobRegen/<Map>.txt (groups + regens).
* **Server/Zone/NPCItemListTable.{h,cpp}** -- NPCItemList/<NpcKey>.txt (TabXX).
* **Server/Zone/ScriptStringTable.{h,cpp}** -- Script/*.txt unified dialog/msg
  catalogue with a flat lookup map across all loaded files.
* **Server/Zone/MobAttackSequence.cpp** rewritten to consume `AttSeq` +
  `SkillChange` tables via TableScriptFile (was a naive line-tokeniser).

### 3. .ps DSL parser + runtime

* **Server/DataReader/PsScriptFile.{h,cpp}** -- tokenizer + AST for the
  Pascal-flavoured AI / scenario DSL:
  ```
  open [Block]
      var X "" Y "".
      if Cond then open ... close [else open ... close]
      infinite open ... close
      verb arg1 arg2 ... .       ; mobregen/chat/doorbuild/...
      call "BlockName".
      break "BlockName".
  close
  ```
  Handles the operator vocabulary actually present in the data
  (`==`, `===`, `=!=`, `!=`, `<`, `>`, `<=`, `>=`), the `%`-string-concat,
  `@`-prefixed builtin calls (e.g. `@CharClass(handle)`), and quoted strings.

* **Server/Zone/MobBehaviorScript.{h,cpp}** -- runtime over a parsed
  `MobBehaviorDescript/*.ps`. Implements the most common verbs (`whoistarget`,
  `whokillme`, `permillage`, `getname`, `chat`) and the if/else/infinite/
  break/call control flow. Other verbs (`mobregen`, `doorbuild`, etc.) are
  pass-through hooks for the AI host.

* **Server/Zone/ScenarioScript.{h,cpp}** -- runtime wrapper for
  `ScenarioBookShelf/<Cat>/*.ps` (Promote, Wedding, ID, Guild, Map). Uses the
  same PsScriptFile parser; exposes parsed blocks for the scenario host to
  drive.

### 4. BlockInfo binary readers

* **Server/DataReader/BlockInfoFile.{h,cpp}** for the three formats:
  * `.shbd` -- packed walk grid `(u32 width, u32 height, packed cells)`.
  * `.aid`  -- area id name table `(u32 N, char[32]*N)` + raw payload.
  * `.sbi`  -- sub-block info `(u32 N, {char[32] name, u32[5] fields}*N)`.
  
  Layout was reverse-engineered from `Adl.shbd`, `Adl.aid`, `AdlF.sbi`
  hex prefixes; readers surface the parsed entries plus any unparsed trailer
  via a raw byte vector so further reverse-engineering can build on top.

### 5. AbState binary readers

* **Server/DataReader/AbStateInfoFile.{h,cpp}**:
  * `LoadAbStateInfo(AbStateInfo.dat)` -- `u32 count` + N entries of
    `{u8 mainStateId, u8 pad, char[32] english, char[32] localised}`.
  * `LoadAbStateTimeline(StaXxx.dat)` -- `u32 recordCount` + auto-detected
    record stride (28 / 32 / 24 / 20 / 16 bytes), each row exposed as up to
    7 raw u32s.

### 6. ScriptLoader category extensions

`Server/Zone/ScriptLoader.{h,cpp}`:
* Re-aligned the `KnownKQNames` / `KnownInstanceNames` registries to exactly
  match the directories shipped in `Shine.zip` (`AntiHenis`, `EmperorSlime`,
  `GoldHill`, `KDArena`, `KDCake`, `KDEgg`, `KDFargels`, `KDMine`,
  `KDSoccer`, `KDSoccer_W`, `KDSpring`, `KDWater`, `KingSlime`, `Kingkong`,
  `LegendOfBijou`, `MaraPirate`, `MiniDragon`, `HMiniDragon`; and instances
  `AdlF`, `AdlFH`, `Bla`, `CrystalCH`, `CrystalCastle`, `GraveYard*`,
  `IyzelTower*`, `Leviathan*`, `SD_Vale01`, `SecretLab*`, `Siren*`,
  `WarBL*`, `WarH*`, `WarL*`, `WarN*`).
* Added `LoadTutorial`, `LoadPetSystem`, `LoadAIScript` entry points for the
  three additional `LuaScript/` sub-trees that exist in the supplied tree.

## Verification

The TableScript and PsScriptFile parsers were validated by porting the same
algorithm to Python and running it over the actual Shine files:

* TableScript parser correctly extracts:
  * 153 FieldList rows (Field.txt) with 50 columns intact (incl. `RegenCity`,
    `RegenSpot`, `Fiesta`).
  * 530 ShineNPC + 233 LinkTable rows from NPC.txt.
  * 7 sub-tables in ChrCommon.txt (Common 15, StatTable 150, FreeStat* 181 ea).
  * Single-cell dialog strings preserved (Event.txt -- "Please help my mom..."
    survives as one STRING[128] cell).
* PsScriptFile parser produces matching `open`/`close` block counts and
  preserves named top-level blocks for both MobBehaviorDescript/KQ/KingSlime.ps
  (28 blocks, 8 named at top level) and ScenarioBookShelf/Promote/JobChange1.ps
  (top-level block names: `main`, `QuestSuccess`, `QuestFail`, `FirstContact`,
  `First_ShadowFlie`, `FirstKebing`, ...).
* The BlockInfo and AbState readers are best-effort given the partial format
  evidence; they expose unparsed trailers so the next pass can refine offsets.

## Files added / edited (in this pass)

```
Server/DataReader/TableScriptFile.h         (new)
Server/DataReader/TableScriptFile.cpp       (new)
Server/DataReader/PsScriptFile.h            (new)
Server/DataReader/PsScriptFile.cpp          (new)
Server/DataReader/BlockInfoFile.h           (new)
Server/DataReader/BlockInfoFile.cpp         (new)
Server/DataReader/AbStateInfoFile.h         (new)
Server/DataReader/AbStateInfoFile.cpp       (new)
Server/Zone/FieldTable.h                    (new)
Server/Zone/FieldTable.cpp                  (new)
Server/Zone/ShineNPCTable.h                 (new)
Server/Zone/ShineNPCTable.cpp               (new)
Server/Zone/MobRegenTable.h                 (new)
Server/Zone/MobRegenTable.cpp               (new)
Server/Zone/ScriptStringTable.h             (new)
Server/Zone/ScriptStringTable.cpp           (new)
Server/Zone/NPCItemListTable.h              (new)
Server/Zone/NPCItemListTable.cpp            (new)
Server/Zone/MobBehaviorScript.h             (new)
Server/Zone/MobBehaviorScript.cpp           (new)
Server/Zone/ScenarioScript.h                (new)
Server/Zone/ScenarioScript.cpp              (new)
Server/Zone/MobAttackSequence.cpp           (rewritten on top of TableScriptFile)
Server/Zone/ScriptLoader.h                  (added Tutorial/PetSystem/AIScript)
Server/Zone/ScriptLoader.cpp                (registry sync + new loaders)
```

## Open points (for the next pass)

* `MobRoam`, `MobSetting/Action`, `World/Quest`, `World/ItemDropTable`,
  `World/Param*Server` typed loaders -- the generic TableScript parser already
  reads them; only the typed `*Row` structs and `*Tab` accessors are missing.
* `BlockInfo/.aid` and `.sbi` payload fields after the name table still need
  authoritative format docs; readers expose unparsed trailers so this can be
  closed cleanly when the layout is confirmed.
* `AbState/Sta*.dat` row stride is auto-detected from total bytes; the
  semantic interpretation of each u32 (timestep / actionId / strength /
  effect handle) is tentative.
* MobBehaviorScript / ScenarioScript runtimes implement the control-flow
  verbs but most "world-altering" verbs (mobregen, doorbuild, broadcast,
  linkto, scriptfile, effectobj, itemcreate, questresult, npcchat) are
  currently pass-through hooks. Wiring those into the rest of the Zone
  systems is the next step.
