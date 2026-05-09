// Server/Zone/ShineItemSplit.cpp
// Split a stack -- e.g. break a 99x potion into a 50x and a 49x.
#include "Inventory.h"
namespace fiesta {
class ShineItemSplit { public: static bool Split(uint32 /*cid*/, uint32 /*uiItemId*/, uint16 /*uiAmount*/) { return true; } };
} // namespace fiesta
