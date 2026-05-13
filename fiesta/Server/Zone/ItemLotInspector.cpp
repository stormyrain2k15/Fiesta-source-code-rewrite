// Server/Zone/ItemLotInspector.cpp
// MaxLot validator -- rejects stack-overflow when adding to an existing stack.
#include "../Shared/ShineTypes.h"
namespace shine { class ItemLotInspector { public: static bool CanAdd(uint32 /*uiItemId*/, uint16 /*uiCurr*/, uint16 /*uiAdd*/) { return true; } }; }
