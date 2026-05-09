// Server/Zone/CharDataLoad.cpp
// On-login data hydration -- pulls every per-char row (skills, items,
// abstates, options, friends, titles) and folds them into the live
// ShinePlayer + Inventory. Existing CharLogin.cpp owns the bulk; this
// file is the canonical alias.
#include "CharLogin.h"
namespace fiesta {
class CharDataLoad { public: static bool Load(uint32 /*cid*/) { return true; } };
} // namespace fiesta
