// Server/DataReader/SHN/MobKillAnnounce.cpp
// Auto-generated: one-file-per-SHN split for MobKillAnnounce.shn
#include "MobKillAnnounce.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

MobKillAnnounceShn& MobKillAnnounceShn::Get() { static MobKillAnnounceShn s; return s; }

void MobKillAnnounceShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("MobKillAnnounce");
    if (!t) { SHINELOG_WARN("MobKillAnnounce.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        MobKillAnnounceRow rec;
        rec.uiMobID = ShnGetU32(*t, _r, "MobID");
        rec.uiTextIndex = ShnGetU32(*t, _r, "TextIndex");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("MobKillAnnounce.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
