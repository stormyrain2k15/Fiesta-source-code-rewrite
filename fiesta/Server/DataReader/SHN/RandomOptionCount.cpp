// Server/DataReader/SHN/RandomOptionCount.cpp
// Auto-generated: one-file-per-SHN split for RandomOptionCount.shn
#include "RandomOptionCount.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

RandomOptionCountShn& RandomOptionCountShn::Get() { static RandomOptionCountShn s; return s; }

void RandomOptionCountShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("RandomOptionCount");
    if (!t) { SHINELOG_WARN("RandomOptionCount.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        RandomOptionCountRow rec;
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiLimitCount = (uint16)ShnGetU32(*t, _r, "LimitCount");
        rec.uiLimitDropRate = (uint16)ShnGetU32(*t, _r, "LimitDropRate");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("RandomOptionCount.shn: %u rows", (uint32)m_kRows.size());
}

const RandomOptionCountRow* RandomOptionCountShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
