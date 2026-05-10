// Server/Zone/GambleHouse/DiceTaiSai/DiceGameTable.cpp
// FEATURE: casino-dice
#include "DiceGameTable.h"
#include "../../../DataReader/ShnRegistry.h"
#include "../../../Shared/ShineLogSystem.h"

namespace fiesta {

DiceGameTable& DiceGameTable::Get() { static DiceGameTable s; return s; }

bool DiceGameTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-dice -- column read: ItemID, UseMinLv, GetSysRate,
    // GetMasterRate, MinGetMoney, MaxBetMoney, CastTime, DelayTime,
    // WinCode, LoseCode, UnkCol9..13
    const ShnFile* t = ShnRegistry::Get().GetTable("DiceGame");
    if (!t) { SHINELOG_WARN("DiceGame.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        LegacyDiceGameRow r;
        r.uiItemID        = (uint16)ShnGetU32(*t, i, "ItemID");
        r.uiUseMinLv      = (uint16)ShnGetU32(*t, i, "UseMinLv");
        r.uiGetSysRate    = (uint16)ShnGetU32(*t, i, "GetSysRate");
        r.uiGetMasterRate = (uint16)ShnGetU32(*t, i, "GetMasterRate");
        r.uiMinGetMoney   =          ShnGetU32(*t, i, "MinGetMoney");
        r.uiMaxBetMoney   =          ShnGetU32(*t, i, "MaxBetMoney");
        r.uiCastTime      = (uint16)ShnGetU32(*t, i, "CastTime");
        r.uiDelayTime     = (uint16)ShnGetU32(*t, i, "DelayTime");
        r.iWinCode        = (int32) ShnGetI32(*t, i, "WinCode");
        r.iLoseCode       = (int32) ShnGetI32(*t, i, "LoseCode");
        r.aUnkCode[0]     = (int32) ShnGetI32(*t, i, "UnkCol9");
        r.aUnkCode[1]     = (int32) ShnGetI32(*t, i, "UnkCol10");
        r.aUnkCode[2]     = (int32) ShnGetI32(*t, i, "UnkCol12");
        r.aUnkCode[3]     = (int32) ShnGetI32(*t, i, "UnkCol13");
        m_kRows.push_back(r);
    }
    SHINELOG_INFO("DiceGame: %u bowls", (uint32)m_kRows.size());
    return !m_kRows.empty();
}

const LegacyDiceGameRow* DiceGameTable::FindByItem(uint16 uiItemID) const {
    for (size_t i = 0; i < m_kRows.size(); ++i) {
        if (m_kRows[i].uiItemID == uiItemID) return &m_kRows[i];
    }
    return NULL;
}

} // namespace fiesta
