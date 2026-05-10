// Server/Zone/Engines/QuestEngine.h
// Auto-generated: QuestEngine — aggregates 11 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_QUESTENGINE_H
#define FIESTA_ZONE_ENGINES_QUESTENGINE_H
#include "../../DataReader/SHN/KQIsVote.h"
#include "../../DataReader/SHN/KQItem.h"
#include "../../DataReader/SHN/KQTeam.h"
#include "../../DataReader/SHN/KQVoteMajorityRate.h"
#include "../../DataReader/SHN/KingdomQuest.h"
#include "../../DataReader/SHN/KingdomQuestMap.h"
#include "../../DataReader/SHN/KingdomQuestRew.h"
#include "../../DataReader/SHN/QuestData.h"
#include "../../DataReader/SHN/QuestDialog.h"
#include "../../DataReader/SHN/QuestScript.h"
#include "../../DataReader/SHN/QuestSpecies.h"
#include <vector>
#include <string>

namespace fiesta {

class QuestEngine {
public:
    static QuestEngine& Get();
    void Bind();

    // Per-SHN accessors
    KQIsVoteShn& kQIsVote() { return KQIsVoteShn::Get(); }
    KQItemShn& kQItem() { return KQItemShn::Get(); }
    KQTeamShn& kQTeam() { return KQTeamShn::Get(); }
    KQVoteMajorityRateShn& kQVoteMajorityRate() { return KQVoteMajorityRateShn::Get(); }
    KingdomQuestShn& kingdomQuest() { return KingdomQuestShn::Get(); }
    KingdomQuestMapShn& kingdomQuestMap() { return KingdomQuestMapShn::Get(); }
    KingdomQuestRewShn& kingdomQuestRew() { return KingdomQuestRewShn::Get(); }
    QuestDataShn& questData() { return QuestDataShn::Get(); }
    QuestDialogShn& questDialog() { return QuestDialogShn::Get(); }
    QuestScriptShn& questScript() { return QuestScriptShn::Get(); }
    QuestSpeciesShn& questSpecies() { return QuestSpeciesShn::Get(); }

private:
    QuestEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_QUESTENGINE_H
