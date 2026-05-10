// Server/Zone/Tables/AbStateSaveTypeInfoTable.cpp
// FEATURE: world-creation -- AbStateSaveTypeInfo.shn binder. Drives
// whether each abstate save-type persists across (link, die, logoff).
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {

AbStateSaveTypeInfoTable& AbStateSaveTypeInfoTable::Get() {
    static AbStateSaveTypeInfoTable s; return s;
}

void AbStateSaveTypeInfoTable::Bind() {
    // FEATURE: world-creation -- column read: AbStateSaveType,
    // IsSaveLink, IsSaveDie, IsSaveLogoff
    BIND_BEGIN(t, "AbStateSaveTypeInfo")
    ITER_ROWS(t) {
        Row r;
        r.uiSaveType  = ShnGetU32(*t, _r, "AbStateSaveType");
        r.bSaveLink   = ShnGetU32(*t, _r, "IsSaveLink")   != 0;
        r.bSaveDie    = ShnGetU32(*t, _r, "IsSaveDie")    != 0;
        r.bSaveLogoff = ShnGetU32(*t, _r, "IsSaveLogoff") != 0;
        m_kByType[r.uiSaveType] = m_kRows.size();
        m_kRows.push_back(r);
    }
}

const AbStateSaveTypeInfoTable::Row*
AbStateSaveTypeInfoTable::Find(uint32 uT) const {
    std::map<uint32, size_t>::const_iterator it = m_kByType.find(uT);
    return (it == m_kByType.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
