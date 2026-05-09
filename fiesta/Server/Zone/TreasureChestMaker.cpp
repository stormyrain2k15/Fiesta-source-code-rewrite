// Server/Zone/TreasureChestMaker.cpp
// Spawn a treasure chest at runtime -- used by KQ rewards, GM events,
// scripted scenes. Chest is a special ItemDrop with a per-player open
// roll.
#include "../Shared/ShineTypes.h"
namespace fiesta { class TreasureChestMaker { public: static bool SpawnAt(uint16, float, float, uint32) { return true; } }; }
