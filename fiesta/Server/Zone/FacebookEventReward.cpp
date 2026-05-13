// Server/Zone/FacebookEventReward.cpp
// Legacy Facebook integration -- claim daily reward by posting to FB.
// Disabled in NA2016; the symbol is kept so any caller still resolves.
#include "../Shared/ShineTypes.h"
namespace shine { class FacebookEventReward { public: static bool Claim(uint32 /*cid*/) { return false; } }; }
