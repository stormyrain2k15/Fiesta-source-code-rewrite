// Server/DataReader/SHN/ItemMerchantInfo.cpp
// Auto-generated: one-file-per-SHN split for ItemMerchantInfo.shn
#include "ItemMerchantInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ItemMerchantInfoShn& ItemMerchantInfoShn::Get() { static ItemMerchantInfoShn s; return s; }

void ItemMerchantInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemMerchantInfo");
    if (!t) { SHINELOG_WARN("ItemMerchantInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemMerchantInfoRow rec;
        rec.uiMap = (uint8)ShnGetU32(*t, _r, "Map");
        rec.uiSub = (uint8)ShnGetU32(*t, _r, "Sub");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemMerchantInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
