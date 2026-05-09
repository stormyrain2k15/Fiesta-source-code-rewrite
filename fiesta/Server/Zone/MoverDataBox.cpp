// Server/Zone/MoverDataBox.cpp
// Per-mount data -- speed, jump-rise, HP pool, skin slot. Sourced from
// MoverMain.shn at boot. (Earlier code referenced "MoverData" -- the
// actual NA2016 file ships as MoverMain.shn.)
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class MoverDataBox {
public:
    static MoverDataBox& Get() { static MoverDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MoverMain") != NULL; }
    const ShnFile* Table() const { return ShnRegistry::Get().GetTable("MoverMain"); }
};
} // namespace fiesta
