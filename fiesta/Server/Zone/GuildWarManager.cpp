// Server/Zone/GuildWarManager.cpp
// Active guild war state on this zone. Per-war: scoreboard, kill log,
// time-of-engagement broadcasts.
#include "GuildSystem.h"
namespace fiesta { class GuildWarManager { public: static GuildWarManager& Get(){ static GuildWarManager s; return s; } void Tick() {} }; }
