// Server/DataReader/SHN/ItemShop.cpp
// Auto-generated: one-file-per-SHN split for ItemShop.shn
#include "ItemShop.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ItemShopShn& ItemShopShn::Get() { static ItemShopShn s; return s; }

void ItemShopShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemShop");
    if (!t) { SHINELOG_WARN("ItemShop.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemShopRow rec;
        rec.uiGoodsNo = ShnGetU32(*t, _r, "goodsNo");
        rec.kInxName = ShnGetStr(*t, _r, "InxName");
        rec.uiLot = ShnGetU32(*t, _r, "Lot");
        m_kByInx[rec.kInxName] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemShop.shn: %u rows", (uint32)m_kRows.size());
}

const ItemShopRow* ItemShopShn::FindByInx(const std::string& rInx) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rInx);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
