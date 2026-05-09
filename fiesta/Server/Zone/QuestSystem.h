// Server/Zone/QuestSystem.h
// 20 -- quest framework. Quest SHN files are NOT parsed here; this consumes
// only PineScript bytecode (handled in QuestParserScript) and runtime hooks.
// EVIDENCE: PDB_CONFIRMED  symbol: Quest, QuestData, QuestParserScript, QuestFramework,
//                                  QuestEvent, CharQuest, ShineQuestDiary, PineScript
#ifndef FIESTA_ZONE_QUESTSYSTEM_H
#define FIESTA_ZONE_QUESTSYSTEM_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <vector>
#include <string>

namespace fiesta {

class ShinePlayer;

enum QuestState { QS_NONE = 0, QS_ACTIVE = 1, QS_COMPLETE = 2, QS_FAILED = 3 };

struct CharQuestEntry {
    uint32     uiQuestId;
    QuestState eState;
    uint32     auiCounters[8];
};

class CharQuest {
public:
    bool   Begin   (uint32 uiQuestId);
    bool   Finish  (uint32 uiQuestId);
    bool   GiveUp  (uint32 uiQuestId);
    void   OnKill  (MobID uiSpecies);
    QuestState State(uint32 uiQuestId) const;
    const std::vector<CharQuestEntry>& Entries() const { return m_kEntries; }
private:
    std::vector<CharQuestEntry> m_kEntries;
};

class QuestFramework {
public:
    static QuestFramework& Get();
    void OnPlayerLogin (ShinePlayer* pk);
    void OnPlayerLogout(ShinePlayer* pk);
    void Tick();
};

class QuestParserScript {
public:
    // Loads compiled PineScript bytecode (.pis), NOT raw quest SHN.
    // Returns false if a quest SHN was passed by mistake -- spec rule 02.
    static bool LoadCompiled(const std::string& rPath);
};

// Per-character quest counter dispatcher used by Battle / NPC-talk hooks.
// Lives outside QuestFramework so it can be called from kill-handlers
// without taking a write lock on the framework.
class QuestProgress {
public:
    static QuestProgress& Get();
    void OnMobKilled (CharID c, MobID uiSpecies);
    void OnNpcTalked (CharID c, uint32 uiNpcId);
    void OnItemUsed  (CharID c, ItemID uiItem);
private:
    QuestProgress() {}
};

class QuestEvent {
public:
    static void Fire(ShinePlayer* pk, uint32 uiEvent, uint32 uiArg);
};

class ShineQuestDiary {
public:
    static void OpenDiary(ShinePlayer* pk);
};

} // namespace fiesta
#endif
