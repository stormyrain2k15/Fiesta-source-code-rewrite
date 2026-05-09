// Server/Zone/PineScript/PineScriptMobRegen.cpp
// PineScript "MobRegen" / "MobKill" / "MobClear" commands. Bridges the
// scripted scenario layer into the live MobSpawnSystem.
#include "PineScript.h"
#include "../MobSpawnSystem.h"
#include "../../Shared/ShineLogSystem.h"
#include <stdlib.h>

namespace fiesta {

bool PSCmdMobRegen(const PSStatement& rSt, PSProgram& /*rProg*/) {
    if (rSt.kCmd == "MobRegen" && rSt.kArgs.size() >= 4) {
        // MobRegen <npcId> <x> <y> <count>
        uint32 npcId = (uint32)atoi(rSt.kArgs[0].c_str());
        int32  x     = atoi(rSt.kArgs[1].c_str());
        int32  y     = atoi(rSt.kArgs[2].c_str());
        uint16 cnt   = (uint16)atoi(rSt.kArgs[3].c_str());
        for (uint16 i = 0; i < cnt; ++i)
            MobSpawnSystem::Get().SpawnAt(npcId, (float)x, (float)y);
        SHINELOG_INFO("PS MobRegen npc=%u x=%d y=%d count=%u", npcId, x, y, (uint32)cnt);
        return true;
    }
    if (rSt.kCmd == "MobClear") {
        MobSpawnSystem::Get().ClearAll();
        return true;
    }
    return false;
}

} // namespace fiesta
