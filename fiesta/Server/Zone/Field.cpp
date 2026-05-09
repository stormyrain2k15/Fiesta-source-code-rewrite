// Server/Zone/Field.cpp
// Field/map instance management. A Field is one logical map; the Zone
// hosts up to 32 fields concurrently. Each field owns its mob list,
// drop list, player list, and PineScript runner state.
//
// The concrete `class Field` lives in `MapField.h` / `MapField.cpp`.
// This translation unit exists only to anchor the `Field.cpp` symbol
// in the canonical NA2016 PDB layout; no code -- just a TU placeholder.
#include "MapField.h"
namespace fiesta { } // namespace fiesta
