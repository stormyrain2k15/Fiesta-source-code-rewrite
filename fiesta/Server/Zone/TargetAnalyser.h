// Server/Zone/TargetAnalyser.h
// WIRE-04 (Lyra, May 2026)
#ifndef FIESTA_ZONE_TARGETANALYSER_H
#define FIESTA_ZONE_TARGETANALYSER_H

namespace fiesta {
class ShineObject;

// Returns true if pkA can legally target pkT for combat:
//   - both objects exist and pkA != pkT
//   - same map
//   - pkT is alive (HP > 0)
//   - distance-squared between A and T <= fMaxDistSq
//   - faction gate: mob-vs-mob rejected (no friendly-fire on mobs)
//   - (optional) per-map PvP flag enforcement when FieldTable is wired
bool IsLegalTarget(ShineObject* pkA, ShineObject* pkT, float fMaxDistSq);

} // namespace fiesta
#endif
