# Shine Online Complete Engine Rewrite Pack — Engine Handoff

Generated: 2026-05-09 00:37:38

This replaces the thin first-pass `shine_engine_requirements` pack with a fuller rewrite spec based on:

- NA2016 server/client file layout and service PDB symbol names.
- Documentation-main SHN/Lua/GM docs.
- Existing packet-capture notes and the planned ShineShark/IOCP/Olly/IDA/Hex-Rays capture workflow.
- Current recovered source inventory.
- Official/fan-facing feature references for public names and gameplay meaning.

## Critical instructions for the implementer

1. **Reuse original names whenever possible.** Start with `function_names/full_function_symbols_filtered.csv` and `function_names/original_object_modules_from_pdb.csv` before inventing a class/function name.
2. **Do not parse or rewrite quest SHNs with unsafe tooling.** Quest handling belongs behind a guarded loader. Runtime packet/DB/Hex-Rays evidence should be used first.
3. **Function coverage comes before perfect math.** Unknown formulas are not blockers if the original-style function exists, is named correctly, and contains local tunable constants/terms that can be edited in the function body later.
4. **Do not invent extra tuning docs, JSON files, balance spreadsheets, DB tables, or config layers for unknown math.** If the original data source exists, use it. If it does not, keep provisional values inside the function itself so the rewrite stays original-shaped.
5. **Treat opcodes/packet layouts marked `VERIFY` as unproven.** Packet structs still need proof, but gameplay math can be approximated inside the correct function and tuned later.
6. **Original server evidence points to Lua 5.2, not Lua 5.4.** Use Lua 5.4 only if Josh explicitly chooses a modernized runtime for the rewrite.
7. **GitHub is source of truth.** This pack is a reconstruction guide, not an authority over the repository.

## Pack contents

- `01_MASTER_COMPLETE_REWRITE_REQUIREMENTS.md` — full system-level requirements.
- `02_COMPLETE_SYSTEM_CATALOG.md` — all systems seen in PDB/data/docs and what each does.
- `03_ORIGINAL_NAMING_AND_FUNCTION_REUSE.md` — how to reuse original naming and where to find names.
- `04_BUILD_ORDER_AND_ACCEPTANCE_TESTS.md` — ordered rewrite plan and pass/fail checks.
- `05_CAPTURE_TO_CODE_WORKFLOW.md` — how ShineShark/rat/Olly/IDA/Hex-Rays output becomes source.
- `06_DATA_TABLE_AND_DOC_CROSSWALK.md` — data files by feature family.
- `07_EXTERNAL_REFERENCE_MAP.md` — official/fan reference use rules.
- `08_FUNCTION_LOCAL_MATH_AND_NO_EXTRA_TUNING_DOCS.md` — policy for missing math/formulas: implement functions first, keep tune values in the function body, and do not invent extra balance documents.
- `function_names/*.csv` — PDB-derived full function/symbol lists, including `high_value_original_symbols_for_reuse.csv` for cleaner direct reuse.
- `manifests/*.csv` — SHN/docs/current-source inventories.
- `source_maps/*.md` — concise reconstruction maps.

## Extract counts

```json
{
  "pdb_filtered_symbols": 92563,
  "pdb_protocol_symbols": 31445,
  "original_object_modules": 621,
  "server_shns": 220,
  "client_shns": 130,
  "server_sidecar_files": 1511,
  "documentation_md_files": 91,
  "current_source_files": 61
}
```
