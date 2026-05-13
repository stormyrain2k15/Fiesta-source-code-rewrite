// Server/Zone/Tables/SingleDataTable.cpp
// FEATURE: world-creation -- SingleData.shn (server constants).
#include "BindMacros.h"
#include "../MoreTables.h"

namespace shine {
SingleDataTable& SingleDataTable::Get() { static SingleDataTable s; return s; }
void SingleDataTable::Bind() {
    // FEATURE: world-creation -- column read: SingleDataIDX, SingleDataValue
    if (const ShnFile* t = ShnRegistry::Get().GetTable("SingleData"))
        for (size_t r=0;r<t->Rows().size();++r)
            m_kRows[ShnGetU32(*t,r,"SingleDataIDX")] = ShnGetI32(*t,r,"SingleDataValue");
}
int32 SingleDataTable::Get(uint32 i, int32 d) const {
    std::map<uint32,int32>::const_iterator it=m_kRows.find(i);
    return it==m_kRows.end()?d:it->second;
}
} // namespace shine
