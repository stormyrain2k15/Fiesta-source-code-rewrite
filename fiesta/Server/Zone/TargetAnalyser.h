// Server/Zone/TargetAnalyser.h
// WIRE-04 (Lyra, May 2026)
#ifndef FIESTA_ZONE_TARGETANALYSER_H
#define FIESTA_ZONE_TARGETANALYSER_H

namespace fiesta {
class ShineObject;

// Returns true if pkA can legally target pkT for combat:
//   - pkT is alive (HP > 0)
//   - distance-squared between A and T <= fMaxDistSq
//   - (PvP rules and map flag checks are TODO pending FieldTable wiring)
bool IsLegalTarget(ShineObject* pkA, ShineObject* pkT, float fMaxDistSq);

} // namespace fiesta
#endif
