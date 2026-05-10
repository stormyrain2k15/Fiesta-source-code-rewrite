// Server/DataReader/SHN/MapWayPoint.h
// Auto-generated: one-file-per-SHN split for MapWayPoint.shn
#ifndef FIESTA_DATAREADER_SHN_MAPWAYPOINT_H
#define FIESTA_DATAREADER_SHN_MAPWAYPOINT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MapWayPointRow {
    uint16           uiMapID;
    uint32           uiX;
    uint32           uiY;
    uint8            uiMWP_Gate;
};

class MapWayPointShn {
public:
    static MapWayPointShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MapWayPointRow>& Rows() const { return m_kRows; }
private:
    std::vector<MapWayPointRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MAPWAYPOINT_H
