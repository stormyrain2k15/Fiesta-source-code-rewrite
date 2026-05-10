// Server/DataReader/SHN/CollectCardMobGroup.cpp
// Auto-generated: one-file-per-SHN split for CollectCardMobGroup.shn
#include "CollectCardMobGroup.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CollectCardMobGroupShn& CollectCardMobGroupShn::Get() { static CollectCardMobGroupShn s; return s; }

void CollectCardMobGroupShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CollectCardMobGroup");
    if (!t) { SHINELOG_WARN("CollectCardMobGroup.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CollectCardMobGroupRow rec;
        rec.kCC_MobInx = ShnGetStr(*t, _r, "CC_MobInx");
        rec.uiCC_CardMobGroup = ShnGetU32(*t, _r, "CC_CardMobGroup");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CollectCardMobGroup.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
