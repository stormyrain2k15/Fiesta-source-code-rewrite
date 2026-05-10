// Server/DataReader/SHN/ChatColor.cpp
// Auto-generated: one-file-per-SHN split for ChatColor.shn
#include "ChatColor.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

ChatColorShn& ChatColorShn::Get() { static ChatColorShn s; return s; }

void ChatColorShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("ChatColor");
    if (!t) { SHINELOG_WARN("ChatColor.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ChatColorRow rec;
        rec.uiChatColorID = (uint8)ShnGetU32(*t, _r, "ChatColorID");
        rec.uiColorR = (uint8)ShnGetU32(*t, _r, "ColorR");
        rec.uiColorG = (uint8)ShnGetU32(*t, _r, "ColorG");
        rec.uiColorB = (uint8)ShnGetU32(*t, _r, "ColorB");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("ChatColor.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
