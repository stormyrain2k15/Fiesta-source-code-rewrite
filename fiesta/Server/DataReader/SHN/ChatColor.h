// Server/DataReader/SHN/ChatColor.h
// Auto-generated: one-file-per-SHN split for ChatColor.shn
#ifndef SHINE_DATAREADER_SHN_CHATCOLOR_H
#define SHINE_DATAREADER_SHN_CHATCOLOR_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ChatColorRow {
    uint8            uiChatColorID;
    uint8            uiColorR;
    uint8            uiColorG;
    uint8            uiColorB;
};

class ChatColorShn {
public:
    static ChatColorShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ChatColorRow>& Rows() const { return m_kRows; }
private:
    std::vector<ChatColorRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_CHATCOLOR_H
