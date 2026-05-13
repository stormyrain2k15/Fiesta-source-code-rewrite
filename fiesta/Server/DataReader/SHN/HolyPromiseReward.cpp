// Server/DataReader/SHN/HolyPromiseReward.cpp
// Auto-generated: one-file-per-SHN split for HolyPromiseReward.shn
#include "HolyPromiseReward.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

HolyPromiseRewardShn& HolyPromiseRewardShn::Get() { static HolyPromiseRewardShn s; return s; }

void HolyPromiseRewardShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("HolyPromiseReward");
    if (!t) { SHINELOG_WARN("HolyPromiseReward.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        HolyPromiseRewardRow rec;
        rec.uiLevel = (uint8)ShnGetU32(*t, _r, "Level");
        rec.uiClass = (uint8)ShnGetU32(*t, _r, "Class");
        rec.kItemIndex = ShnGetStr(*t, _r, "ItemIndex");
        rec.uiLot = (uint16)ShnGetU32(*t, _r, "Lot");
        rec.uiUpgrade = (uint8)ShnGetU32(*t, _r, "Upgrade");
        rec.uiIO_Str = ShnGetU32(*t, _r, "IO_Str");
        rec.uiIO_Con = ShnGetU32(*t, _r, "IO_Con");
        rec.uiIO_Dex = ShnGetU32(*t, _r, "IO_Dex");
        rec.uiIO_Int = ShnGetU32(*t, _r, "IO_Int");
        rec.uiIO_Men = ShnGetU32(*t, _r, "IO_Men");
        rec.uiRes1 = ShnGetU32(*t, _r, "Res1");
        rec.uiRes2 = ShnGetU32(*t, _r, "Res2");
        rec.uiRes3 = ShnGetU32(*t, _r, "Res3");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("HolyPromiseReward.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
