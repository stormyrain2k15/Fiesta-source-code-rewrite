// Server/Zone/BmpDataBox.cpp
// BMP.shn ground-effect data -- per-pixel terrain code on the world map
// (water / lava / poison / safe / no-mount). Lookup by integer pixel.
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
class BmpDataBox {
public:
    static BmpDataBox& Get() { static BmpDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("BMP") != NULL; }
    uint8 KindAt(uint16 /*uiMap*/, float /*x*/, float /*y*/) const { return 0; }
};
} // namespace fiesta
