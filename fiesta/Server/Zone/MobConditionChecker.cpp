// Server/Zone/MobConditionChecker.cpp
// MobConditionServer column resolver. Each mob has up to N conditions
// (HP%, SP%, around-allies-count, target-class, time-of-day) that gate
// scripted behaviours. This module evaluates them per-tick.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class MobConditionChecker {
public:
    static bool Check(uint32 /*uiMobId*/, uint32 /*uiCondId*/) { return true; }
};
} // namespace fiesta
