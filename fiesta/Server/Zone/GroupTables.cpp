// Server/Zone/GroupTables.cpp
// Each Bind() walks the corresponding ShnFile's rows once and builds a
// typed vector + index map. Column reads are by NAME (`ShnGetU32(t, i,
// "ColName")`) so a future drop that renames or reorders columns lights up
// the missing-column path (zero-fill / empty-string) instead of silently
// reading the wrong field. The trade-off vs index reads is one O(N) name
// scan per accessor binding -- negligible at boot.
#include "GroupTables.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

#define BIND_BEGIN(VAR, NAME) \
    const ShnFile* VAR = ShnRegistry::Get().GetTable(NAME); \
    if (!VAR) { SHINELOG_WARN("%s.shn missing -- group accessor disabled", NAME); return; }

#define ITER_ROWS(VAR) for (size_t _r = 0; _r < (VAR)->Rows().size(); ++_r)

// =============================================================================
//  ItemTables / MobTables / SkillTables / MapTables / AbStateTables
//  -- moved to /Tables/<Name>.cpp under the one-cpp-per-shn convention
//  adopted 2026-02. See docs/SCHEMA_COVERAGE.md for the policy and
//  Build/CI/audit_unwired_loads.py for the canary that catches
//  regressions where a Bind() exists but is never called from
//  BindAllGroupTables() below.

// =============================================================================
//  All per-class binders moved to /Tables/<Name>.cpp under the
//  one-cpp-per-shn convention (docs/PER_SHN_CONVENTION.md).
//  This file is now just the BindAllGroupTables() orchestrator.
// =============================================================================

void BindAllGroupTables() {
    ItemTables::Get()        .Bind();
    MobTables::Get()         .Bind();
    SkillTables::Get()       .Bind();
    MapTables::Get()         .Bind();
    AbStateTables::Get()     .Bind();
    PresentationTables::Get().Bind();
    PupTables::Get()         .Bind();
    MountTables::Get()       .Bind();
    MiniHouseTables::Get()   .Bind();
    // GuildTables2 binder body lives in Tables/MiniHouseTables.cpp
    // (it consumes GuildAcademy / GuildGradeData / GuildTournament[Reward]
    // shns). Wire it here so the academy/tournament/grade tables actually
    // populate at boot.
    GuildTables2::Get()      .Bind();
    CollectTables::Get()     .Bind();
    GradeRandomTables::Get() .Bind();
    KQTables::Get()          .Bind();
    SHINELOG_INFO("BindAllGroupTables: done; ShnRegistry has %u tables",
                  (uint32)ShnRegistry::Get().size());
}

#undef BIND_BEGIN
#undef ITER_ROWS

} // namespace shine
