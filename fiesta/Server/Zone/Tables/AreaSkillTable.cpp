// Server/Zone/Tables/AreaSkillTable.cpp
// FEATURE: world-creation -- AreaSkill.shn.
// NA2016 columns (per data dictionary): AS_SkillInx, AS_Step,
// AS_BMPIndex, AS_ImagePin, AS_IsDirection. Drives the per-skill
// area-of-effect step animation and the bitmap that renders the
// damage shape on the client.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace shine {
AreaSkillTable& AreaSkillTable::Get() { static AreaSkillTable s; return s; }
void AreaSkillTable::Bind() {
    // FEATURE: world-creation -- column read: AS_SkillInx, AS_Step,
    // AS_BMPIndex, AS_ImagePin, AS_IsDirection
    BIND_BEGIN(t, "AreaSkill")
    ITER_ROWS(t) {
        Row x;
        x.kSkillInx    = ShnGetStr(*t,_r,"AS_SkillInx");
        x.uiStep       = ShnGetU32(*t,_r,"AS_Step");
        x.uiBMPIndex   = ShnGetU32(*t,_r,"AS_BMPIndex");
        x.kImagePin    = ShnGetStr(*t,_r,"AS_ImagePin");
        x.uiIsDirection= ShnGetU32(*t,_r,"AS_IsDirection");
        m_kBySkill[x.kSkillInx].push_back(m_kRows.size());
        m_kRows.push_back(x);
    }
}
void AreaSkillTable::RowsForSkill(const std::string& s,
                                  std::vector<const Row*>& rOut) const {
    rOut.clear();
    std::map<std::string,std::vector<size_t> >::const_iterator i=m_kBySkill.find(s);
    if (i==m_kBySkill.end()) return;
    for (size_t k=0;k<i->second.size();++k) rOut.push_back(&m_kRows[i->second[k]]);
}
} // namespace shine
