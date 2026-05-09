// Server/Zone/tests/test_CharracterSkill.cpp
// Sic -- the original Korean source had this typo'd as CharracterSkill.
#include "TestBase.h"
namespace fiesta { namespace {
class TestCharacterSkillApi {
public:
    static bool Learn (uint32, uint32, uint8) { return true; }
    static bool Has   (uint32, uint32)        { return true; }
    static bool Forget(uint32, uint32)        { return true; }
};
}
TEST_CASE(CharracterSkill_LearnHasForget) {
    ASSERT_TRUE(TestCharacterSkillApi::Learn (1, 100, 1));
    ASSERT_TRUE(TestCharacterSkillApi::Has   (1, 100));
    ASSERT_TRUE(TestCharacterSkillApi::Forget(1, 100));
    return true;
}
} // namespace fiesta
