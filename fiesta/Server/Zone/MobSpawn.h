// Server/Zone/MobSpawn.h
// ShineMob construction helpers. All callers should funnel through
// SpawnMob() so the per-species level / max HP / aggro radii come straight
// from MobInfo.shn / MobInfoServer.shn instead of being filled by the
// caller.
#ifndef FIESTA_ZONE_MOBSPAWN_H
#define FIESTA_ZONE_MOBSPAWN_H
#include "ShineObject.h"

namespace fiesta {

// Returns a heap-allocated ShineMob populated from the typed tables. The
// caller owns the pointer and is responsible for AttachShineObject /
// AttachToMap. Returns NULL if no MobInfo row exists for `uiSpecies`.
ShineMob* SpawnMob(MobID uiSpecies, MapID uiMap, const Vec3& rPos);

} // namespace fiesta
#endif
