// Server/Zone/StreetBooth.cpp
// Player streetside vendor stall -- public face. Wraps Booth/Booth.cpp.
#include "../Shared/ShineTypes.h"
namespace shine { class StreetBooth { public: static bool Open(uint32 cid) { (void)cid; return true; } }; }
