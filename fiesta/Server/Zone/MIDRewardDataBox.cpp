// Server/Zone/MIDRewardDataBox.cpp
// MID (Match Instance Dungeon) reward data.
// CN2012-only feature -- absent from NA2016. Safe to leave as a no-op stub.
// confirmed
// CN2012 only, no implementation needed for NA2016 target.
#include "../DataReader/ShnRegistry.h"
namespace shine {
// Intentionally empty for NA2016. The class name is preserved for PDB
// slot parity. Do not add implementation unless targeting CN build.
class MIDRewardDataBox {
public:
    static MIDRewardDataBox& Get() { static MIDRewardDataBox s; return s; }
    bool Load() { return true; } // CN2012 only -- no-op is correct
};
} // namespace shine
