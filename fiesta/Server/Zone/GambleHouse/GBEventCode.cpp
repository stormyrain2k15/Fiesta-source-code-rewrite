// Server/Zone/GambleHouse/GBEventCode.cpp
// FEATURE: casino-eventcode
#include "GBAuxTables.h"
#include "../../DataReader/ShnRegistry.h"
#include "../../Shared/ShineLogSystem.h"
namespace shine {
GBEventCodeTable& GBEventCodeTable::Get() { static GBEventCodeTable s; return s; }
bool GBEventCodeTable::Bind() {
    m_kRows.clear();
    // FEATURE: casino-eventcode -- column read: GameType, GB_ANI, GB_ECode
    const ShnFile* t = ShnRegistry::Get().GetTable("GBEventCode");
    if (!t) { SHINELOG_WARN("GBEventCode.shn missing"); return false; }
    for (size_t i = 0; i < t->Rows().size(); ++i) {
        uint32 gt  = ShnGetU32(*t, i, "GameType");
        uint32 ani = ShnGetU32(*t, i, "GB_ANI");
        int32  ec  = (int32)ShnGetI32(*t, i, "GB_ECode");
        uint64 key = ((uint64)gt << 32) | ani;
        m_kRows[key] = ec;
    }
    SHINELOG_INFO("GBEventCode: %u (game,ani) codes", (uint32)m_kRows.size());
    return !m_kRows.empty();
}
int32 GBEventCodeTable::Code(uint32 uiGT, uint32 uiAni) const {
    uint64 key = ((uint64)uiGT << 32) | uiAni;
    std::map<uint64, int32>::const_iterator it = m_kRows.find(key);
    return (it == m_kRows.end()) ? 0 : it->second;
}
} // namespace shine
