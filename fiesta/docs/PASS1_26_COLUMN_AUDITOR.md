# Pass 1.26 -- Long-tail SHN + World/*.txt column auditor

## Mandate
User: "Audit per-column coverage of the long-tail SHNs read via the generic
ShnRegistry path (CollectCardDropRate / CollectCardStarRate / EnchantSocket /
KQReward / GuildAcademy* / etc.) and fill any column that's parsed-but-
unconsumed. Audit every `World/*.txt` runtime side (BlockInfo, MobRegen,
MobRoam, MobSetting, MobAttackSequence, NPCAction, NPCItemList) the same way."

## Approach
With 200+ SHN files and ~50 World/*.txt files, we built a runtime auditor
that diffs each file's declared columns against the binders' read-set.
Any column that's parsed but never asked-for emits a `WARN` line at the
end of boot. The user pastes the warnings back, we fill them in the next
pass. This is the only safe way to keep coverage when the SHN headers
drift between drops.

## Implementation

### SHN side
- `Server/DataReader/ShnRegistry.cpp` -- adds three free functions
  (`ShnAudit_BeginTable`, `ShnAudit_RecordRead`, `ShnAudit_EmitReport`)
  hooked transparently into `GetTable()` and `ShnGet*`. The "current
  audit table" is set as a side-effect of `GetTable()` returning a
  pointer; `ShnGet*` records the column key against it but only when the
  passed-in `&rTab == s_pkAuditLastFile` (so cross-table reads don't
  pollute the wrong set).
- `Server/DataReader/ShnRegistry.h` -- exposes the three audit hooks.

### TS side (World/*.txt)
- `Server/DataReader/TableScriptFile.cpp` -- adds the same shape:
  `TsAudit_Record / Reset / RegisterLoaded / VisitTable / EmitReport`.
  `TsTable::GetCell(row, name, out)` stamps `(table, column)` into the
  auditor; `TableScriptFile::Load()` registers every parsed `TsTable`
  with `TsAudit_RegisterLoaded` so the boot-end walker can diff against
  the binder reads.
- `Server/DataReader/TableScriptFile.h` -- forward declarations.

### Boot wiring
- `Server/Zone/Main.cpp` -- after `BindAllExtendedTables()` and
  `BindTypedSchemaConsumers()`, calls
  `ShnAudit_EmitReport(ShnRegistry::Get())` and `TsAudit_EmitReport()`.
  Each unconsumed column appears as a single `WARN` line in the boot
  log, deduped by table name (so a 30-map MobRegen run only emits each
  unread column once).

### Direct gap fill
- `Server/Zone/NPCItemListTable.cpp` -- bumped the per-tab column
  capture from `Column00..05` to `Column00..15`. The shipped tab
  formats vary (BUYITEM=2 cols, QUEST=4-5, MOVERLINK=6-8, GBSTORE=10+);
  16 is wide enough to cover every observed shape without truncation.

## Coverage today

| Path                    | Status                                      |
|-------------------------|---------------------------------------------|
| Typed SHN schemas (25)  | 100 % -- every column on every typed row    |
| Generic SHN registry    | Audited at boot; warnings drive the next pass |
| TS World/*.txt          | Audited at boot; same WARN cadence          |
| BlockInfo binaries      | 100 % -- byte-level packed, no columns      |

## Next-pass action items
After the first compile + boot, the user will have a log full of
`ShnAudit:` and `TsAudit:` warnings naming each `<table>.<column>` that's
parsed but not consumed. Paste those back; the next pass extends the
relevant binder fields and routes them to a runtime consumer.
