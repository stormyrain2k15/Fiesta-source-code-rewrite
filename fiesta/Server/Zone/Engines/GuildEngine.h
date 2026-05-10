// Server/Zone/Engines/GuildEngine.h
// GuildEngine — aggregates 16 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_GUILDENGINE_H
#define FIESTA_ZONE_ENGINES_GUILDENGINE_H
#include "../../DataReader/SHN/GTIBreedSubject.h"
#include "../../DataReader/SHN/GTIGetRate.h"
#include "../../DataReader/SHN/GTIGetRateGap.h"
#include "../../DataReader/SHN/GTWinScore.h"
#include "../../DataReader/SHN/GuildAcademy.h"
#include "../../DataReader/SHN/GuildAcademyLevelUp.h"
#include "../../DataReader/SHN/GuildAcademyRank.h"
#include "../../DataReader/SHN/GuildGradeData.h"
#include "../../DataReader/SHN/GuildGradeScoreData.h"
#include "../../DataReader/SHN/GuildLevelScoreData.h"
#include "../../DataReader/SHN/GuildTournamentLvGap.h"
#include "../../DataReader/SHN/GuildTournamentMasterBuff.h"
#include "../../DataReader/SHN/GuildTournamentOccupy.h"
#include "../../DataReader/SHN/GuildTournamentRequire.h"
#include "../../DataReader/SHN/GuildTournamentReward.h"
#include "../../DataReader/SHN/GuildTournamentScore.h"
#include <vector>
#include <string>

namespace fiesta {

class GuildEngine {
public:
    static GuildEngine& Get();
    void Bind();

    // Per-SHN accessors
    GTIBreedSubjectShn& gTIBreedSubject() { return GTIBreedSubjectShn::Get(); }
    GTIGetRateShn& gTIGetRate() { return GTIGetRateShn::Get(); }
    GTIGetRateGapShn& gTIGetRateGap() { return GTIGetRateGapShn::Get(); }
    GTWinScoreShn& gTWinScore() { return GTWinScoreShn::Get(); }
    GuildAcademyShn& guildAcademy() { return GuildAcademyShn::Get(); }
    GuildAcademyLevelUpShn& guildAcademyLevelUp() { return GuildAcademyLevelUpShn::Get(); }
    GuildAcademyRankShn& guildAcademyRank() { return GuildAcademyRankShn::Get(); }
    GuildGradeDataShn& guildGradeData() { return GuildGradeDataShn::Get(); }
    GuildGradeScoreDataShn& guildGradeScoreData() { return GuildGradeScoreDataShn::Get(); }
    GuildLevelScoreDataShn& guildLevelScoreData() { return GuildLevelScoreDataShn::Get(); }
    GuildTournamentLvGapShn& guildTournamentLvGap() { return GuildTournamentLvGapShn::Get(); }
    GuildTournamentMasterBuffShn& guildTournamentMasterBuff() { return GuildTournamentMasterBuffShn::Get(); }
    GuildTournamentOccupyShn& guildTournamentOccupy() { return GuildTournamentOccupyShn::Get(); }
    GuildTournamentRequireShn& guildTournamentRequire() { return GuildTournamentRequireShn::Get(); }
    GuildTournamentRewardShn& guildTournamentReward() { return GuildTournamentRewardShn::Get(); }
    GuildTournamentScoreShn& guildTournamentScore() { return GuildTournamentScoreShn::Get(); }

private:
    GuildEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_GUILDENGINE_H
