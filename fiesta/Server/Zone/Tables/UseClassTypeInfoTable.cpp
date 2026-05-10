// Server/Zone/Tables/UseClassTypeInfoTable.cpp
// FEATURE: world-creation -- UseClassTypeInfo.shn binder. 27-class
// usability mask per UseClass id.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {

UseClassTypeInfoTable& UseClassTypeInfoTable::Get() {
    static UseClassTypeInfoTable s; return s;
}

void UseClassTypeInfoTable::Bind() {
    static const char* kClassCols[] = {
        "Fig","Cfig","War","Gla","Kni","Cle","Hcle","Pal","Hol","Gua",
        "Arc","Harc","Sco","Sha","Ran","Mag","Wmag","Enc","Warl","Wiz",
        "Jok","Chs","Cru","Cls","Ass","Sen","Sav"
    };
    static const size_t kN = sizeof(kClassCols) / sizeof(kClassCols[0]);
    // FEATURE: world-creation -- column read: UseClass, Fig..Sav (27 flags)
    BIND_BEGIN(t, "UseClassTypeInfo")
    ITER_ROWS(t) {
        uint64 mask = 0;
        for (size_t c = 0; c < kN; ++c) {
            if (ShnGetU32(*t, _r, kClassCols[c]) != 0) mask |= (1ULL << c);
        }
        m_kMask[ShnGetU32(*t, _r, "UseClass")] = mask;
    }
}

bool UseClassTypeInfoTable::IsUsable(uint32 uiUseClass, uint32 uiClassID) const {
    std::map<uint32, uint64>::const_iterator it = m_kMask.find(uiUseClass);
    if (it == m_kMask.end() || uiClassID >= 64) return false;
    return (it->second & (1ULL << uiClassID)) != 0;
}

} // namespace fiesta
