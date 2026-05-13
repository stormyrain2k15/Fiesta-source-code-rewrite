// Server/Zone/ItemRandomOption.cpp
// Random option roller -- equipment can roll up to 4 random options
// (e.g. +5% crit, +30 ATK). Sourced from RandomOption.shn.
#include "../DataReader/ShnRegistry.h"
namespace shine {
class ItemRandomOption {
public:
    static ItemRandomOption& Get() { static ItemRandomOption s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("RandomOption") != NULL; }
    bool Roll(uint32 /*uiItemId*/, uint16* /*pOpts*/, uint8 /*uiOptCount*/) { return true; }
};
} // namespace shine
