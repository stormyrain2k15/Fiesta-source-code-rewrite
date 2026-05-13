// Server/Zone/Tables/AbStateTables.cpp
// FEATURE: world-creation -- AbState.shn binder.
// Effect-engine root. Without this, every buff/debuff/abnormal state
// returns "unknown" and skill effects, mob auras, item-use buffs and
// passive aura ticks all cascade into noops. The single most
// load-bearing data table in the gameplay layer.
#include "BindMacros.h"
#include "../GroupTables.h"

namespace shine {

AbStateTables& AbStateTables::Get() { static AbStateTables s; return s; }

void AbStateTables::Bind() {
    // FEATURE: world-creation -- column read: ID, InxName, AbStataIndex,
    // KeepTimeRatio, KeepTimePower, StateGrade, PartyState1..5,
    // PartyRange, PartyEnchantNumber, SubAbState, DispelIndex,
    // SubDispelIndex, AbStateSaveType, MainStateInx, Duplicate
    BIND_BEGIN(t, "AbState")
    m_kRows.reserve(t->Rows().size());
    ITER_ROWS(t) {
        AbStateRow rec;
        rec.uiID                   = ShnGetU32(*t, _r, "ID");
        rec.kInxName               = ShnGetStr(*t, _r, "InxName");
        rec.uiAbStataIndex         = ShnGetU32(*t, _r, "AbStataIndex");
        rec.uiKeepTimeRatio        = ShnGetU32(*t, _r, "KeepTimeRatio");
        rec.uiKeepTimePower        = ShnGetU32(*t, _r, "KeepTimePower");
        rec.uiStateGrade           = ShnGetU32(*t, _r, "StateGrade");
        rec.kPartyState[0]         = ShnGetStr(*t, _r, "PartyState1");
        rec.kPartyState[1]         = ShnGetStr(*t, _r, "PartyState2");
        rec.kPartyState[2]         = ShnGetStr(*t, _r, "PartyState3");
        rec.kPartyState[3]         = ShnGetStr(*t, _r, "PartyState4");
        rec.kPartyState[4]         = ShnGetStr(*t, _r, "PartyState5");
        rec.uiPartyRange           = ShnGetU32(*t, _r, "PartyRange");
        rec.uiPartyEnchantNumber   = ShnGetU32(*t, _r, "PartyEnchantNumber");
        rec.kSubAbState            = ShnGetStr(*t, _r, "SubAbState");
        rec.uiDispelIndex          = ShnGetU32(*t, _r, "DispelIndex");
        rec.uiSubDispelIndex       = ShnGetU32(*t, _r, "SubDispelIndex");
        rec.uiAbStateSaveType      = ShnGetU32(*t, _r, "AbStateSaveType");
        rec.kMainStateInx          = ShnGetStr(*t, _r, "MainStateInx");
        rec.uiDuplicate            = ShnGetU32(*t, _r, "Duplicate");
        m_kById[rec.uiID]       = m_kRows.size();
        m_kByInx[rec.kInxName]  = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("AbState.shn: %u rows", (uint32)m_kRows.size());
}

const AbStateRow* AbStateTables::Find(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

const AbStateRow* AbStateTables::FindByInx(const std::string& rN) const {
    std::map<std::string, size_t>::const_iterator it = m_kByInx.find(rN);
    return (it == m_kByInx.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
