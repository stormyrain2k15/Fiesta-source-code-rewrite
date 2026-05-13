// Server/Zone/NearScan.h
// Proximity-scan helpers. Used by AreaSkill, AOE buffs, KQ score, and
// Lua's `cFindNearestObject` binding.
#ifndef SHINE_ZONE_NEARSCAN_H
#define SHINE_ZONE_NEARSCAN_H
#include "../Shared/ShineTypes.h"
#include <vector>

namespace shine {

class ShinePlayer;
class ShineObject;
class Field;
struct Vec3;

class NearScan {
public:
    // Append all `ShinePlayer*` on `uiMap` whose (x, z) lies within
    // radius `r` of (cx, cy). Caller is responsible for clearing rOut
    // beforehand if they want to merge with prior results.
    static void Players(uint16 uiMap, float cx, float cy, float r,
                        std::vector<ShinePlayer*>& rOut);

    // Legacy entry kept for callers that target a specific `Field`.
    // Mirrors the old MoveManager::NearScan::Run signature; visibility
    // radius defaults to 30 world units (per the comment in the
    // original declaration).
    static void RunOnField(Field& rField, const Vec3& kFrom,
                           std::vector<ShineObject*>& rOut);
};

} // namespace shine
#endif
