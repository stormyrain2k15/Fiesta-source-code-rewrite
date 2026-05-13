// Server/Zone/MoreTables.cpp
//
// All MoreTables binders moved to /Tables/<Name>.cpp under the
// per-SHN convention (see docs/PER_SHN_CONVENTION.md). This file is
// now just the orchestrator + the empty-namespace block for the
// historical T() macro (no longer used here; binders use BindMacros.h).
//
// Splits owned by /Tables/:
//   PartyBonusTables.cpp      PartyBonusByLvDiff + PartyBonusByMember + PartyBonusLimit
//   SpamerTables.cpp          SpamerPenalty + SpamerPenaltyRule + SpamerReport
//   QuestTables.cpp           QuestDialog + QuestScript + QuestSpecies
//   ToggleSkillTable.cpp      ToggleSkill
//   SetEffectTable.cpp        SetEffect
//   UpgradeTables.cpp         AccUpgrade + UpgradeInfo
//   WeaponAttribTable.cpp     WeaponAttrib
//   UseClassTypeInfoTable.cpp UseClassTypeInfo
//   ShineRewardTable.cpp      ShineReward
//   PupAITables.cpp           PupMind + PupPriority + PupCaseDesc + PupFactorCondition
//   MobBattleTables.cpp       MobAutoAction + MobWeapon + MobResist
//   MiscTables2.cpp           RaceNameInfo + ReactionType + TermExtendMatch +
//                             RareMoverRate + RandomOptionCount
//   SubAbStateTable.cpp       SubAbState
//   AbStateSaveTypeInfoTable.cpp  AbStateSaveTypeInfo
//   SingleDataTable.cpp       SingleData
//   AreaSkillTable.cpp        AreaSkill
#include "MoreTables.h"

namespace shine {

void BindAllMoreTables() {
    PartyBonusTables::Get()        .Bind();
    SpamerTables::Get()            .Bind();
    QuestTables::Get()             .Bind();
    ToggleSkillTable::Get()        .Bind();
    SetEffectTable::Get()          .Bind();
    UpgradeTables::Get()           .Bind();
    WeaponAttribTable::Get()       .Bind();
    UseClassTypeInfoTable::Get()   .Bind();
    ShineRewardTable::Get()        .Bind();
    PupAITables::Get()             .Bind();
    MobBattleTables::Get()         .Bind();
    MiscTables2::Get()             .Bind();
    SubAbStateTable::Get()         .Bind();
    AbStateSaveTypeInfoTable::Get().Bind();
    SingleDataTable::Get()         .Bind();
    AreaSkillTable::Get()          .Bind();
}

} // namespace shine
