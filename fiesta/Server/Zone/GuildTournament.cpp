// Server/Zone/GuildTournament.cpp
// GT zone-side runtime. Schedule: every Tuesday 13:00, 25h duration
// (GuildTournament.shn). Score = 20 / 60s occupy.
#include "GuildSystem.h"
namespace fiesta { class GuildTournament { public: static GuildTournament& Get(){ static GuildTournament s; return s; } void Tick() {} }; }
