// Server/DataReader/SHN/ActionViewInfo.h
// Auto-generated: one-file-per-SHN split for ActionViewInfo.shn
#ifndef FIESTA_DATAREADER_SHN_ACTIONVIEWINFO_H
#define FIESTA_DATAREADER_SHN_ACTIONVIEWINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ActionViewInfoRow {
    uint8            uiNIndex;
    std::string      kInxName;
    std::string      kActionName;
    uint16           uiLinkActionIndex;
    std::string      kIconFileName;
    uint16           uiNIconNum;
    uint32           uiEActionType;
    uint32           uiNEventCode;
    uint32           uiNAfterCode;
    uint8            uiIsDance;
};

class ActionViewInfoShn {
public:
    static ActionViewInfoShn& Get();
    void Load();
    const ActionViewInfoRow* FindByInx(const std::string& rInx) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ActionViewInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<ActionViewInfoRow>         m_kRows;
    std::map<std::string, size_t>  m_kByInx;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ACTIONVIEWINFO_H
