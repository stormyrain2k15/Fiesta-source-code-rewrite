// Server/Zone/Tables/UpgradeTables.cpp
// FEATURE: world-creation -- AccUpgrade.shn + UpgradeInfo.shn.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace shine {

UpgradeTables& UpgradeTables::Get() { static UpgradeTables s; return s; }

void UpgradeTables::Bind() {
    // FEATURE: world-creation -- column read: ID, CriFail, DownFail,
    // NormalFail, nCon, LuckySuc
    if (const ShnFile* t = ShnRegistry::Get().GetTable("AccUpgrade")) {
        ITER_ROWS(t) {
            AccRow r;
            r.uiID         = ShnGetU32(*t, _r, "ID");
            r.uiCriFail    = ShnGetU32(*t, _r, "CriFail");
            r.uiDownFail   = ShnGetU32(*t, _r, "DownFail");
            r.uiNormalFail = ShnGetU32(*t, _r, "NormalFail");
            r.uiCon        = ShnGetU32(*t, _r, "nCon");
            r.uiLuckySuc   = ShnGetU32(*t, _r, "LuckySuc");
            m_kAccById[r.uiID] = m_kAcc.size();
            m_kAcc.push_back(r);
        }
    }
    // FEATURE: world-creation -- column read: ID, InxName, UpFactor, Updata
    if (const ShnFile* t = ShnRegistry::Get().GetTable("UpgradeInfo")) {
        ITER_ROWS(t) {
            InfoRow r;
            r.uiID       = ShnGetU32(*t, _r, "ID");
            r.kInxName   = ShnGetStr(*t, _r, "InxName");
            r.uiUpFactor = ShnGetU32(*t, _r, "UpFactor");
            r.kUpdata    = ShnGetStr(*t, _r, "Updata");
            m_kInfoById[r.uiID] = m_kInfo.size();
            m_kInfo.push_back(r);
        }
    }
}

const UpgradeTables::AccRow* UpgradeTables::FindAcc(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kAccById.find(uiID);
    return (it == m_kAccById.end()) ? NULL : &m_kAcc[it->second];
}
const UpgradeTables::InfoRow* UpgradeTables::FindInfo(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kInfoById.find(uiID);
    return (it == m_kInfoById.end()) ? NULL : &m_kInfo[it->second];
}

} // namespace shine
