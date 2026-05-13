// Server/DataReader/SHN/KingdomQuestRew.cpp
// Auto-generated: one-file-per-SHN split for KingdomQuestRew.shn
#include "KingdomQuestRew.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

KingdomQuestRewShn& KingdomQuestRewShn::Get() { static KingdomQuestRewShn s; return s; }

void KingdomQuestRewShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("KingdomQuestRew");
    if (!t) { SHINELOG_WARN("KingdomQuestRew.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        KingdomQuestRewRow rec;
        rec.uiID = ShnGetU32(*t, _r, "ID");
        rec.kIndexString = ShnGetStr(*t, _r, "IndexString");
        rec.kKQBoxItemIDX = ShnGetStr(*t, _r, "KQBoxItemIDX");
        rec.iReward = (int16)ShnGetI32(*t, _r, "Reward");
        rec.iUnkCol4 = (int16)ShnGetI32(*t, _r, "UnkCol4");
        rec.iUnkCol5 = (int16)ShnGetI32(*t, _r, "UnkCol5");
        rec.iUnkCol6 = (int16)ShnGetI32(*t, _r, "UnkCol6");
        rec.iUnkCol7 = (int16)ShnGetI32(*t, _r, "UnkCol7");
        rec.iUnkCol8 = (int16)ShnGetI32(*t, _r, "UnkCol8");
        rec.iUnkCol9 = (int16)ShnGetI32(*t, _r, "UnkCol9");
        rec.iUnkCol10 = (int16)ShnGetI32(*t, _r, "UnkCol10");
        rec.iUnkCol11 = (int16)ShnGetI32(*t, _r, "UnkCol11");
        rec.iUnkCol12 = (int16)ShnGetI32(*t, _r, "UnkCol12");
        rec.iUnkCol13 = (int16)ShnGetI32(*t, _r, "UnkCol13");
        rec.iUnkCol14 = (int16)ShnGetI32(*t, _r, "UnkCol14");
        rec.iUnkCol15 = (int16)ShnGetI32(*t, _r, "UnkCol15");
        rec.iUnkCol16 = (int16)ShnGetI32(*t, _r, "UnkCol16");
        rec.iUnkCol17 = (int16)ShnGetI32(*t, _r, "UnkCol17");
        rec.iRewardRate = (int16)ShnGetI32(*t, _r, "RewardRate");
        rec.iUnkCol19 = (int16)ShnGetI32(*t, _r, "UnkCol19");
        rec.iUnkCol20 = (int16)ShnGetI32(*t, _r, "UnkCol20");
        rec.iUnkCol21 = (int16)ShnGetI32(*t, _r, "UnkCol21");
        rec.iUnkCol22 = (int16)ShnGetI32(*t, _r, "UnkCol22");
        rec.iUnkCol23 = (int16)ShnGetI32(*t, _r, "UnkCol23");
        rec.iUnkCol24 = (int16)ShnGetI32(*t, _r, "UnkCol24");
        rec.iUnkCol25 = (int16)ShnGetI32(*t, _r, "UnkCol25");
        rec.iUnkCol26 = (int16)ShnGetI32(*t, _r, "UnkCol26");
        rec.iUnkCol27 = (int16)ShnGetI32(*t, _r, "UnkCol27");
        rec.iUnkCol28 = (int16)ShnGetI32(*t, _r, "UnkCol28");
        rec.iUnkCol29 = (int16)ShnGetI32(*t, _r, "UnkCol29");
        rec.iUnkCol30 = (int16)ShnGetI32(*t, _r, "UnkCol30");
        rec.iUnkCol31 = (int16)ShnGetI32(*t, _r, "UnkCol31");
        rec.iUnkCol32 = (int16)ShnGetI32(*t, _r, "UnkCol32");
        m_kById[rec.uiID] = m_kRows.size();
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("KingdomQuestRew.shn: %u rows", (uint32)m_kRows.size());
}

const KingdomQuestRewRow* KingdomQuestRewShn::FindById(uint32 uiID) const {
    std::map<uint32, size_t>::const_iterator it = m_kById.find(uiID);
    return (it == m_kById.end()) ? NULL : &m_kRows[it->second];
}

} // namespace shine
