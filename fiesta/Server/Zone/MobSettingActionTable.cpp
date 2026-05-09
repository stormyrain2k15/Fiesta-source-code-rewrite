// Server/Zone/MobSettingActionTable.cpp
#include "MobSettingActionTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

bool MobSettingActionFile::Load(const std::string& rPath) {
    m_kTriggers.clear();
    TableScriptFile f;
    if (!f.Load(rPath)) return false;
    const TsTable* t = f.Find("Trigger");
    if (!t) return false;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        MobActionTrigger x;
        x.uiConditionID = (uint32)t->GetInt(r, "ConditionID");
        x.uiActionID    = (uint32)t->GetInt(r, "ActionID");
        m_kTriggers.push_back(x);
    }
    return !m_kTriggers.empty();
}

bool MobSettingActionFile::ResolveActionFor(uint32 uiConditionID, uint32* pOut) const {
    for (size_t i = 0; i < m_kTriggers.size(); ++i) {
        if (m_kTriggers[i].uiConditionID == uiConditionID) {
            if (pOut) *pOut = m_kTriggers[i].uiActionID;
            return true;
        }
    }
    return false;
}

MobSettingActionBox& MobSettingActionBox::Get() { static MobSettingActionBox s; return s; }

const MobSettingActionFile* MobSettingActionBox::Load(const std::string& rRoot, const std::string& rMobName) {
    std::string p = rRoot + "\\MobSetting\\Action\\" + rMobName + ".txt";
    MobSettingActionFile* pk = new MobSettingActionFile();
    if (!pk->Load(p)) { delete pk; return NULL; }
    m_kAll[rMobName] = pk;
    SHINELOG_DEBUG("MobSettingAction '%s' triggers=%u", rMobName.c_str(), (uint32)pk->Triggers().size());
    return pk;
}
const MobSettingActionFile* MobSettingActionBox::Find(const std::string& rMobName) const {
    std::map<std::string, MobSettingActionFile*>::const_iterator it = m_kAll.find(rMobName);
    return (it == m_kAll.end()) ? NULL : it->second;
}
void MobSettingActionBox::Clear() {
    for (std::map<std::string, MobSettingActionFile*>::iterator it = m_kAll.begin(); it != m_kAll.end(); ++it)
        delete it->second;
    m_kAll.clear();
}

} // namespace fiesta
