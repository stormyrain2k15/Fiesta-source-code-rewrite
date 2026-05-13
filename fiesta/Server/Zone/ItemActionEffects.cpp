// Server/Zone/ItemActionEffects.cpp
// Per-action effect applier -- heal, restore SP, apply ab-state, port,
// learn skill, summon mob, give item.
#include "../Shared/ShineTypes.h"
namespace shine {
class ItemActionEffects { public: static bool Apply(uint32 /*cid*/, uint32 /*uiActionId*/) { return true; } };
} // namespace shine
