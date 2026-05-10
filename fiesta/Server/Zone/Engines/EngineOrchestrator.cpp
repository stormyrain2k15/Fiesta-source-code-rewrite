// Server/Zone/Engines/EngineOrchestrator.cpp
#include "AbStateEngine.h"
#include "CharEngine.h"
#include "CollectEngine.h"
#include "GambleHouseEngine.h"
#include "GuildEngine.h"
#include "HouseEngine.h"
#include "ItemEngine.h"
#include "MapEngine.h"
#include "MiscEngine.h"
#include "MobEngine.h"
#include "MountEngine.h"
#include "NPCEngine.h"
#include "PartyEngine.h"
#include "PupEngine.h"
#include "QuestEngine.h"
#include "SkillEngine.h"
#include "UpgradeEngine.h"
#include "../../Shared/ShineLogSystem.h"
namespace fiesta {
void BindAllEngines(const std::string& rShineRoot) {
    AbStateEngine::Get().Bind();
    CharEngine::Get().Bind(rShineRoot);
    CollectEngine::Get().Bind();
    GambleHouseEngine::Get().Bind();
    GuildEngine::Get().Bind();
    HouseEngine::Get().Bind();
    ItemEngine::Get().Bind();
    MapEngine::Get().Bind();
    MiscEngine::Get().Bind();
    MobEngine::Get().Bind();
    MountEngine::Get().Bind();
    NPCEngine::Get().Bind();
    PartyEngine::Get().Bind();
    PupEngine::Get().Bind();
    QuestEngine::Get().Bind();
    SkillEngine::Get().Bind();
    UpgradeEngine::Get().Bind();
    SHINELOG_INFO("BindAllEngines: complete");
}
} // namespace fiesta
