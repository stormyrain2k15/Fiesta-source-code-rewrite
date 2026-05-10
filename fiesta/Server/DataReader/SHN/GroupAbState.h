// Server/DataReader/SHN/GroupAbState.h
// Auto-generated: one-file-per-SHN split for GroupAbState.shn
#ifndef FIESTA_DATAREADER_SHN_GROUPABSTATE_H
#define FIESTA_DATAREADER_SHN_GROUPABSTATE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct GroupAbStateRow {
    std::string      kGroupAbStateIDX;
    std::string      kAbstateIDX;
    uint8            uiStrength;
};

class GroupAbStateShn {
public:
    static GroupAbStateShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GroupAbStateRow>& Rows() const { return m_kRows; }
private:
    std::vector<GroupAbStateRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_GROUPABSTATE_H
