// Server/Zone/Engines/QuestEngine.cpp
// Auto-generated: QuestEngine — calls Load() on each constituent SHN.
#include "QuestEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

QuestEngine& QuestEngine::Get() { static QuestEngine s; return s; }

void QuestEngine::Bind() {
    KQIsVoteShn::Get().Load();
    KQItemShn::Get().Load();
    KQTeamShn::Get().Load();
    KQVoteMajorityRateShn::Get().Load();
    KingdomQuestShn::Get().Load();
    KingdomQuestMapShn::Get().Load();
    KingdomQuestRewShn::Get().Load();
    QuestDataShn::Get().Load();
    QuestDialogShn::Get().Load();
    QuestScriptShn::Get().Load();
    QuestSpeciesShn::Get().Load();
    SHINELOG_INFO("QuestEngine::Bind done");
}

} // namespace shine
