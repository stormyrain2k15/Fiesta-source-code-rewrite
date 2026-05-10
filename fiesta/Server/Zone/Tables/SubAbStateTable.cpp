// Server/Zone/Tables/SubAbStateTable.cpp
// FEATURE: world-creation -- SubAbState.shn binder. Effect-engine
// sub-state rows (4 action-index/arg pairs per row; selected by
// (Inx, Strength) tier). Core to the AbState runtime.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {

SubAbStateTable& SubAbStateTable::Get() { static SubAbStateTable s; return s; }

void SubAbStateTable::Bind() {
    // FEATURE: world-creation -- column read: ID, InxName, Strength,
    // Type, SubType, KeepTime, ActionIndexA..D, ActionArgA..D
    BIND_BEGIN(t, "SubAbState")
    ITER_ROWS(t) {
        Row r;
        r.uiID         = ShnGetU32(*t, _r, "ID");
        r.kInxName     = ShnGetStr(*t, _r, "InxName");
        r.uiStrength   = ShnGetU32(*t, _r, "Strength");
        r.uiType       = ShnGetU32(*t, _r, "Type");
        r.uiSubType    = ShnGetU32(*t, _r, "SubType");
        r.uiKeepTimeMs = ShnGetU32(*t, _r, "KeepTime");
        r.uiActionIdxA = ShnGetU32(*t, _r, "ActionIndexA");
        r.iActionArgA  = ShnGetI32(*t, _r, "ActionArgA");
        r.uiActionIdxB = ShnGetU32(*t, _r, "ActionIndexB");
        r.iActionArgB  = ShnGetI32(*t, _r, "ActionArgB");
        r.uiActionIdxC = ShnGetU32(*t, _r, "ActionIndexC");
        r.iActionArgC  = ShnGetI32(*t, _r, "ActionArgC");
        r.uiActionIdxD = ShnGetU32(*t, _r, "ActionIndexD");
        r.iActionArgD  = ShnGetI32(*t, _r, "ActionArgD");
        size_t idx = m_kRows.size();
        m_kById[r.uiID] = idx;
        m_kByInx[r.kInxName].push_back(idx);
        m_kRows.push_back(r);
    }
}

const SubAbStateTable::Row* SubAbStateTable::Find(uint32 uID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const SubAbStateTable::Row* SubAbStateTable::FindByInx(const std::string& rInx,
                                                       uint32 uiStrength) const {
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kByInx.find(rInx);
    if (it == m_kByInx.end() || it->second.empty()) return NULL;
    const Row* pkBest = NULL;
    for (size_t i = 0; i < it->second.size(); ++i) {
        const Row* r = &m_kRows[it->second[i]];
        if (uiStrength == 0) {
            if (!pkBest || r->uiStrength < pkBest->uiStrength) pkBest = r;
        } else {
            if (r->uiStrength <= uiStrength) {
                if (!pkBest || r->uiStrength > pkBest->uiStrength) pkBest = r;
            }
        }
    }
    if (!pkBest) {
        for (size_t i = 0; i < it->second.size(); ++i) {
            const Row* r = &m_kRows[it->second[i]];
            if (!pkBest || r->uiStrength < pkBest->uiStrength) pkBest = r;
        }
    }
    return pkBest;
}

void SubAbStateTable::GatherByInx(const std::string& rInx,
                                  std::vector<const Row*>& rOut) const {
    rOut.clear();
    std::map<std::string, std::vector<size_t> >::const_iterator it = m_kByInx.find(rInx);
    if (it == m_kByInx.end()) return;
    for (size_t i = 0; i < it->second.size(); ++i)
        rOut.push_back(&m_kRows[it->second[i]]);
}

} // namespace fiesta
