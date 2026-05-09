// Server/Zone/SkillDataBox.cpp
// ActiveSkill/PassiveSkill SHN aggregation. Existing SkillSystem.cpp
// already loads these; this file is the canonical-named alias.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class SkillDataBox {
public:
    static SkillDataBox& Get() { static SkillDataBox s; return s; }
    bool Load() {
        return ShnRegistry::Get().GetTable("ActiveSkill") != NULL
            && ShnRegistry::Get().GetTable("PassiveSkill") != NULL;
    }
};
}} // anonymous
