// Server/Zone/MobSettingActionTable.h
// 19 -- MobSetting/Action/<Mob>.txt -- per-mob (Trigger -> Action) bindings.
//
// Each file ships one Trigger table:
//   ConditionID  ActionID
//
// ConditionID identifies a runtime gate (e.g. "OnRegen", "OnDamageBelowHP30%",
// "OnTargetTooFar") and ActionID names a row in the mob's AI script that
// should fire. Same backing format as the rest -- TableScript.
//
// EVIDENCE: DATA_CONFIRMED  source: project-owner-supplied MobSetting/Action/.
#ifndef FIESTA_ZONE_MOBSETTINGACTIONTABLE_H
#define FIESTA_ZONE_MOBSETTINGACTIONTABLE_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MobActionTrigger {
    uint32 uiConditionID;
    uint32 uiActionID;
};

class MobSettingActionFile {
public:
    bool Load(const std::string& rPath);
    const std::vector<MobActionTrigger>& Triggers() const { return m_kTriggers; }
    bool ResolveActionFor(uint32 uiConditionID, uint32* pOut) const;
private:
    std::vector<MobActionTrigger> m_kTriggers;
};

class MobSettingActionBox {
public:
    static MobSettingActionBox& Get();
    const MobSettingActionFile* Load(const std::string& rRoot, const std::string& rMobName);
    const MobSettingActionFile* Find(const std::string& rMobName) const;
    void Clear();
private:
    std::map<std::string, MobSettingActionFile*> m_kAll;
};

} // namespace fiesta
#endif
