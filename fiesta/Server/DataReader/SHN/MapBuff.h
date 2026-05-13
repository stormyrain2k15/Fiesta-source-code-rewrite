// Server/DataReader/SHN/MapBuff.h
// Auto-generated: one-file-per-SHN split for MapBuff.shn
#ifndef SHINE_DATAREADER_SHN_MAPBUFF_H
#define SHINE_DATAREADER_SHN_MAPBUFF_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MapBuffRow {
    std::string      kMapName;
    uint32           uiModeIDLv;
    std::string      kAbStateIDX;
};

class MapBuffShn {
public:
    static MapBuffShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MapBuffRow>& Rows() const { return m_kRows; }
private:
    std::vector<MapBuffRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MAPBUFF_H
