// Server/DataReader/SHN/MapInfo.h
// Auto-generated: one-file-per-SHN split for MapInfo.shn
#ifndef FIESTA_DATAREADER_SHN_MAPINFO_H
#define FIESTA_DATAREADER_SHN_MAPINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MapInfoRow {
    uint16           uiID;
    std::string      kMapName;
    std::string      kName;
    uint32           uiIsWMLink;
    uint32           uiRegenX;
    uint32           uiRegenY;
    uint8            uiKingdomMap;
    std::string      kMapFolderName;
    uint8            uiInSide;
    uint32           uiSight;
};

class MapInfoShn {
public:
    static MapInfoShn& Get();
    void Load();
    const MapInfoRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MapInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<MapInfoRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MAPINFO_H
