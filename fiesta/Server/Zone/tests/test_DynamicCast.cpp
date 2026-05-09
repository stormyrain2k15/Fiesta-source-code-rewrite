// Server/Zone/tests/test_DynamicCast.cpp
// VS2010 RTTI sanity -- ShineObject* dynamic_cast to derived types.
#include "TestBase.h"
#include "../ShineObject.h"

namespace fiesta {

TEST_CASE(DynamicCast_PlayerToObject) {
    ShinePlayer* p = new ShinePlayer();
    ShineObject* o = p;
    ShinePlayer* back = dynamic_cast<ShinePlayer*>(o);
    ASSERT_TRUE(back != NULL);
    delete p;
    return true;
}

} // namespace fiesta
