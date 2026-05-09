// Server/Zone/MoverDataBox.cpp
// Per-mount data -- speed, jump-rise, HP pool, skin slot. Sourced from
// MoverData.shn at boot.
#include "../DataReader/ShnRegistry.h"
namespace fiesta { namespace {
class MoverDataBox {
public:
    static MoverDataBox& Get() { static MoverDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MoverData") != NULL; }
};
}} // anonymous
