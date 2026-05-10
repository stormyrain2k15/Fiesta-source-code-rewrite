// Server/DataReader/SHN/MultiHitType.cpp
// Auto-generated: one-file-per-SHN split for MultiHitType.shn
#include "MultiHitType.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MultiHitTypeShn& MultiHitTypeShn::Get() { static MultiHitTypeShn s; return s; }

void MultiHitTypeShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MultiHitType");
    if (!t) { SHINELOG_WARN("MultiHitType.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MultiHitTypeRow rec;
        rec.uiID = (uint16)ShnGetU32(*t, _r, "ID");
        rec.uiHitTime = (uint16)ShnGetU32(*t, _r, "HitTime");
        rec.kAbIndex = ShnGetStr(*t, _r, "AbIndex");
        rec.uiAS_Step = (uint8)ShnGetU32(*t, _r, "AS_Step");
        rec.uiAbStr = (uint8)ShnGetU32(*t, _r, "AbStr");
        rec.uiAbRate = (uint16)ShnGetU32(*t, _r, "AbRate");
        rec.uiDmgRate = (uint16)ShnGetU32(*t, _r, "DmgRate");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MultiHitType.shn: %u rows", (uint32)m_kRows.size());
}

const MultiHitTypeRow* MultiHitTypeShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
