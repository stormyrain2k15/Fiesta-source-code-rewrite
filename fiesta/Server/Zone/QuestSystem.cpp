// Server/Zone/QuestSystem.cpp
#include "QuestSystem.h"
#include "ZoneServer.h"
#include "../Shared/ShineLogSystem.h"
#include <string.h>

namespace shine {

static bool EndsWith(const std::string& s, const char* sfx) {
    size_t n = strlen(sfx); return s.size() >= n && _stricmp(s.c_str()+s.size()-n, sfx) == 0;
}

bool CharQuest::Begin(uint32 q) {
    for (size_t i = 0; i < m_kEntries.size(); ++i)
        if (m_kEntries[i].uiQuestId == q) return false;
    CharQuestEntry e; ZeroMemory(&e, sizeof(e)); e.uiQuestId = q; e.eState = QS_ACTIVE;
    m_kEntries.push_back(e); return true;
}
bool CharQuest::Finish(uint32 q) {
    for (size_t i = 0; i < m_kEntries.size(); ++i)
        if (m_kEntries[i].uiQuestId == q && m_kEntries[i].eState == QS_ACTIVE) {
            m_kEntries[i].eState = QS_COMPLETE; return true;
        }
    return false;
}
bool CharQuest::GiveUp(uint32 q) {
    for (size_t i = 0; i < m_kEntries.size(); ++i)
        if (m_kEntries[i].uiQuestId == q) { m_kEntries.erase(m_kEntries.begin()+i); return true; }
    return false;
}

void CharQuest::FinishKeyAdd(uint32 uiKey) {
    // Insertion sort into the sorted vector (typical key counts: < 32).
    for (size_t i = 0; i < m_kFinishKeys.size(); ++i) {
        if (m_kFinishKeys[i] == uiKey) return;
        if (m_kFinishKeys[i] >  uiKey) {
            m_kFinishKeys.insert(m_kFinishKeys.begin() + i, uiKey);
            return;
        }
    }
    m_kFinishKeys.push_back(uiKey);
}
bool CharQuest::FinishKeyHas(uint32 uiKey) const {
    // Linear scan -- the vector is small enough that binary-search adds
    // more code than it saves.
    for (size_t i = 0; i < m_kFinishKeys.size(); ++i)
        if (m_kFinishKeys[i] == uiKey) return true;
    return false;
}
void CharQuest::OnKill(MobID s) {
    for (size_t i = 0; i < m_kEntries.size(); ++i)
        if (m_kEntries[i].eState == QS_ACTIVE)
            m_kEntries[i].auiCounters[0] += (s ? 1 : 0);
}
QuestState CharQuest::State(uint32 q) const {
    for (size_t i = 0; i < m_kEntries.size(); ++i)
        if (m_kEntries[i].uiQuestId == q) return m_kEntries[i].eState;
    return QS_NONE;
}

QuestFramework& QuestFramework::Get() { static QuestFramework s; return s; }
void QuestFramework::OnPlayerLogin (ShinePlayer*) {}
void QuestFramework::OnPlayerLogout(ShinePlayer*) {}
void QuestFramework::Tick() {}

bool QuestParserScript::LoadCompiled(const std::string& rPath) {
    // Spec rule 02: refuse anything that smells like a raw quest SHN.
    if (EndsWith(rPath, ".shn")) {
        SHINELOG_ERROR("QuestParserScript: refused raw quest SHN (%s) -- protected", rPath.c_str());
        return false;
    }
    if (!EndsWith(rPath, ".pis") && !EndsWith(rPath, ".pinec")) {
        SHINELOG_WARN("QuestParserScript: unknown quest bytecode ext for %s", rPath.c_str());
        return false;
    }
    SHINELOG_INFO("QuestParserScript: pretend-loaded %s", rPath.c_str());
    return true;
}

void QuestEvent::Fire(ShinePlayer*, uint32, uint32) {}
void ShineQuestDiary::OpenDiary(ShinePlayer*) {}

QuestEventDispatcher& QuestEventDispatcher::Get() { static QuestEventDispatcher s; return s; }
void QuestEventDispatcher::OnMobKilled(CharID c, MobID uiSpecies) {
    ShinePlayer* pk = ZoneServer::Get().FindPlayerByCharID(c);
    if (!pk) return;
    pk->Quest().OnKill(uiSpecies);
}
void QuestEventDispatcher::OnNpcTalked(CharID c, uint32 uiNpcId) {
    QuestEvent::Fire(ZoneServer::Get().FindPlayerByCharID(c), 1 /*ETALK*/, uiNpcId);
}
void QuestEventDispatcher::OnItemUsed(CharID c, ItemID uiItem) {
    QuestEvent::Fire(ZoneServer::Get().FindPlayerByCharID(c), 2 /*EUSE*/, (uint32)uiItem);
}

} // namespace shine
