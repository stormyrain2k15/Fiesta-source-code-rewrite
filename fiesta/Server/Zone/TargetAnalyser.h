// Server/Zone/TargetAnalyser.h
#ifndef SHINE_ZONE_TARGETANALYSER_H
#define SHINE_ZONE_TARGETANALYSER_H

namespace shine {
class ShineObject;

// Returns true if pkA can legally target pkT for combat:
//   - both objects exist and pkA != pkT
//   - same map
//   - pkT is alive (HP > 0)
//   - distance-squared between A and T <= fMaxDistSq
//   - faction gate: mob-vs-mob rejected (no friendly-fire on mobs)
//   - (optional) per-map PvP flag enforcement when FieldTable is wired
bool IsLegalTarget(ShineObject* pkA, ShineObject* pkT, float fMaxDistSq);

} // namespace shine
#endif
