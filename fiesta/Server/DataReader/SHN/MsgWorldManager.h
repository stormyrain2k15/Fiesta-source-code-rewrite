// Server/DataReader/SHN/MsgWorldManager.h
// Auto-generated: one-file-per-SHN split for MsgWorldManager.shn
#ifndef FIESTA_DATAREADER_SHN_MSGWORLDMANAGER_H
#define FIESTA_DATAREADER_SHN_MSGWORLDMANAGER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct MsgWorldManagerRow {
    std::string      kDesc;
};

class MsgWorldManagerShn {
public:
    static MsgWorldManagerShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MsgWorldManagerRow>& Rows() const { return m_kRows; }
private:
    std::vector<MsgWorldManagerRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_MSGWORLDMANAGER_H
