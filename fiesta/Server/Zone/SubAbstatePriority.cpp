// Server/Zone/SubAbstatePriority.cpp
// SubAbState priority resolver. Some ab-states have multiple sub-effects
// (e.g. a "Sleep" master state with sub-effects like "no-skill" + "no-
// move"); their stacking order is resolved here.
#include "../Shared/ShineTypes.h"
namespace fiesta { namespace {
class SubAbstatePriority {
public:
    static int32 Compare(uint32 uiA, uint32 uiB) { return (int32)uiA - (int32)uiB; }
};
}} // anonymous
