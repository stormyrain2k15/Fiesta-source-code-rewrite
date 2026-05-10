# Per-SHN Binder Convention

**Adopted 2026-02.** One `.cpp` (and where practical, one `.h`) per
`.shn` file. Same rule applies to per-table `.txt` schemas. Only `.ps`
(PineScript) and `.lua` files are folder-walked, because they are
actionable scripts, not world-data tables.

## Why

Korean-MMO patch shipping convention: the publisher gets a `.obj`
drop, relinks, and ships. Beyond that, there's a hard technical
reason — **static-initialization order**. When 38 binders share one
translation unit, the order their constructors and `static T s;`
singletons run in is implicit. Add or remove a single SHN and any
unrelated binder three modules over can silently break because its
init now happens at a different point in the sequence. Splitting
each binder into its own translation unit makes the link order
explicit, the patch unit minimal, and each binder grep-target-able.

## What lives where

```
Server/Zone/                       (legacy mega-binders below
├── Tables/                         + new per-SHN files in /Tables/)
│   ├── BindMacros.h               # shared BIND_BEGIN / ITER_ROWS
│   ├── ItemTables.cpp             # ItemInfo + ItemInfoServer +
│   │                              #  ItemUpgrade + ItemAction
│   ├── MobTables.cpp              # MobInfo + MobSpecies + MobLifeTime
│   ├── MapTables.cpp              # MapInfo
│   ├── AbStateTables.cpp          # AbState
│   └── SkillTables.cpp            # ActiveSkill + ActiveSkillInfoServer
│                                  #  + PassiveSkill + AreaSkill
├── GambleHouse/                   # full per-SHN split (18 binders)
│   ├── GBHouseTable.{h,cpp}
│   ├── GBExchangeMaxCoinTable.{h,cpp}
│   ├── ...
│   ├── DiceTaiSai/
│   │   ├── DiceGameTable.{h,cpp}
│   │   ├── DiceRateTable.{h,cpp}
│   │   └── DiceDividindTable.{h,cpp}
│   └── SlotMachine/
│       ├── GBSMTables.h           # shared header (8 sibling tables)
│       ├── GBSMAll.cpp ... GBSMNPC.cpp
│       └── ...
├── Link.cpp / Link.h              # MapLinkPoint + MapWayPoint
├── TownPortalSystem.{cpp,h}       # TownPortal
├── LuckyCapsuleSystem.{cpp,h}     # LCGroupRate + LCReward
└── GroupTables.cpp                # remaining 8 mega-binders
                                   #  (PresentationTables, PupTables,
                                   #   MountTables, MiniHouseTables,
                                   #   GuildTables2, CollectTables,
                                   #   GradeRandomTables, KQTables) --
                                   #  flagged for future split
```

## When to use a shared header

Two cases where one header for several SHNs is allowed:

1. **One Get() across multiple SHNs** -- e.g. `ItemTables` owns four
   SHNs (`ItemInfo`, `ItemInfoServer`, `ItemUpgrade`, `ItemAction`)
   that form one Find* surface. Splitting the singleton across .cpp
   files would force it into the header (the static-init trap). Solve
   by keeping all four binders in one .cpp and sharing the class
   declaration in the upstream header.

2. **Coupled sibling tables** -- e.g. the eight `GBSM*` slot tables
   (one .cpp each, one shared `GBSMTables.h`). Each table is small;
   the consumer always touches several at once during a slot resolve;
   eight separate headers would force every consumer to include nine
   files.

In every other case: **one `.shn` -> one `.cpp`** without exception.

## How to add a new SHN

1. Pick the destination dir (`/Tables/` for world-creation tables,
   `/GambleHouse/` for casino, the system's own dir for system-local
   tables).
2. Create `<TableName>.cpp` and (where the binder is class-owned)
   `<TableName>.h`. Use the BIND_BEGIN / ITER_ROWS macros via
   `Tables/BindMacros.h`.
3. Mark every column read with the comment
   `// FEATURE: <bucket-name> -- column read: ColA, ColB, ColC`
   so the column auditor can see which subsystem owns the read.
4. Wire `Get().Bind()` (or `Get().Load(rRoot)` for `.txt` schemas)
   from a boot path:
     - `Server/Zone/Main.cpp` for top-level singletons.
     - `BindAllGroupTables`/`BindAllMiscTables`/`BindAllMoreTables`/
       `BindAllExtendedTables` for legacy mega-binder reuse.
     - `GambleSystem::BindAllCasinoTables()` for casino.
5. Run the audits locally:
     ```
     python3 Build/CI/audit_shn_wiring.py
     python3 Build/CI/audit_unwired_loads.py
     python3 Build/CI/audit_shn_columns.py --system <feature>
     ```
   The first two MUST exit 0. The third surfaces the column-level
   gap; check that every column the SHN ships is read or annotated
   `// FEATURE: <bucket> -- column read: <ColName> (write-only)` if
   the column is intentionally not consumed.

## Backlog (legacy mega-binders not yet split)

| File                        | Binders left | Priority   |
| --------------------------- | -----------: | ---------- |
| `Zone/GroupTables.cpp`      |            8 | next sprint|
| `Zone/ExtendedTables.cpp`   |           38 | future     |
| `Zone/MoreTables.cpp`       |           16 | future     |
| `Zone/MiscTables.cpp`       |            7 | future     |
| `Zone/WorldTables.cpp`      |            7 | future     |

The remaining `GroupTables.cpp` binders to split next:
PresentationTables, PupTables, MountTables, MiniHouseTables,
GuildTables2, CollectTables, GradeRandomTables, KQTables.
