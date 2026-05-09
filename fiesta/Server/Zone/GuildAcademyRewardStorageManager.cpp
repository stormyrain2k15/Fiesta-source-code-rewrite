// Server/Zone/GuildAcademyRewardStorageManager.cpp
// Per-academy reward storage. Members earn entries; reward is granted
// on graduation (academy member level cap).
#include "Inventory.h"
namespace fiesta { class GuildAcademyRewardStorageManager { public: static GuildAcademyRewardStorageManager& Get(){ static GuildAcademyRewardStorageManager s; return s; } }; }
