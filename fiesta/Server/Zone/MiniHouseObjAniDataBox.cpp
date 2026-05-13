// Server/Zone/MiniHouseObjAniDataBox.cpp
// Per-furniture-piece animation data -- looped idle / interaction sequence.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class MiniHouseObjAniDataBox {
public:
    static MiniHouseObjAniDataBox& Get() { static MiniHouseObjAniDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MiniHouseObjAni") != NULL; }
};
} // namespace shine
