// Server/DataReader/SHN/BRAccUpgradeInfo.h
// Auto-generated: one-file-per-SHN split for BRAccUpgradeInfo.shn
#ifndef SHINE_DATAREADER_SHN_BRACCUPGRADEINFO_H
#define SHINE_DATAREADER_SHN_BRACCUPGRADEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct BRAccUpgradeInfoRow {
    uint16           uiID;
    std::string      kInxName;
    uint32           uiUpFactor;
    int16            iBRAccUpdata;
    int16            iUnkCol4;
    int16            iUnkCol5;
    int16            iUnkCol6;
    int16            iUnkCol7;
    int16            iUnkCol8;
    int16            iUnkCol9;
    int16            iUnkCol10;
    int16            iUnkCol11;
    int16            iUnkCol12;
    int16            iUnkCol13;
    int16            iUnkCol14;
    int16            iUnkCol15;
    int16            iUnkCol16;
    int16            iUnkCol17;
    int16            iUnkCol18;
    int16            iUnkCol19;
    int16            iUnkCol20;
    int16            iUnkCol21;
    int16            iUnkCol22;
    int16            iUnkCol23;
};

class BRAccUpgradeInfoShn {
public:
    static BRAccUpgradeInfoShn& Get();
    void Load();
    const BRAccUpgradeInfoRow* FindById(uint32 uiID) const;
    const BRAccUpgradeInfoRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<BRAccUpgradeInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<BRAccUpgradeInfoRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_BRACCUPGRADEINFO_H
