// Server/DataReader/SHN/Riding.h
// Auto-generated: one-file-per-SHN split for Riding.shn
#ifndef SHINE_DATAREADER_SHN_RIDING_H
#define SHINE_DATAREADER_SHN_RIDING_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct RidingRow {
    uint16           uiHandle;
    std::string      kItemID;
    std::string      kName;
    std::string      kBodyType;
    std::string      kShape;
    uint16           uiUseTime;
    std::string      kFeedType;
    std::string      kTexture;
    uint16           uiFeedGauge;
    uint16           uiHGauge;
    uint16           uiInitHgauge;
    uint16           uiTick;
    uint16           uiUGauge;
    uint16           uiRunSpeed;
    uint16           uiFootSpeed;
    uint16           uiCastingTime;
    uint32           uiCoolTime;
    std::string      kIconFileN;
    uint16           uiIconIndex;
    std::string      kImageN;
    std::string      kImageH;
    std::string      kImageE;
    std::string      kDummyA;
    std::string      kDummyB;
};

class RidingShn {
public:
    static RidingShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<RidingRow>& Rows() const { return m_kRows; }
private:
    std::vector<RidingRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_RIDING_H
