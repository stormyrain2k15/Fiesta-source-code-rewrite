# SHN Schema & Wiring Coverage

This document is the canonical record of which `.shn` data files the
server is responsible for, which it intentionally leaves to the client,
and how the wiring is verified.

## Source of truth

The data dictionary that drove this audit ships in
`docs/spec_pack/data_dictionary/`:

| File                                              | Purpose                                                     |
| ------------------------------------------------- | ----------------------------------------------------------- |
| `File_Summary.csv`                                | One row per file in the data tree (1730 rows, 219 SHN).     |
| `Plain_English_Data_Dictionary.csv`               | Per-column meaning for every parsed SHN (5727 rows).        |
| `Server_Link_Hints.csv`                           | Foreign-key / lookup hints (720 columns flagged).           |
| `Parse_Issues.csv`                                | Empty -- every SHN parsed cleanly.                          |
| `Shine_Plain_English_Column_Map_skip_QuestData.xlsx` | XLSX form of the column map.                              |

These files are user-supplied as the authoritative schema reference.
They are read-only inputs for tooling; do not edit them in place.

## Coverage summary (last verified 2026-02)

```
Total SHN files in inventory: 219
Client-only view tables:      18  (intentionally unwired)
Server-wired:                 201
Server-side gaps:               0
```

Verified by `Build/CI/audit_shn_wiring.py`. Run it any time after
adding or removing SHN consumers:

```cmd
cd fiesta
python Build\CI\audit_shn_wiring.py
```

Exit code `0` on PASS, `1` on regression. The canary workflow
(`.github/workflows/canary.yml`) runs this on every push to `main`.

## What "server-wired" means

A SHN is considered wired if any of its access forms appears in the
C++ source under `/Server`:

| Form                              | Where it shows up                                  |
| --------------------------------- | -------------------------------------------------- |
| `ShnRegistry::Get().GetTable("X")`| Direct registry lookup -- most common.             |
| `T("X")` inline helper            | `MiscTables.cpp`, `ExtendedTables.cpp`, etc.       |
| `BIND_BEGIN(t, "X")` macro        | `GroupTables.cpp` block bindings.                  |
| `"X.shn"` literal path            | `MoverTables.cpp`, `ChargedEffect.cpp`, etc.       |

## The 18 client-only View tables

Loaded by the registry (so generic tooling can introspect them) but
never read by any server binder. Listed in `ShnRegistry::IsClientViewShn`:

```
ActiveSkillView          MapViewInfo
CharacterTitleStateView  MobConditionView
CollectCardView          MobViewInfo
EffectViewInfo           MoverSlotView
GTIView                  MoverView
ItemShopView             PassiveSkillView
ItemViewDummy            ProduceView
ItemViewEquipTypeInfo    PupView
ItemViewInfo             SetItemView
```

These hold pure UI display data: skill icons & descriptions, item
tooltips & equip-slot icons, mob nameplates, minimap labels, mount
ride-UI sprites, pet UI, set-bonus tooltips, character-title overlays,
production-UI captions. The NA2016 server never validates against
them; gameplay logic uses the `*.shn` companion in the parent
`Shine\` folder (e.g. `ActiveSkill.shn` + `ActiveSkillInfoServer.shn`
on the server, `View\ActiveSkillView.shn` on the client).

If you ever need the server to own one of these, remove it from the
`kKnown[]` list in `Server/DataReader/ShnRegistry.cpp` AND the
`CLIENT_ONLY_VIEWS` set in `Build/CI/audit_shn_wiring.py`, then add
the consumer.

## Boot-time runtime audit

`ShnRegistry::ShnAudit_EmitReport` runs once after every binder has
had its first access pass, and emits two diagnostics:

1. **Per-table column audit.** For every table that *was* opened, any
   column that no binder read gets a `ShnAudit: <table>.<col> parsed
   but not consumed` warning. This catches partial-binder coverage.
2. **Registry-level ownership audit.** Walks the full registry; any
   table with no recorded read produces an `unowned table` warning,
   except quest-deferred SHNs (owned by `QuestShnReader`) and the 18
   client-view tables (intentionally unread).

Sample expected log line:
```
ShnRegistryAuditor: 0 unowned, N quest-deferred, 18 client-view (intentionally unread)
```

A non-zero `unowned` count means a new SHN file landed on disk and no
binder consumes it -- treat as a regression and add the consumer or
explicitly tag it.
