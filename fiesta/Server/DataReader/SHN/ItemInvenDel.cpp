// Server/DataReader/SHN/ItemInvenDel.cpp
// Auto-generated: one-file-per-SHN split for ItemInvenDel.shn
#include "ItemInvenDel.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemInvenDelShn& ItemInvenDelShn::Get() { static ItemInvenDelShn s; return s; }

void ItemInvenDelShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemInvenDel");
    if (!t) { SHINELOG_WARN("ItemInvenDel.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemInvenDelRow rec;
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiDelCondition = ShnGetU32(*t, _r, "DelCondition");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemInvenDel.shn: %u rows", (uint32)m_kRows.size());
}

const ItemInvenDelRow* ItemInvenDelShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace fiesta
