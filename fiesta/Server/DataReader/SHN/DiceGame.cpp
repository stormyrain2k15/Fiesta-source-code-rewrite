// Server/DataReader/SHN/DiceGame.cpp
// Auto-generated: one-file-per-SHN split for DiceGame.shn
#include "DiceGame.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

DiceGameShn& DiceGameShn::Get() { static DiceGameShn s; return s; }

void DiceGameShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("DiceGame");
    if (!t) { SHINELOG_WARN("DiceGame.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        DiceGameRow rec;
        rec.uiItemID = (uint16)ShnGetU32(*t, _r, "ItemID");
        rec.uiUseMinLv = (uint16)ShnGetU32(*t, _r, "UseMinLv");
        rec.uiGetSysRate = (uint16)ShnGetU32(*t, _r, "GetSysRate");
        rec.uiGetMasterRate = (uint16)ShnGetU32(*t, _r, "GetMasterRate");
        rec.uiMinGetMoney = ShnGetU32(*t, _r, "MinGetMoney");
        rec.uiMaxBetMoney = ShnGetU32(*t, _r, "MaxBetMoney");
        rec.uiCastTime = (uint16)ShnGetU32(*t, _r, "CastTime");
        rec.uiDelayTime = (uint16)ShnGetU32(*t, _r, "DelayTime");
        rec.iWinCode = ShnGetI32(*t, _r, "WinCode");
        rec.iUnkCol9 = ShnGetI32(*t, _r, "UnkCol9");
        rec.iUnkCol10 = ShnGetI32(*t, _r, "UnkCol10");
        rec.iLoseCode = ShnGetI32(*t, _r, "LoseCode");
        rec.iUnkCol12 = ShnGetI32(*t, _r, "UnkCol12");
        rec.iUnkCol13 = ShnGetI32(*t, _r, "UnkCol13");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("DiceGame.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
