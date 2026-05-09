// Server/Zone/TradeBoard.cpp
// Trade-board NPC -- stores listings persistently across player logins
// (unlike StreetBooth which requires the seller online).
#include "../Shared/ShineTypes.h"
namespace fiesta { class TradeBoard { public: static bool ListForSale(uint32, uint64, uint16, uint32) { return true; } }; }
