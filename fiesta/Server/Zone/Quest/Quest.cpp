// Server/Zone/Quest/Quest.cpp
// Per-character quest runtime. The class declaration is in Quest.h above.
// This .cpp owns the state map + DB write-through.
#include "Quest.h"
#include "../ShineObject.h"
#include "../CharDBClient.h"
#include "../../Shared/GTimer.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

Quest& Quest::Get() { static Quest s; return s; }

bool Quest::Accept(ShinePlayer* pkP, uint32 uiQuestId) {
    if (!pkP) return false;
    QuestState& s = m_kPerChar[pkP->GetCharID()][uiQuestId];
    s.uiQuestId = uiQuestId; s.uiStep = 1; s.bComplete = false;
    s.uiStartedAtMs = GTimer::NowMillis();
    for (int i = 0; i < 8; ++i) s.aCounters[i] = 0;
    CharDBClient::Get().QuestSet(pkP->GetCharID(), uiQuestId, s.uiStep, 0);
    return true;
}

bool Quest::Abandon(ShinePlayer* pkP, uint32 uiQuestId) {
    if (!pkP) return false;
    m_kPerChar[pkP->GetCharID()].erase(uiQuestId);
    CharDBClient::Get().QuestSet(pkP->GetCharID(), uiQuestId, -1, 0);
    return true;
}

bool Quest::Complete(ShinePlayer* pkP, uint32 uiQuestId) {
    if (!pkP) return false;
    std::map<uint32, QuestState>& q = m_kPerChar[pkP->GetCharID()];
    std::map<uint32, QuestState>::iterator it = q.find(uiQuestId);
    if (it == q.end()) return false;
    it->second.bComplete = true;
    CharDBClient::Get().QuestSet(pkP->GetCharID(), uiQuestId, 0xFF, 1);
    SHINELOG_INFO("Quest complete cid=%u qid=%u", pkP->GetCharID(), uiQuestId);
    return true;
}

void Quest::OnMobKill(ShinePlayer* /*pkP*/, uint32 /*uiNpcID*/) {}
void Quest::OnItemPick(ShinePlayer* /*pkP*/, uint32 /*uiItemId*/) {}

} // namespace shine
