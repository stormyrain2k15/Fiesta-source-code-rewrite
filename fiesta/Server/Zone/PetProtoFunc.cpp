// Server/Zone/PetProtoFunc.cpp
// Pet proto functions -- summon / unsummon, feed, play. Each maps to a
// stored proc on CharDB for the per-pet row.
#include "../Shared/ShineTypes.h"
namespace shine { class PetProtoFunc { public: static bool Summon(uint32, uint64) { return true; } }; }
