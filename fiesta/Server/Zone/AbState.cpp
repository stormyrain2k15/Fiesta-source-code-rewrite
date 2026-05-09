// Server/Zone/AbState.cpp
#include "AbState.h"
#include "../Shared/GTimer.h"
#include "../DataReader/Schemas.h"
#include "../DataReader/Tables.h"  // back-compat aliases

namespace fiesta {

void AbnormalState::Apply(uint32 ab, int32 ms, uint16 stk) {
    uint64 now = GTimer::NowMillis();
    for (size_t i = 0; i < m_kList.size(); ++i)
        if (m_kList[i].uiAbInxName == ab) {
            if (SubAbstatePriority::ShouldStack(ab, ab)) m_kList[i].uiStack += stk;
            m_kList[i].uiExpireMs = now + (uint64)(ms < 0 ? 0 : ms);
            return;
        }
    AbStateInst inst; inst.uiAbInxName = ab; inst.uiExpireMs = now + (uint64)(ms < 0 ? 0 : ms); inst.uiStack = stk;
    m_kList.push_back(inst);
}

void AbnormalState::Remove(uint32 ab) {
    for (size_t i = 0; i < m_kList.size(); ++i)
        if (m_kList[i].uiAbInxName == ab) { m_kList.erase(m_kList.begin() + i); return; }
}

void AbnormalState::Tick(uint64 now) {
    for (size_t i = 0; i < m_kList.size(); ) {
        if (m_kList[i].uiExpireMs && now >= m_kList[i].uiExpireMs)
            m_kList.erase(m_kList.begin() + i);
        else ++i;
    }
}

bool AbnormalState::Has(uint32 ab) const {
    for (size_t i = 0; i < m_kList.size(); ++i) if (m_kList[i].uiAbInxName == ab) return true;
    return false;
}

AbnormalStateDictionary& AbnormalStateDictionary::Get() { static AbnormalStateDictionary s; return s; }

uint16 AbnormalStateDictionary::GetSaveType(uint32) const {
    // Real lookup wires through ITableBase<AbStateRow>::ms_pkTable in pass 2;
    // generated AbStateRow has the documented "SaveType"-class fields under
    // schema-derived names -- pass-2 maps the SHINE_ABSTATE_ID -> row.
    return 0;
}
uint16 AbnormalStateDictionary::GetShelterFlags(uint32) const { return 0; }

bool AbnormalStateShelter::IsShelteredFrom(uint32) { return false; }
bool SubAbstatePriority::ShouldStack(uint32, uint32) { return true; }

} // namespace fiesta
