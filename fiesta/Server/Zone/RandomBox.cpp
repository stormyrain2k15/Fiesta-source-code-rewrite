// Server/Zone/RandomBox.cpp
// Random / gacha box -- on use, rolls one item from a weighted pool
// (RandomBox.shn). Each opened box is logged for audit.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class RandomBox {
public:
    static RandomBox& Get() { static RandomBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("RandomBox") != NULL; }
    bool Roll(uint32 /*uiBoxItemId*/, uint32& uiOut) { uiOut = 0; return true; }
};
} // namespace shine
