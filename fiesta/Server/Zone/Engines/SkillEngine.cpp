// Server/Zone/Engines/SkillEngine.cpp
// Auto-generated: SkillEngine — calls Load() on each constituent SHN.
#include "SkillEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

SkillEngine& SkillEngine::Get() { static SkillEngine s; return s; }

void SkillEngine::Bind() {
    ActiveSkillGroupShn::Get().Load();
    ActiveSkillInfoServerShn::Get().Load();
    AreaSkillShn::Get().Load();
    GuildTournamentSkillShn::Get().Load();
    MoverUseSkillShn::Get().Load();
    PSkillSetAbstateShn::Get().Load();
    PassiveSkillShn::Get().Load();
    ToggleSkillShn::Get().Load();
    SHINELOG_INFO("SkillEngine::Bind done");
}

} // namespace shine
