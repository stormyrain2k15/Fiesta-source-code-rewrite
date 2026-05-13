// Server/Zone/MapDataBox.cpp
// Per-map metadata -- name, kind (field/town/dungeon/instance), default
// spawn, gravity, weather, BGM. Sourced from MapInfo.shn.
#include "../DataReader/ShnRegistry.h"
namespace shine { namespace {
class MapDataBox {
public:
    static MapDataBox& Get() { static MapDataBox s; return s; }
    bool Load() { return ShnRegistry::Get().GetTable("MapInfo") != NULL; }
};
}} // anonymous
