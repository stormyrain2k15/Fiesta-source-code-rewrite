// Server/Zone/MobDataBox.cpp
// Decoded MobInfoServer SHN as a flat lookup. Existing aggregate
// MobRegenTable / MoreTables hold most of the per-mob runtime data;
// this canonical-named file is the discovery layer.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class MobDataBox {
public:
    static MobDataBox& Get() { static MobDataBox s; return s; }
    bool Load() {
        return ShnRegistry::Get().GetTable("MobInfoServer") != NULL
            && ShnRegistry::Get().GetTable("MobInfo")       != NULL;
    }
};
} // namespace fiesta
