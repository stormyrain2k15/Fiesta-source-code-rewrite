// Server/Zone/SetItemData.cpp
// Set-bonus table -- equipping a full set yields bonus stats. Sourced
// from SetItem.shn; applied at equipment-summary build time.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class SetItemData {
public:
    static SetItemData& Get() { static SetItemData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("SetItem") != NULL; }
};
}} // anonymous
