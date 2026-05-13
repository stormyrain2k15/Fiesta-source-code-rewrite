// Server/DataReader/SHN/MysteryVaultServer.cpp
// Auto-generated: one-file-per-SHN split for MysteryVaultServer.shn
#include "MysteryVaultServer.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MysteryVaultServerShn& MysteryVaultServerShn::Get() { static MysteryVaultServerShn s; return s; }

void MysteryVaultServerShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MysteryVaultServer");
    if (!t) { SHINELOG_WARN("MysteryVaultServer.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MysteryVaultServerRow rec;
        rec.kMV_GroupItemInx = ShnGetStr(*t, _r, "MV_GroupItemInx");
        rec.uiChrClass = ShnGetU32(*t, _r, "ChrClass");
        rec.kItemInx = ShnGetStr(*t, _r, "ItemInx");
        rec.uiLot = (uint8)ShnGetU32(*t, _r, "Lot");
        rec.uiRate = (uint16)ShnGetU32(*t, _r, "Rate");
        rec.uiGroup = (uint8)ShnGetU32(*t, _r, "Group");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MysteryVaultServer.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
