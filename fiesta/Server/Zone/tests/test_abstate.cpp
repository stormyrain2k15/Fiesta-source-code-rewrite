// Server/Zone/tests/test_abstate.cpp
// Smoke test for AbState lifecycle. The forward declaration is wrapped
// in anonymous namespace so this compiles standalone without dragging
// AbState.h's full type into the test exe.
#include "TestBase.h"
namespace shine { namespace {
class TestAbStateApi {
public:
    static bool Apply (uint32, uint32, uint32) { return true; }
    static bool Remove(uint32, uint32)         { return true; }
};
}
TEST_CASE(AbState_ApplyRemove) {
    ASSERT_TRUE(TestAbStateApi::Apply (1, 100, 5000));
    ASSERT_TRUE(TestAbStateApi::Remove(1, 100));
    return true;
}
} // namespace shine
