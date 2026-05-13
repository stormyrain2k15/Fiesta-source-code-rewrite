// Server/Zone/Tables/MiscTables2.cpp
// FEATURE: world-creation -- 5 small SHNs share the MiscTables2 surface:
//   RaceNameInfo, ReactionType, TermExtendMatch, RareMoverRate,
//   RandomOptionCount.
// Listed as "one Get(), multiple SHNs" exception per docs/PER_SHN_CONVENTION.md
// because each table is 1-3 columns and the original NA2016 source
// shipped them as a single MiscTables2 binder pair.
#include "BindMacros.h"
#include "../MoreTables.h"
#include <stdio.h>

namespace shine {
MiscTables2& MiscTables2::Get() { static MiscTables2 s; return s; }
void MiscTables2::Bind() {
    // FEATURE: world-creation -- column read: RaceID, acLocalName
    if (const ShnFile* t = ShnRegistry::Get().GetTable("RaceNameInfo"))
        for (size_t r=0;r<t->Rows().size();++r)
            m_kRace[ShnGetU32(*t,r,"RaceID")] = ShnGetStr(*t,r,"acLocalName");
    // FEATURE: world-creation -- column read: MobInx, RAType, Ecode
    if (const ShnFile* t = ShnRegistry::Get().GetTable("ReactionType"))
        for (size_t r=0;r<t->Rows().size();++r) {
            char k[256];
            const std::string& mob = ShnGetStr(*t,r,"MobInx");
            uint32 raType = ShnGetU32(*t,r,"RAType");
            sprintf_s(k, sizeof(k), "%u|%s", raType, mob.c_str());
            m_kReaction[k] = ShnGetI32(*t,r,"Ecode");
        }
    // FEATURE: world-creation -- column read: ExtendItemIDX, TermItemIDX
    if (const ShnFile* t = ShnRegistry::Get().GetTable("TermExtendMatch"))
        for (size_t r=0;r<t->Rows().size();++r)
            m_kTermExtend[ShnGetStr(*t,r,"ExtendItemIDX")] = ShnGetStr(*t,r,"TermItemIDX");
    // FEATURE: world-creation -- column read: RMR_Rate
    if (const ShnFile* t = ShnRegistry::Get().GetTable("RareMoverRate"))
        for (size_t r=0;r<t->Rows().size();++r)
            m_kRareRates.push_back(ShnGetI32(*t,r,"RMR_Rate"));
    // FEATURE: world-creation -- column read: InxName, LimitCount, LimitDropRate
    if (const ShnFile* t = ShnRegistry::Get().GetTable("RandomOptionCount"))
        for (size_t r=0;r<t->Rows().size();++r) {
            ROC x;
            x.uiLimitCount    = ShnGetU32(*t,r,"LimitCount");
            x.uiLimitDropRate = ShnGetU32(*t,r,"LimitDropRate");
            m_kROC[ShnGetStr(*t,r,"InxName")] = x;
        }
}
const std::string& MiscTables2::RaceLocalName(uint32 r) const {
    static std::string e;
    std::map<uint32,std::string>::const_iterator i=m_kRace.find(r);
    return i==m_kRace.end()?e:i->second;
}
int32 MiscTables2::Reaction(uint32 ra, const std::string& m) const {
    char k[256]; sprintf_s(k,sizeof(k),"%u|%s",ra,m.c_str());
    std::map<std::string,int32>::const_iterator i=m_kReaction.find(k);
    return i==m_kReaction.end()?0:i->second;
}
bool MiscTables2::TermExtendMatch(const std::string& e, std::string& o) const {
    std::map<std::string,std::string>::const_iterator i=m_kTermExtend.find(e);
    if (i==m_kTermExtend.end()) return false; o=i->second; return true;
}
int32 MiscTables2::RareMoverRateAt(size_t i) const {
    return i<m_kRareRates.size()?m_kRareRates[i]:0;
}
bool MiscTables2::RandomOptionCount(const std::string& n, uint32& c, uint32& dr) const {
    std::map<std::string,ROC>::const_iterator i=m_kROC.find(n);
    if (i==m_kROC.end()) { c=0; dr=0; return false; }
    c=i->second.uiLimitCount; dr=i->second.uiLimitDropRate; return true;
}
} // namespace shine
