// Server/Zone/SparrowWing.cpp
// "Sparrow Wing" / Goddess Wing -- recall-coord items. Each wing item
// stores a recall point id; using the wing teleports to that point.
#include "../Shared/ShineTypes.h"
namespace fiesta {
class SparrowWing {
public:
    static bool Use(uint32 cid, uint32 uiRecallId);
};
bool SparrowWing::Use(uint32 /*cid*/, uint32 /*uiRecallId*/) { return true; }
} // namespace fiesta
