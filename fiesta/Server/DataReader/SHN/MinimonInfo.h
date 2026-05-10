// Server/DataReader/SHN/MinimonInfo.h
// Auto-generated: one-file-per-SHN split for MinimonInfo.shn
#ifndef FIESTA_DATAREADER_SHN_MINIMONINFO_H
#define FIESTA_DATAREADER_SHN_MINIMONINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MinimonInfoRow {
    std::string      kItemIDX;
    uint32           uiMinimonEquipPos;
    uint32           uiMinimonRole;
};

class MinimonInfoShn {
public:
    static MinimonInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MinimonInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<MinimonInfoRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MINIMONINFO_H
