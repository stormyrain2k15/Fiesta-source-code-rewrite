// Server/Zone/GambleHouse/DiceTaiSai/ShineDiceTaiSai.cpp
// "Tai Sai" (sic-bo style) dice game. Three d6, players bet on small (4-10),
// big (11-17), specific triples, etc. Outcomes are computed server-side
// via the WM's authoritative RNG (GambleSystemServer::RollDice).
#include "../../../Shared/ShineLogSystem.h"
namespace shine {
class ShineDiceTaiSai { public: static ShineDiceTaiSai& Get(){ static ShineDiceTaiSai s; return s; } };
} // namespace shine
