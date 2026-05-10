// Server/DataReader/SHN/GBEventCode.h
// Auto-generated: one-file-per-SHN split for GBEventCode.shn
#ifndef FIESTA_DATAREADER_SHN_GBEVENTCODE_H
#define FIESTA_DATAREADER_SHN_GBEVENTCODE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GBEventCodeRow {
    uint32           uiGameType;
    uint32           uiGB_ANI;
    uint32           uiGB_ECode;
};

class GBEventCodeShn {
public:
    static GBEventCodeShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBEventCodeRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBEventCodeRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GBEVENTCODE_H
