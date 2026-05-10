// Server/DataReader/SHN/BelongTypeInfo.cpp
// Auto-generated: one-file-per-SHN split for BelongTypeInfo.shn
#include "BelongTypeInfo.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

BelongTypeInfoShn& BelongTypeInfoShn::Get() { static BelongTypeInfoShn s; return s; }

void BelongTypeInfoShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("BelongTypeInfo");
    if (!t) { SHINELOG_WARN("BelongTypeInfo.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        BelongTypeInfoRow rec;
        rec.uiBT_Inx = ShnGetU32(*t, _r, "BT_Inx");
        rec.uiPutOnBelonged = (uint8)ShnGetU32(*t, _r, "PutOnBelonged");
        rec.uiNoDrop = (uint8)ShnGetU32(*t, _r, "NoDrop");
        rec.uiNoSell = (uint8)ShnGetU32(*t, _r, "NoSell");
        rec.uiNoStorage = (uint8)ShnGetU32(*t, _r, "NoStorage");
        rec.uiNoTrade = (uint8)ShnGetU32(*t, _r, "NoTrade");
        rec.uiNoDelete = (uint8)ShnGetU32(*t, _r, "NoDelete");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("BelongTypeInfo.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
