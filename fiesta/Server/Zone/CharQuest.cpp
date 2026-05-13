// Server/Zone/CharQuest.cpp
// Per-character quest state container. Persistent rows live in CharDB
// (p_Quest_Set / p_Quest_Get); this is the in-memory mirror.
#include "Quest/Quest.h"
namespace shine { class CharQuest { public: static void Hydrate(uint32 /*cid*/) {} }; }
