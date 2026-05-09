// Server/Zone/DefSkillData.cpp
// Defensive / passive skill entries -- per-class default skills granted
// on level up (e.g. mage gets MagicArmor at 10).
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class DefSkillData {
public:
    static DefSkillData& Get() { static DefSkillData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("DefSkill") != NULL; }
};
} // namespace fiesta
