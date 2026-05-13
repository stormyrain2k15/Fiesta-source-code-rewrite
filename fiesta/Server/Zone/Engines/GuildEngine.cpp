// Server/Zone/Engines/GuildEngine.cpp
#include "GuildEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

GuildEngine& GuildEngine::Get() { static GuildEngine s; return s; }

void GuildEngine::Bind() {
    GTIBreedSubjectShn::Get().Load();
    GTIGetRateShn::Get().Load();
    GTIGetRateGapShn::Get().Load();
    GTWinScoreShn::Get().Load();
    GuildAcademyShn::Get().Load();
    GuildAcademyLevelUpShn::Get().Load();
    GuildAcademyRankShn::Get().Load();
    GuildGradeDataShn::Get().Load();
    GuildGradeScoreDataShn::Get().Load();
    GuildLevelScoreDataShn::Get().Load();
    GuildTournamentLvGapShn::Get().Load();
    GuildTournamentMasterBuffShn::Get().Load();
    GuildTournamentOccupyShn::Get().Load();
    GuildTournamentRequireShn::Get().Load();
    GuildTournamentRewardShn::Get().Load();
    GuildTournamentScoreShn::Get().Load();
    SHINELOG_INFO("GuildEngine::Bind done");
}

} // namespace shine
