// Server/Zone/GambleHouse/GambleObjectManager.cpp
// Registry over every placed GambleObject. Loaded at zone boot from
// Data/Gamble/<map>.gob. Looked up by handle on player interact.
#include "../ShineObject.h"
namespace shine {
class GambleObjectManager {
public:
    static GambleObjectManager& Get() { static GambleObjectManager s; return s; }
    void LoadFor(uint16 /*uiMap*/) {}
};
} // namespace shine
