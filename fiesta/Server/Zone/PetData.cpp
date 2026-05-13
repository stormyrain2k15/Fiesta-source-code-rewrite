// Server/Zone/PetData.cpp
// Pet runtime state -- current HP, fullness, mood, learned skills.
// Sourced from PetData.shn at boot.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class PetData {
public:
    static PetData& Get() { static PetData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("PetData") != NULL; }
};
} // namespace shine
