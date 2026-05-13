// Server/DataReader/SHN/StateItem.h
// Auto-generated: one-file-per-SHN split for StateItem.shn
#ifndef SHINE_DATAREADER_SHN_STATEITEM_H
#define SHINE_DATAREADER_SHN_STATEITEM_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct StateItemRow {
    std::string      kItemIDX;
    std::string      kAbStateInx;
    uint32           uiStaStrength;
    uint16           uiStaSucRate;
};

class StateItemShn {
public:
    static StateItemShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<StateItemRow>& Rows() const { return m_kRows; }
private:
    std::vector<StateItemRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_STATEITEM_H
