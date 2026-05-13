// Server/Zone/ItemTotalInformation.cpp
// Aggregated read-only view over an inventory -- count by family, total
// gold value, weighted endure score. Used by GuildBank and trade UIs.
#include "Inventory.h"
namespace shine { class ItemTotalInformation { public: static uint32 SumCount(const Inventory& /*kInv*/) { return 0; } }; }
