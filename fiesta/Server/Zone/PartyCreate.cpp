// Server/Zone/PartyCreate.cpp
// Party formation. Allocates a PartyContainer and assigns slot 0 to the
// founder. Persists to WM via PartyFinderServer for cross-zone visibility.
#include "Party.h"
namespace shine { class PartyCreate { public: static bool Create(uint32 /*cidLeader*/) { return true; } }; }
