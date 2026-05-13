// Server/DataReader/SHN/GBJoinGameMember.h
// Auto-generated: one-file-per-SHN split for GBJoinGameMember.shn
#ifndef SHINE_DATAREADER_SHN_GBJOINGAMEMEMBER_H
#define SHINE_DATAREADER_SHN_GBJOINGAMEMEMBER_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct GBJoinGameMemberRow {
    uint32           uiGameType;
    uint8            uiMinJoinMember;
    uint8            uiMaxJoinMember;
};

class GBJoinGameMemberShn {
public:
    static GBJoinGameMemberShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<GBJoinGameMemberRow>& Rows() const { return m_kRows; }
private:
    std::vector<GBJoinGameMemberRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_GBJOINGAMEMEMBER_H
