// Server/DataReader/SHN/UpgradeInfo.h
// Auto-generated: one-file-per-SHN split for UpgradeInfo.shn
#ifndef SHINE_DATAREADER_SHN_UPGRADEINFO_H
#define SHINE_DATAREADER_SHN_UPGRADEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct UpgradeInfoRow {
    uint16           uiID;
    std::string      kInxName;
    uint32           uiUpFactor;
    int16            iUpdata;
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
};

class UpgradeInfoShn {
public:
    static UpgradeInfoShn& Get();
    void Load();
    const UpgradeInfoRow* FindById(uint32 uiID) const;
    const UpgradeInfoRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<UpgradeInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<UpgradeInfoRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_UPGRADEINFO_H
