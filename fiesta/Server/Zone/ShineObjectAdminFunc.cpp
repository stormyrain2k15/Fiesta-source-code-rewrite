// Server/Zone/ShineObjectAdminFunc.cpp
// GM-on-object admin funcs. Triggered through &commands (parser is in
// AmpersandCommands.cpp); this file is the thin destination for the
// per-object effect (kill / teleport / freeze / spawn-mob).
#include "ShineObject.h"
#include "ZoneServer.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

void AdminKill(ShineObject* pkObj) {
    if (!pkObj) return;
    pkObj->SetHP(0);
    SHINELOG_WARN("[GM] kill on obj=%u", pkObj->GetCharID());
}

void AdminTeleport(ShineObject* pkObj, int32 x, int32 y) {
    if (!pkObj) return;
    pkObj->SetPos((float)x, 0.0f, (float)y);
    SHINELOG_WARN("[GM] teleport obj=%u -> (%d,%d)", pkObj->GetCharID(), x, y);
}

void AdminSpawnMob(uint32 uiNpcID, int32 x, int32 y, uint16 uiCount) {
    SHINELOG_WARN("[GM] spawnmob npc=%u count=%u at (%d,%d)", uiNpcID, (uint32)uiCount, x, y);
}

} // namespace shine
