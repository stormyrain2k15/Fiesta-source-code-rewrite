// Server/Zone/Engines/HouseEngine.cpp
// HouseEngine — calls Load() on each constituent SHN.
#include "HouseEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

HouseEngine& HouseEngine::Get() { static HouseEngine s; return s; }

void HouseEngine::Bind() {
    CharacterTitleStateServerShn::Get().Load();
    MiniHouseShn::Get().Load();
    MiniHouseDummyShn::Get().Load();
    MiniHouseEndureShn::Get().Load();
    MiniHouseFurnitureShn::Get().Load();
    MiniHouseFurnitureObjEffectShn::Get().Load();
    MiniHouseObjAniShn::Get().Load();
    SHINELOG_INFO("HouseEngine::Bind done");
}

} // namespace shine
