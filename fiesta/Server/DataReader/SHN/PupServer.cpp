// Server/DataReader/SHN/PupServer.cpp
// Auto-generated: one-file-per-SHN split for PupServer.shn
#include "PupServer.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

PupServerShn& PupServerShn::Get() { static PupServerShn s; return s; }

void PupServerShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("PupServer");
    if (!t) { SHINELOG_WARN("PupServer.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        PupServerRow rec;
        rec.kPupIDX = ShnGetStr(*t, _r, "PupIDX");
        rec.uiMinMind = (uint8)ShnGetU32(*t, _r, "MinMind");
        rec.uiMaxMind = (uint8)ShnGetU32(*t, _r, "MaxMind");
        rec.uiMinStress = (uint8)ShnGetU32(*t, _r, "MinStress");
        rec.uiMaxStress = (uint8)ShnGetU32(*t, _r, "MaxStress");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("PupServer.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
