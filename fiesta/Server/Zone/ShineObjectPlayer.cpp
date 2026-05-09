// Server/Zone/ShineObjectPlayer.cpp
// Player-specific overrides on top of ShineObject. The bulk of the player
// runtime (login, stat compose, inventory) lives in CharLogin.cpp /
// EquipSummaryBuilder.cpp / Inventory.cpp; this file holds the overrides
// that the original split kept on ShineObjectPlayer (admin level checks,
// damage aggregation, etc.).
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

// Helper -- send the BAT_LEVELUP CMD body. The actual broadcast lives in
// CharLogin's leveling path; keep the helper here so the symbol pattern
// matches the NA2016 PDB.
void ShinePlayer_OnLevelUp(ShinePlayer* pkP) {
    if (!pkP) return;
    SHINELOG_INFO("Player level-up cid=%u newLv=%u", pkP->GetCharID(), pkP->GetLevel());
}

} // namespace fiesta
