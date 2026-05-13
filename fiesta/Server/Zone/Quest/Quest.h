// Server/Zone/Quest/Quest.h
// Per-character quest runtime. Each player carries up to N active quests;
// each quest tracks per-objective counters / flags persisted via
// CharDBClient::QuestSet. Quest acceptance, abandonment, completion, and
// reward grant all flow through this module.
#ifndef SHINE_ZONE_QUEST_QUEST_H
#define SHINE_ZONE_QUEST_QUEST_H
#include "../../Shared/ShineTypes.h"
#include <map>

namespace shine {

class ShinePlayer;

struct QuestState {
    uint32 uiQuestId;
    uint8  uiStep;
    uint16 aCounters[8];
    uint64 uiStartedAtMs;
    bool   bComplete;
};

class Quest {
public:
    static Quest& Get();
    bool Accept   (ShinePlayer* pkP, uint32 uiQuestId);
    bool Abandon  (ShinePlayer* pkP, uint32 uiQuestId);
    bool Complete (ShinePlayer* pkP, uint32 uiQuestId);
    void OnMobKill(ShinePlayer* pkP, uint32 uiNpcID);
    void OnItemPick(ShinePlayer* pkP, uint32 uiItemId);
private:
    Quest() {}
    std::map<uint32, std::map<uint32, QuestState> > m_kPerChar; // char -> qid -> state
};

} // namespace shine
#endif
