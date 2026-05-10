// Server/Zone/Tables/PartyBonusTables.cpp
// FEATURE: world-creation -- PartyBonusByLvDiff + PartyBonusByMember
// + PartyBonusLimit. Three coupled SHNs sharing one PartyBonusTables.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {
PartyBonusTables& PartyBonusTables::Get() { static PartyBonusTables s; return s; }
void PartyBonusTables::Bind() {
    // FEATURE: world-creation -- column read: PB_LvDiff, PB_BonusRatio,
    // PB_PartyMember, PSE_ChrLv, PSE_ExpLimit
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PartyBonusByLvDiff"))
        for (size_t r=0;r<t->Rows().size();++r) m_kByDiff[ShnGetI32(*t,r,"PB_LvDiff")] = ShnGetI32(*t,r,"PB_BonusRatio");
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PartyBonusByMember"))
        for (size_t r=0;r<t->Rows().size();++r) m_kByMember[ShnGetU32(*t,r,"PB_PartyMember")] = ShnGetI32(*t,r,"PB_BonusRatio");
    if (const ShnFile* t = ShnRegistry::Get().GetTable("PartyBonusLimit"))
        for (size_t r=0;r<t->Rows().size();++r) m_kLimit[ShnGetU32(*t,r,"PSE_ChrLv")] = ShnGetI32(*t,r,"PSE_ExpLimit");
}
int32 PartyBonusTables::ByLvDiff(int32 d) const  { std::map<int32,int32>::const_iterator i=m_kByDiff.find(d);  return i==m_kByDiff.end()?1000:i->second; }
int32 PartyBonusTables::ByMember(uint32 n) const { std::map<uint32,int32>::const_iterator i=m_kByMember.find(n); return i==m_kByMember.end()?1000:i->second; }
int32 PartyBonusTables::ChrLvLimit(uint32 l) const { std::map<uint32,int32>::const_iterator i=m_kLimit.find(l); return i==m_kLimit.end()?0:i->second; }
} // namespace fiesta
