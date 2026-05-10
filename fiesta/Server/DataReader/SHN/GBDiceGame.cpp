// Server/DataReader/SHN/GBDiceGame.cpp
// Auto-generated: one-file-per-SHN split for GBDiceGame.shn
#include "GBDiceGame.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

GBDiceGameShn& GBDiceGameShn::Get() { static GBDiceGameShn s; return s; }

void GBDiceGameShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("GBDiceGame");
    if (!t) { SHINELOG_WARN("GBDiceGame.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        GBDiceGameRow rec;
        rec.uiMaxBetMoney = ShnGetU32(*t, _r, "MaxBetMoney");
        rec.uiCastTime = (uint16)ShnGetU32(*t, _r, "CastTime");
        rec.uiDelayTime = (uint16)ShnGetU32(*t, _r, "DelayTime");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("GBDiceGame.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
