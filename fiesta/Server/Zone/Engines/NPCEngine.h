// Server/Zone/Engines/NPCEngine.h
// NPCEngine — aggregates 2 SHN loader(s).
#ifndef SHINE_ZONE_ENGINES_NPCENGINE_H
#define SHINE_ZONE_ENGINES_NPCENGINE_H
#include "../../DataReader/SHN/NPCViewInfo.h"
#include "../../DataReader/SHN/NpcSchedule.h"
#include <vector>
#include <string>

namespace shine {

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

} // namespace shine
#endif // SHINE_ZONE_ENGINES_NPCENGINE_H
