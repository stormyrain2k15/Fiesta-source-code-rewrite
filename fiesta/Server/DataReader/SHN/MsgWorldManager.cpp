// Server/DataReader/SHN/MsgWorldManager.cpp
// Auto-generated: one-file-per-SHN split for MsgWorldManager.shn
#include "MsgWorldManager.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MsgWorldManagerShn& MsgWorldManagerShn::Get() { static MsgWorldManagerShn s; return s; }

void MsgWorldManagerShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MsgWorldManager");
    if (!t) { SHINELOG_WARN("MsgWorldManager.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MsgWorldManagerRow rec;
        rec.kDesc = ShnGetStr(*t, _r, "Desc");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MsgWorldManager.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
