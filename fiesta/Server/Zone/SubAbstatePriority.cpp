// Server/Zone/SubAbstatePriority.cpp
// PDB-name anchor for the `SubAbstatePriority` symbol set. The real
// implementation lives next to the buff/debuff ledger in `AbState.cpp`
// (it depends on `AbStateRow.uiDuplicate` / `uiStateGrade`), where
// `ShouldReplace` and `ShouldStack` are defined. Keeping this TU empty
// preserves the canonical layout without introducing a duplicate
// definition.
namespace fiesta { } // namespace fiesta
