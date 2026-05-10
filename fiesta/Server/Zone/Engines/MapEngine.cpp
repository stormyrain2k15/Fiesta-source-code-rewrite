// Server/Zone/Engines/MapEngine.cpp
// Auto-generated: MapEngine — calls Load() on each constituent SHN.
#include "MapEngine.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

MapEngine& MapEngine::Get() { static MapEngine s; return s; }

void MapEngine::Bind() {
    BMPShn::Get().Load();
    FieldLvConditionShn::Get().Load();
    MapInfoShn::Get().Load();
    MapLinkPointShn::Get().Load();
    MapWayPointShn::Get().Load();
    SHINELOG_INFO("MapEngine::Bind done");
}

} // namespace fiesta
