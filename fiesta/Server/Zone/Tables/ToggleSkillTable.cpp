// Server/Zone/Tables/ToggleSkillTable.cpp
// FEATURE: world-creation -- ToggleSkill.shn binder.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {
ToggleSkillTable& ToggleSkillTable::Get() { static ToggleSkillTable s; return s; }
void ToggleSkillTable::Bind() {
    // FEATURE: world-creation -- column read: TS_SkillInx, TS_Condition, TS_Value
    if (const ShnFile* t = ShnRegistry::Get().GetTable("ToggleSkill"))
        for (size_t r=0;r<t->Rows().size();++r) {
            Row x;
            x.uiSkillInx  = ShnGetU32(*t,r,"TS_SkillInx");
            x.uiCondition = ShnGetU32(*t,r,"TS_Condition");
            x.iValue      = ShnGetI32(*t,r,"TS_Value");
            m_kBySkill[x.uiSkillInx] = m_kRows.size();
            m_kRows.push_back(x);
        }
}
const ToggleSkillTable::Row* ToggleSkillTable::Find(uint32 s) const {
    std::map<uint32,size_t>::const_iterator i=m_kBySkill.find(s);
    return i==m_kBySkill.end()?NULL:&m_kRows[i->second];
}
} // namespace fiesta
