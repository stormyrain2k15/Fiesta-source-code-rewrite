// Server/Zone/tests/test_UtilFunction.cpp
#include "TestBase.h"
#include "../UtilFunction.cpp"

namespace fiesta {

TEST_CASE(UtilFunction_Clamp) {
    ASSERT_EQ(UtilFunction::Clamp<int>(5,  0, 10),  5);
    ASSERT_EQ(UtilFunction::Clamp<int>(-1, 0, 10),  0);
    ASSERT_EQ(UtilFunction::Clamp<int>(99, 0, 10), 10);
    return true;
}
TEST_CASE(UtilFunction_SignOf) {
    ASSERT_EQ(UtilFunction::SignOf(-7), -1);
    ASSERT_EQ(UtilFunction::SignOf( 0),  0);
    ASSERT_EQ(UtilFunction::SignOf( 9),  1);
    return true;
}

} // namespace fiesta
