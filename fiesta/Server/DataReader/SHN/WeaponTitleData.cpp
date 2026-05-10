// Server/DataReader/SHN/WeaponTitleData.cpp
// Auto-generated: one-file-per-SHN split for WeaponTitleData.shn
#include "WeaponTitleData.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

WeaponTitleDataShn& WeaponTitleDataShn::Get() { static WeaponTitleDataShn s; return s; }

void WeaponTitleDataShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("WeaponTitleData");
    if (!t) { SHINELOG_WARN("WeaponTitleData.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        WeaponTitleDataRow rec;
        rec.uiMobID = (uint16)ShnGetU32(*t, _r, "MobID");
        rec.uiLevel = (uint8)ShnGetU32(*t, _r, "Level");
        rec.kPrefix = ShnGetStr(*t, _r, "Prefix");
        rec.kSuffix = ShnGetStr(*t, _r, "Suffix");
        rec.uiMobKillCount = ShnGetU32(*t, _r, "MobKillCount");
        rec.uiMinAdd = (uint16)ShnGetU32(*t, _r, "MinAdd");
        rec.uiMaxAdd = (uint16)ShnGetU32(*t, _r, "MaxAdd");
        rec.uiSP1_Reference = (uint8)ShnGetU32(*t, _r, "SP1_Reference");
        rec.iSP1_Type = (int16)ShnGetI32(*t, _r, "SP1_Type");
        rec.uiSP1_Value = ShnGetU32(*t, _r, "SP1_Value");
        rec.uiSP2_Reference = (uint8)ShnGetU32(*t, _r, "SP2_Reference");
        rec.iSP2_Type = (int16)ShnGetI32(*t, _r, "SP2_Type");
        rec.uiSP2_Value = ShnGetU32(*t, _r, "SP2_Value");
        rec.uiSP3_Reference = (uint8)ShnGetU32(*t, _r, "SP3_Reference");
        rec.iSP3_Type = (int16)ShnGetI32(*t, _r, "SP3_Type");
        rec.uiSP3_Value = ShnGetU32(*t, _r, "SP3_Value");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("WeaponTitleData.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
