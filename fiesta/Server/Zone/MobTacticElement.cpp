// Server/Zone/MobTacticElement.cpp
// Tactical element -- one decision unit in the mob brain (e.g. "if HP <
// 30% cast Heal"). Loaded from MobTactic.shn and composed per-mob.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class MobTacticElement {
public:
    static MobTacticElement& Get() { static MobTacticElement s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MobTactic") != NULL; }
};
} // namespace shine
