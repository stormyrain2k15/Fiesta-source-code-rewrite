// Server/Zone/TradeMisc.cpp
#include "../Shared/ShineTypes.h"
namespace shine { class TradeMisc { public: static uint32 ListingFee(uint32 uiAskPrice) { return uiAskPrice / 100; } }; }
