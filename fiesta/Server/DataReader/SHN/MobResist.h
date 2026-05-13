// Server/DataReader/SHN/MobResist.h
// Auto-generated: one-file-per-SHN split for MobResist.shn
#ifndef SHINE_DATAREADER_SHN_MOBRESIST_H
#define SHINE_DATAREADER_SHN_MOBRESIST_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MobResistRow {
    std::string      kInxName;
    uint16           uiResDot;
    uint16           uiResStun;
    uint16           uiResMoveSpeed;
    uint16           uiResFear;
    uint16           uiResBinding;
    uint16           uiResReverse;
    uint16           uiResMesmerize;
    uint16           uiResSeverBone;
    uint16           uiResKnockBack;
    uint16           uiResTBMinus;
};

class MobResistShn {
public:
    static MobResistShn& Get();
    void Load();
    const MobResistRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MobResistRow>& Rows() const { return m_kRows; }
private:
    std::vector<MobResistRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MOBRESIST_H
