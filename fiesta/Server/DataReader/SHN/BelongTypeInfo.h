// Server/DataReader/SHN/BelongTypeInfo.h
// Auto-generated: one-file-per-SHN split for BelongTypeInfo.shn
#ifndef FIESTA_DATAREADER_SHN_BELONGTYPEINFO_H
#define FIESTA_DATAREADER_SHN_BELONGTYPEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct BelongTypeInfoRow {
    uint32           uiBT_Inx;
    uint8            uiPutOnBelonged;
    uint8            uiNoDrop;
    uint8            uiNoSell;
    uint8            uiNoStorage;
    uint8            uiNoTrade;
    uint8            uiNoDelete;
};

class BelongTypeInfoShn {
public:
    static BelongTypeInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<BelongTypeInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<BelongTypeInfoRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_BELONGTYPEINFO_H
