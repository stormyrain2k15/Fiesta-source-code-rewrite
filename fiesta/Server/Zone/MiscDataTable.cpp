// Server/Zone/MiscDataTable.cpp
// Catch-all for the small misc tables: ExpandSkill, AbStateReset,
// SkillBreedMob, Polymorph, Neutralization. Each is a flat shn that the
// SkillSystem queries at runtime.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class MiscDataTable {
public:
    static MiscDataTable& Get() { static MiscDataTable s; return s; }
    bool Load() {
        const char* aTabs[] = { "ExpandSkill", "AbStateReset", "SkillBreedMob",
                                "Polymorph", "Neutralization" };
        bool bAny = false;
        for (int i = 0; i < 5; ++i) if (ShnRegistry::Get().GetTable(aTabs[i])) bAny = true;
        return bAny;
    }
};
} // namespace fiesta
