// Server/Zone/MiniHouseEndureDataBox.cpp
// Per-house endure rules. Endure decays over time; at 0 the house
// "expires" and contents go to a pickup chest.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class MiniHouseEndureDataBox {
public:
    static MiniHouseEndureDataBox& Get() { static MiniHouseEndureDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MiniHouseEndure") != NULL; }
};
} // namespace shine
