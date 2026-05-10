// Server/Zone/Engines/NPCEngine.cpp
// NPCEngine — calls Load() on each constituent SHN.
#include "NPCEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

NPCEngine& NPCEngine::Get() { static NPCEngine s; return s; }

void NPCEngine::Bind() {
    NPCViewInfoShn::Get().Load();
    NpcScheduleShn::Get().Load();
    SHINELOG_INFO("NPCEngine::Bind done");
}

} // namespace fiesta
