// Server/Zone/AbnormalState.cpp
// PDB-name anchor for the legacy `AbnormalState*` symbol surface. The
// real implementation lives in `AbState.cpp` (the runtime ledger) and
// `AbStateRuntime.cpp` (the per-tick action dispatcher). This file is
// intentionally empty so the PDB layout still has a TU named after the
// canonical class set when an old debugger lays down per-symbol files.
namespace shine { } // namespace shine
