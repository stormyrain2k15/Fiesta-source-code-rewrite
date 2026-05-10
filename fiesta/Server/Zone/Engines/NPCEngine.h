// Server/Zone/Engines/NPCEngine.h
// NPCEngine — aggregates 2 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_NPCENGINE_H
#define FIESTA_ZONE_ENGINES_NPCENGINE_H
#include "../../DataReader/SHN/NPCViewInfo.h"
#include "../../DataReader/SHN/NpcSchedule.h"
#include <vector>
#include <string>

namespace fiesta {

class NPCEngine {
public:
    static NPCEngine& Get();
    void Bind();

    // Per-SHN accessors
    NPCViewInfoShn& nPCViewInfo() { return NPCViewInfoShn::Get(); }
    NpcScheduleShn& npcSchedule() { return NpcScheduleShn::Get(); }

private:
    NPCEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_NPCENGINE_H
