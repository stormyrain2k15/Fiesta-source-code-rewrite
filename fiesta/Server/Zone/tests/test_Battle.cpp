// Server/Zone/tests/test_Battle.cpp
// Validates BuildBattleStat composes class baseline + free stat + equip.
#include "TestBase.h"
#include "../StatDistribute.h"

namespace fiesta {

TEST_CASE(Battle_StatCompose_BaseOnly) {
    RAWCHARSTAT raw; raw.Clear();
    raw.nLevel = 10;  raw.bClass = 1;
    raw.nBaseHP = 100; raw.nBaseSP = 50;
    raw.nBaseATK = 10; raw.nBaseDEF = 5;
    BATTLESTAT out;
    BuildBattleStat(&out, &raw, NULL, NULL);
    ASSERT_TRUE(out.nMaxHP >= 100);
    ASSERT_TRUE(out.nATK   >= 10);
    return true;
}

} // namespace fiesta
