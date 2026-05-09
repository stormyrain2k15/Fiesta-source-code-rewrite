// Server/Zone/MIDRewardDataBox.cpp
// MID (Match Instance Dungeon) reward data -- CN-only feature. Stubbed
// in NA2016 but we keep the loader so the table registration doesn't
// log as missing.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class MIDRewardDataBox {
public:
    static MIDRewardDataBox& Get() { static MIDRewardDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MIDReward") != NULL; }
};
}} // anonymous
