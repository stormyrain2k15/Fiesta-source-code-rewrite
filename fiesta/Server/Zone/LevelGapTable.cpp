// Server/Zone/LevelGapTable.cpp
#include "LevelGapTable.h"
#include "../DataReader/ShnFile.h"
#include "../Shared/ShineLogSystem.h"

namespace fiesta {

LevelGapTable& LevelGapTable::Get() { static LevelGapTable s; return s; }

bool LevelGapTable::LoadOne(const std::string& rPath, int32* pTable) {
    ShnFile f;
    if (!f.LoadFromFile(rPath)) {
        SHINELOG_WARN("LevelGapTable: missing %s -- using neutral 1.0x", rPath.c_str());
        return false;
    }
    std::vector<std::vector<std::string> > rows;
    f.ExportAsStringRows(rows);
    if (rows.size() < 2) return false;
    // Header column lookup.
    int idxGap = -1, idxMul = -1;
    for (size_t c = 0; c < rows[0].size(); ++c) {
        const std::string& n = rows[0][c];
        if (idxGap < 0 && (n == "LevelDiff" || n == "Gap" || n == "LvGap")) idxGap = (int)c;
        if (idxMul < 0 && (n == "Multiplier" || n == "Damage" || n == "Rate")) idxMul = (int)c;
    }
    if (idxGap < 0) idxGap = 0;
    if (idxMul < 0) idxMul = 1;
    for (size_t r = 1; r < rows.size(); ++r) {
        if ((int)rows[r].size() <= idxMul) continue;
        int32 gap  = atoi(rows[r][idxGap].c_str());
        int32 mult = atoi(rows[r][idxMul].c_str());
        int   slot = (int)gap + kLvGapOffset;
        if (slot >= 0 && slot < kLvGapTableSize) pTable[slot] = mult;
    }
    return true;
}

bool LevelGapTable::Load(const std::string& rRoot) {
    bool ok = true;
    ok &= LoadOne(rRoot + "\\DamageLvGapPVE.shn", m_aPvE);
    ok &= LoadOne(rRoot + "\\DamageLvGapPVP.shn", m_aPvP);
    ok &= LoadOne(rRoot + "\\DamageLvGapEVP.shn", m_aEvP);
    m_bLoaded = ok;
    return ok;
}

int32 LevelGapTable::GetMultiplier(int32 nAtkLevel, int32 nDefLevel, eLvGapType eType) const {
    int32 gap = nAtkLevel - nDefLevel;
    if (gap < -kMaxLevelGap) gap = -kMaxLevelGap;
    if (gap >  kMaxLevelGap) gap =  kMaxLevelGap;
    int slot = (int)gap + kLvGapOffset;
    if (slot < 0) slot = 0; if (slot >= kLvGapTableSize) slot = kLvGapTableSize - 1;
    switch (eType) {
        case LVGAP_PVE: return m_aPvE[slot];
        case LVGAP_PVP: return m_aPvP[slot];
        case LVGAP_EVP: return m_aEvP[slot];
        default:        return 1000;
    }
}

} // namespace fiesta
