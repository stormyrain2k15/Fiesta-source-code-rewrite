// Server/DataReader/SHN/Produce.h
// Auto-generated: one-file-per-SHN split for Produce.shn
#ifndef SHINE_DATAREADER_SHN_PRODUCE_H
#define SHINE_DATAREADER_SHN_PRODUCE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ProduceRow {
    uint16           uiProductID;
    std::string      kProduceIndex;
    std::string      kName;
    std::string      kProduct;
    uint32           uiLot;
    std::string      kRaw0;
    uint32           uiQuantity0;
    std::string      kRaw1;
    uint32           uiQuantity1;
    std::string      kRaw2;
    uint32           uiQuantity2;
    std::string      kRaw3;
    uint32           uiQuantity3;
    std::string      kRaw4;
    uint32           uiQuantity4;
    std::string      kRaw5;
    uint32           uiQuantity5;
    std::string      kRaw6;
    uint32           uiQuantity6;
    std::string      kRaw7;
    uint32           uiQuantity7;
    uint32           uiMasteryType;
    uint32           uiMasteryGain;
    uint32           uiNeededMasteryType;
    uint32           uiNeededMasteryGain;
};

class ProduceShn {
public:
    static ProduceShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ProduceRow>& Rows() const { return m_kRows; }
private:
    std::vector<ProduceRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_PRODUCE_H
