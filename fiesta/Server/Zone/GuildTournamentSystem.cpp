// Server/Zone/GuildTournamentSystem.cpp
// GT system glue -- match maker, kill credit, reward distribution.
#include "GuildSystem.h"
namespace shine { class GuildTournamentSystem { public: static GuildTournamentSystem& Get(){ static GuildTournamentSystem s; return s; } }; }
