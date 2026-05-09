// Server/Zone/ItemGroupClassifier.cpp
// Group classifier -- maps items to display groups for the inventory UI
// (potion / weapon / armor / etc.).
#include "../Shared/ShineTypes.h"
namespace fiesta {
class ItemGroupClassifier { public: static uint8 Group(uint32 /*uiItemId*/) { return 0; } };
} // namespace fiesta
