// Server/DataReader/SHN/PupMain.h
// Auto-generated: one-file-per-SHN split for PupMain.shn
#ifndef FIESTA_DATAREADER_SHN_PUPMAIN_H
#define FIESTA_DATAREADER_SHN_PUPMAIN_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct PupMainRow {
    uint32           uiPupID;
    std::string      kPupIDX;
    std::string      kItemIDX;
    uint16           uiPupSpeed;
};

class PupMainShn {
public:
    static PupMainShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<PupMainRow>& Rows() const { return m_kRows; }
private:
    std::vector<PupMainRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_PUPMAIN_H
