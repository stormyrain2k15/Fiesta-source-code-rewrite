// Server/Zone/ItemsInChest.cpp
// Per-character storage chest. Like inventory, but accessed only at
// chest NPCs. Persisted to CharDB via p_Chest_*.
#include "Inventory.h"
namespace fiesta { class ItemsInChest : public Inventory {}; }
