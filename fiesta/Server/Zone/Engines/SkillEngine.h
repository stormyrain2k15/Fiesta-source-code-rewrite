// Server/Zone/Engines/SkillEngine.h
// Auto-generated: SkillEngine — aggregates 8 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_SKILLENGINE_H
#define FIESTA_ZONE_ENGINES_SKILLENGINE_H
#include "../../DataReader/SHN/ActiveSkillGroup.h"
#include "../../DataReader/SHN/ActiveSkillInfoServer.h"
#include "../../DataReader/SHN/AreaSkill.h"
#include "../../DataReader/SHN/GuildTournamentSkill.h"
#include "../../DataReader/SHN/MoverUseSkill.h"
#include "../../DataReader/SHN/PSkillSetAbstate.h"
#include "../../DataReader/SHN/PassiveSkill.h"
#include "../../DataReader/SHN/ToggleSkill.h"
#include <vector>
#include <string>

namespace fiesta {

class SkillEngine {
public:
    static SkillEngine& Get();
    void Bind();

    // Per-SHN accessors
    ActiveSkillGroupShn& activeSkillGroup() { return ActiveSkillGroupShn::Get(); }
    ActiveSkillInfoServerShn& activeSkillInfoServer() { return ActiveSkillInfoServerShn::Get(); }
    AreaSkillShn& areaSkill() { return AreaSkillShn::Get(); }
    GuildTournamentSkillShn& guildTournamentSkill() { return GuildTournamentSkillShn::Get(); }
    MoverUseSkillShn& moverUseSkill() { return MoverUseSkillShn::Get(); }
    PSkillSetAbstateShn& pSkillSetAbstate() { return PSkillSetAbstateShn::Get(); }
    PassiveSkillShn& passiveSkill() { return PassiveSkillShn::Get(); }
    ToggleSkillShn& toggleSkill() { return ToggleSkillShn::Get(); }

private:
    SkillEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_SKILLENGINE_H
