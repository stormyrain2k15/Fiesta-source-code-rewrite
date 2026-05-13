// Server/Zone/RaidSystem.cpp
// Raid -- cross-party group of up to 8 parties. Used for boss raids
// outside of instances. Loot rules: master, boss-fcfs, round-robin.
#include "../Shared/ShineTypes.h"
namespace shine { class RaidSystem { public: static RaidSystem& Get(){ static RaidSystem s; return s; } }; }
