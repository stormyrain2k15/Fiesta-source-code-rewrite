// Server/Zone/ItemMixData.cpp
// Item-fusion / craft recipe table -- input materials -> output. Sourced
// from ItemMix.shn. Used by the Gather/Craft system.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class ItemMixData {
public:
    static ItemMixData& Get() { static ItemMixData s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("ItemMix") != NULL; }
};
} // namespace shine
