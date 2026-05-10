// Server/DataReader/SHN/AnnounceData.h
// Auto-generated: one-file-per-SHN split for AnnounceData.shn
#ifndef FIESTA_DATAREADER_SHN_ANNOUNCEDATA_H
#define FIESTA_DATAREADER_SHN_ANNOUNCEDATA_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct AnnounceDataRow {
    uint32           uiType;
    std::string      kMessage;
    uint32           uiValue;
};

class AnnounceDataShn {
public:
    static AnnounceDataShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<AnnounceDataRow>& Rows() const { return m_kRows; }
private:
    std::vector<AnnounceDataRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ANNOUNCEDATA_H
