// Server/DataReader/SHN/ItemMoney.cpp
// Auto-generated: one-file-per-SHN split for ItemMoney.shn
#include "ItemMoney.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ItemMoneyShn& ItemMoneyShn::Get() { static ItemMoneyShn s; return s; }

void ItemMoneyShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemMoney");
    if (!t) { SHINELOG_WARN("ItemMoney.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemMoneyRow rec;
        rec.kIM_ItemIDX = ShnGetStr(*t, _r, "IM_ItemIDX");
        rec.kIM_MoneyIDX = ShnGetStr(*t, _r, "IM_MoneyIDX");
        rec.uiIM_MoneyLot = (uint16)ShnGetU32(*t, _r, "IM_MoneyLot");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemMoney.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
