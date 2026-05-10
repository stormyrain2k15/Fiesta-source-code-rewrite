// Server/DataReader/SHN/MobAbStateDropSetting.cpp
// Auto-generated: one-file-per-SHN split for MobAbStateDropSetting.shn
#include "MobAbStateDropSetting.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MobAbStateDropSettingShn& MobAbStateDropSettingShn::Get() { static MobAbStateDropSettingShn s; return s; }

void MobAbStateDropSettingShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobAbStateDropSetting");
    if (!t) { SHINELOG_WARN("MobAbStateDropSetting.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobAbStateDropSettingRow rec;
        rec.kMobInx = ShnGetStr(*t, _r, "MobInx");
        rec.kABStateInx = ShnGetStr(*t, _r, "ABStateInx");
        rec.uiDropType = ShnGetU32(*t, _r, "DropType");
        rec.uiMaxCount = (uint8)ShnGetU32(*t, _r, "MaxCount");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobAbStateDropSetting.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
