// Server/Zone/Tables/SpamerTables.cpp
// FEATURE: world-creation -- SpamerPenalty + SpamerPenaltyRule + SpamerReport.
#include "BindMacros.h"
#include "../MoreTables.h"

namespace fiesta {
SpamerTables& SpamerTables::Get() { static SpamerTables s; return s; }
void SpamerTables::Bind() {
    // FEATURE: world-creation -- column read: PenaltyLv, ChatBlockTime,
    // ProbateTime, SecondRule, RepeatRule, SR_Term, SR_Number, SR_Message
    if (const ShnFile* t = ShnRegistry::Get().GetTable("SpamerPenalty"))
        for (size_t r=0;r<t->Rows().size();++r) {
            Penalty p; p.uiChatBlockSec=ShnGetU32(*t,r,"ChatBlockTime"); p.uiProbateSec=ShnGetU32(*t,r,"ProbateTime");
            m_kPenalty[ShnGetU32(*t,r,"PenaltyLv")]=p;
        }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("SpamerPenaltyRule"))
        for (size_t r=0;r<t->Rows().size();++r) {
            Rule x; x.uiSecond=ShnGetU32(*t,r,"SecondRule"); x.uiRepeat=ShnGetU32(*t,r,"RepeatRule");
            m_kRules.push_back(x);
        }
    if (const ShnFile* t = ShnRegistry::Get().GetTable("SpamerReport"))
        for (size_t r=0;r<t->Rows().size();++r) {
            ReportTerm x;
            x.uiTerm=ShnGetU32(*t,r,"SR_Term"); x.uiNumber=ShnGetU32(*t,r,"SR_Number");
            x.kMsg=ShnGetStr(*t,r,"SR_Message");
            m_kReport.push_back(x);
        }
}
bool SpamerTables::Lookup(uint32 l, Penalty& o) const {
    std::map<uint32,Penalty>::const_iterator i=m_kPenalty.find(l);
    if (i==m_kPenalty.end()) return false; o=i->second; return true;
}
bool SpamerTables::ReportThreshold(uint32 t, uint32 n) const {
    for (size_t i=0;i<m_kReport.size();++i)
        if (m_kReport[i].uiTerm==t && m_kReport[i].uiNumber==n) return true;
    return false;
}
bool SpamerTables::RepeatRuleHit(uint32 s, uint32 r) const {
    for (size_t i=0;i<m_kRules.size();++i)
        if (s<=m_kRules[i].uiSecond && r>=m_kRules[i].uiRepeat) return true;
    return false;
}
} // namespace fiesta
