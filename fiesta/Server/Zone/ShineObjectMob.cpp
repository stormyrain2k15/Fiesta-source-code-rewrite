// Server/Zone/ShineObjectMob.cpp
// Mob-specific overrides. Spawn/despawn/respawn windows + drop emission
// helpers. Combat-side mob behaviour lives under MobAISystem and the AI
// runner; this file keeps the per-class symbol space NA2016-compatible.
#include "ShineObject.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

void ShineMob_OnSpawn  (ShineMob* pkM) { if (pkM) SHINELOG_DEBUG("MobSpawn id=%u", pkM->GetCharID()); }
void ShineMob_OnDespawn(ShineMob* pkM) { if (pkM) SHINELOG_DEBUG("MobDespawn id=%u", pkM->GetCharID()); }

} // namespace shine
