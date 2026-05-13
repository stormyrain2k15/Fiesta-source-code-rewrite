// Server/Zone/ChargedItem/ChargedItemEffectList.cpp
// Per-charged-buff effect list -- which BATTLESTAT fields the buff
// modifies and by how much. Sourced from ChargedItemEffect.shn.
#include "../../DataReader/ShnRegistry.h"
namespace shine {
class ChargedItemEffectList {
public:
    static ChargedItemEffectList& Get() { static ChargedItemEffectList s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("ChargedItemEffect") != NULL; }
};
} // namespace shine
