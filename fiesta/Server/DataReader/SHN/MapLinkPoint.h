// Server/DataReader/SHN/MapLinkPoint.h
// Auto-generated: one-file-per-SHN split for MapLinkPoint.shn
#ifndef FIESTA_DATAREADER_SHN_MAPLINKPOINT_H
#define FIESTA_DATAREADER_SHN_MAPLINKPOINT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MapLinkPointRow {
    uint32           uiMLP_FromID;
    uint32           uiMLP_ToID;
    uint32           uiMLP_Weight;
    uint16           uiMLP_OneWay_Street;
};

class MapLinkPointShn {
public:
    static MapLinkPointShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MapLinkPointRow>& Rows() const { return m_kRows; }
private:
    std::vector<MapLinkPointRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MAPLINKPOINT_H
