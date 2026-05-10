// Server/Zone/MiscTables.cpp
//
// All MiscTables binders moved to /Tables/<Name>.cpp under the
// per-SHN convention (see docs/PER_SHN_CONVENTION.md). This file is
// now just the orchestrator + the explicit removal of the broken
// DamageLvGapTables binder.
//
// Splits owned by /Tables/:
//   ActionTables.cpp     ActionEffectItem.shn + ActionRangeFactor.shn
//   StateTables.cpp      StateField.shn + StateItem.shn + StateMob.shn
//   CraftTables.cpp      TownPortal.shn (presence) + Gather.shn + Produce.shn
//   HPRewardTable.cpp    HolyPromiseReward.shn
//   RidingTable.cpp      Riding.shn
//   BadNameFilter.cpp    BadNameFilter.shn
//
// REMOVED: DamageLvGapTables binder. The original looked up a column
// named "Multiplier" that does not exist in NA2016's DamageLvGapPVE/
// EVP/PVP SHNs (PvP is a 151x151 (MyLv x TargetLv) matrix; PvE/EvP are
// (LvGap, DamageRate) curves). The class is superseded by
// LevelGapTable (Server/Zone/LevelGapTable.{cpp,h}) which reads the
// real columns and is wired from Zone Main. Anything still calling
// DamageLvGapTables::Get().PvE/EvP/PvP should migrate to
// LevelGapTable::Get().GetMultiplier(...). Backlog: remove the class
// declaration once all callers are migrated.
#include "MiscTables.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

// Stub binders for the deprecated DamageLvGapTables so the class still
// links. Returns the safe 1.0x neutral fallback for every lookup; the
// real damage scaler is LevelGapTable.
DamageLvGapTables& DamageLvGapTables::Get() { static DamageLvGapTables s; return s; }
void DamageLvGapTables::Bind() {
    // Intentionally empty -- LevelGapTable does the real work.
    m_uiPvPSide = 0;
}
int32 DamageLvGapTables::PvE(int32) const                          { return 1000; }
int32 DamageLvGapTables::EvP(int32) const                          { return 1000; }
int32 DamageLvGapTables::PvP(int32, uint32, uint32) const          { return 1000; }

// One-call orchestrator. Order is irrelevant -- each binder is
// self-contained and reads its own SHN.
void BindAllMiscTables() {
    DamageLvGapTables::Get().Bind();      // deprecated; safe stub
    ActionTables::Get()     .Bind();
    StateTables::Get()      .Bind();
    CraftTables::Get()      .Bind();
    HPRewardTable::Get()    .Bind();
    RidingTable::Get()      .Bind();
    BadNameFilter::Get()    .Bind();
}

} // namespace fiesta
