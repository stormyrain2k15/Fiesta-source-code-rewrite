// Server/Zone/DropLocationManager.cpp
// Where dropped items physically land. Avoids stacking on top of the
// dropper / mob corpse; spreads drops in a small radius.
#include "../Shared/ShineTypes.h"
namespace shine {
class DropLocationManager {
public:
    static void NextLocation(float cx, float cy, uint8 /*uiIndex*/, float& x, float& y) {
        x = cx + 30.0f; y = cy + 30.0f;
    }
};
} // namespace shine
