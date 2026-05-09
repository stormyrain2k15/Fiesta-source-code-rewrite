// Server/Zone/PetRelatedSkill.cpp
// Per-pet skill list -- the skills the pet can use in combat.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class PetRelatedSkill {
public:
    static PetRelatedSkill& Get() { static PetRelatedSkill s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("PetRelatedSkill") != NULL; }
};
}} // anonymous
