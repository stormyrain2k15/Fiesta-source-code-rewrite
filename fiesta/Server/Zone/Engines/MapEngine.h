// Server/Zone/Engines/MapEngine.h
// Auto-generated: MapEngine — aggregates 5 SHN loader(s).
#ifndef FIESTA_ZONE_ENGINES_MAPENGINE_H
#define FIESTA_ZONE_ENGINES_MAPENGINE_H
#include "../../DataReader/SHN/BMP.h"
#include "../../DataReader/SHN/FieldLvCondition.h"
#include "../../DataReader/SHN/MapInfo.h"
#include "../../DataReader/SHN/MapLinkPoint.h"
#include "../../DataReader/SHN/MapWayPoint.h"
#include <vector>
#include <string>

namespace fiesta {

class MapEngine {
public:
    static MapEngine& Get();
    void Bind();

    // Per-SHN accessors
    BMPShn& bMP() { return BMPShn::Get(); }
    FieldLvConditionShn& fieldLvCondition() { return FieldLvConditionShn::Get(); }
    MapInfoShn& mapInfo() { return MapInfoShn::Get(); }
    MapLinkPointShn& mapLinkPoint() { return MapLinkPointShn::Get(); }
    MapWayPointShn& mapWayPoint() { return MapWayPointShn::Get(); }

private:
    MapEngine() {}
};

} // namespace fiesta
#endif // FIESTA_ZONE_ENGINES_MAPENGINE_H
