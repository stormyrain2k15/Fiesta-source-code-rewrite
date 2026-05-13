// Server/DataReader/SHN/CollectCardMobGroup.h
// Auto-generated: one-file-per-SHN split for CollectCardMobGroup.shn
#ifndef SHINE_DATAREADER_SHN_COLLECTCARDMOBGROUP_H
#define SHINE_DATAREADER_SHN_COLLECTCARDMOBGROUP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct CollectCardMobGroupRow {
    std::string      kCC_MobInx;
    uint32           uiCC_CardMobGroup;
};

class CollectCardMobGroupShn {
public:
    static CollectCardMobGroupShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<CollectCardMobGroupRow>& Rows() const { return m_kRows; }
private:
    std::vector<CollectCardMobGroupRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_COLLECTCARDMOBGROUP_H
