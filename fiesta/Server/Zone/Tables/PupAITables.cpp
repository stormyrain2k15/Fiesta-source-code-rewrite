// Server/Zone/Tables/PupAITables.cpp
// FEATURE: world-creation -- PupMind.shn + PupPriority.shn +
// PupCaseDesc.shn + PupFactorCondition.shn binders. Pet AI brain.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {

PupAITables& PupAITables::Get() { static PupAITables s; return s; }

void PupAITables::Bind() {
    // FEATURE: world-creation -- column read: PupMindType, MinMind, MaxMind
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupMind")) {
        ITER_ROWS(t) {
            Mind r;
            r.uiType = ShnGetU32(*t, _r, "PupMindType");
            r.iMin   = ShnGetI32(*t, _r, "MinMind");
            r.iMax   = ShnGetI32(*t, _r, "MaxMind");
            m_kMindByType[r.uiType] = m_kMind.size();
            m_kMind.push_back(r);
        }
    }
    // FEATURE: world-creation -- column read: PupPriorityType, PriorityNum
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupPriority")) {
        ITER_ROWS(t) {
            Priority r;
            r.uiType = ShnGetU32(*t, _r, "PupPriorityType");
            r.uiNum  = ShnGetU32(*t, _r, "PriorityNum");
            m_kPrioByType[r.uiType] = m_kPrio.size();
            m_kPrio.push_back(r);
        }
    }
    // FEATURE: world-creation -- column read: PupPriorityType, PupCaseType,
    // PupIDX, PupAIType, SM_Inx, ActionEffectID, SoundFile
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupCaseDesc")) {
        ITER_ROWS(t) {
            CaseDesc r;
            r.uiPriorityType   = ShnGetU32(*t, _r, "PupPriorityType");
            r.uiCaseType       = ShnGetU32(*t, _r, "PupCaseType");
            r.kPupIDX          = ShnGetStr(*t, _r, "PupIDX");
            r.uiAIType         = ShnGetU32(*t, _r, "PupAIType");
            r.kSMInx           = ShnGetStr(*t, _r, "SM_Inx");
            r.uiActionEffectID = ShnGetU32(*t, _r, "ActionEffectID");
            r.kSoundFile       = ShnGetStr(*t, _r, "SoundFile");
            m_kCase.push_back(r);
        }
    }
    // FEATURE: world-creation -- column read: PupMindType,
    // PupFactorConditionType, PupFactorType, IsMinus, Value
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PupFactorCondition")) {
        ITER_ROWS(t) {
            Factor r;
            r.uiMindType            = ShnGetU32(*t, _r, "PupMindType");
            r.uiFactorConditionType = ShnGetU32(*t, _r, "PupFactorConditionType");
            r.uiFactorType          = ShnGetU32(*t, _r, "PupFactorType");
            r.uiIsMinus             = ShnGetU32(*t, _r, "IsMinus");
            r.iValue                = ShnGetI32(*t, _r, "Value");
            m_kFactor.push_back(r);
        }
    }
}

const PupAITables::Mind* PupAITables::FindMind(uint32 uT) const {
    std::map<uint32, size_t>::const_iterator it = m_kMindByType.find(uT);
    return (it == m_kMindByType.end()) ? NULL : &m_kMind[it->second];
}
const PupAITables::Priority* PupAITables::FindPriority(uint32 uT) const {
    std::map<uint32, size_t>::const_iterator it = m_kPrioByType.find(uT);
    return (it == m_kPrioByType.end()) ? NULL : &m_kPrio[it->second];
}
void PupAITables::FactorsByMind(uint32 uM, std::vector<const Factor*>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kFactor.size(); ++i)
        if (m_kFactor[i].uiMindType == uM) rOut.push_back(&m_kFactor[i]);
}
void PupAITables::CasesByPriority(uint32 uP, std::vector<const CaseDesc*>& rOut) const {
    rOut.clear();
    for (size_t i = 0; i < m_kCase.size(); ++i)
        if (m_kCase[i].uiPriorityType == uP) rOut.push_back(&m_kCase[i]);
}

} // namespace fiesta
