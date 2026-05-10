// Server/DataReader/SHN/CharacterTitleStateServer.cpp
// Auto-generated: one-file-per-SHN split for CharacterTitleStateServer.shn
#include "CharacterTitleStateServer.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

CharacterTitleStateServerShn& CharacterTitleStateServerShn::Get() { static CharacterTitleStateServerShn s; return s; }

void CharacterTitleStateServerShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("CharacterTitleStateServer");
    if (!t) { SHINELOG_WARN("CharacterTitleStateServer.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        CharacterTitleStateServerRow rec;
        rec.uiType = ShnGetU32(*t, _r, "Type");
        rec.uiTitleLV = (uint8)ShnGetU32(*t, _r, "TitleLV");
        rec.kStateName = ShnGetStr(*t, _r, "StateName");
        rec.uiStrength = (uint8)ShnGetU32(*t, _r, "Strength");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("CharacterTitleStateServer.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
