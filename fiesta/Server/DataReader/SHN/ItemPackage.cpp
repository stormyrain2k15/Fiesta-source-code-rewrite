// Server/DataReader/SHN/ItemPackage.cpp
// Auto-generated: one-file-per-SHN split for ItemPackage.shn
#include "ItemPackage.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ItemPackageShn& ItemPackageShn::Get() { static ItemPackageShn s; return s; }

void ItemPackageShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ItemPackage");
    if (!t) { SHINELOG_WARN("ItemPackage.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ItemPackageRow rec;
        rec.uiHandle = (uint16)ShnGetU32(*t, _r, "Handle");
        rec.kItemID = ShnGetStr(*t, _r, "ItemID");
        rec.kContent = ShnGetStr(*t, _r, "Content");
        rec.uiNumber = (uint16)ShnGetU32(*t, _r, "Number");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ItemPackage.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
