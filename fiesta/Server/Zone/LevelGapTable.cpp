// Server/Zone/LevelGapTable.cpp
// FEATURE: lvgap-damage-curve
#include "LevelGapTable.h"
#include "../DataReader/ShnFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace shine {

LevelGapTable& LevelGapTable::Get() { static LevelGapTable s; return s; }

LevelGapTable::LevelGapTable() : m_bLoaded(false) {
    int i, j;
    for (i = 0; i < kLvGapTableSize; ++i) {
        m_aPvE[i] = 1000;
        m_aEvP[i] = 1000;
    }
    for (i = 0; i < kPvPMatrixSize; ++i) {
        for (j = 0; j < kPvPMatrixSize; ++j) {
            m_aPvP[i][j] = 1000;
        }
    }
}

// 1-D curve: row { LvGap, DamageRate }. Used by DamageLvGapPVE and
// DamageLvGapEVP. The column names are stable across the NA2016 drop.
bool LevelGapTable::LoadCurve1D(const std::string& rPath, int32* pTable) {
    ShnFile f;
    if (!f.LoadFromFile(rPath)) {
        SHINELOG_WARN("LevelGapTable: missing %s -- using neutral 1.0x",
                      rPath.c_str());
        return false;
    }
    std::vector<std::vector<std::string> > rows;
    f.ExportAsStringRows(rows);
    if (rows.size() < 2) return false;

    // FEATURE: lvgap-damage-curve -- column read: LvGap, DamageRate
    int32 idxGap = -1, idxRate = -1;
    for (size_t c = 0; c < rows[0].size(); ++c) {
        const std::string& n = rows[0][c];
        if (idxGap  < 0 && _stricmp(n.c_str(), "LvGap")      == 0) idxGap  = (int32)c;
        if (idxRate < 0 && _stricmp(n.c_str(), "DamageRate") == 0) idxRate = (int32)c;
    }
    if (idxGap < 0 || idxRate < 0) {
        SHINELOG_WARN("LevelGapTable: %s missing LvGap/DamageRate columns "
                      "(got %u columns)", rPath.c_str(), (uint32)rows[0].size());
        return false;
    }

    int32 readCount = 0;
    for (size_t r = 1; r < rows.size(); ++r) {
        if ((int32)rows[r].size() <= idxRate) continue;
        const char* sGap  = rows[r][idxGap].c_str();
        const char* sRate = rows[r][idxRate].c_str();
        if (!sGap[0]) continue;
        int32 gap  = atoi(sGap);
        int32 rate = atoi(sRate);
        int32 slot = gap + kLvGapOffset;
        if (slot >= 0 && slot < kLvGapTableSize) {
            pTable[slot] = rate;
            ++readCount;
        }
    }
    SHINELOG_INFO("LevelGapTable: %s loaded %d gap rows", rPath.c_str(), readCount);
    return readCount > 0;
}

// 2-D matrix: row keyed on MyLv, columns TargetLv1..TargetLv150. Used
// only by DamageLvGapPVP. Each cell is the multiplier per-1000 to apply
// when an attacker of MyLv hits a defender of TargetLv<N>.
bool LevelGapTable::LoadMatrix2D(const std::string& rPath) {
    ShnFile f;
    if (!f.LoadFromFile(rPath)) {
        SHINELOG_WARN("LevelGapTable: missing %s -- using neutral 1.0x",
                      rPath.c_str());
        return false;
    }
    std::vector<std::vector<std::string> > rows;
    f.ExportAsStringRows(rows);
    if (rows.size() < 2) return false;

    // FEATURE: lvgap-damage-curve -- column read: MyLv
    int32 idxMyLv = -1;
    // For each TargetLv<N>, remember which CSV column index it lives in.
    // We size for kPvPMatrixSize-1 because TargetLv is 1-based (TargetLv1
    // .. TargetLv150 in the NA2016 file -> matrix column 0..149).
    int32 idxTarget[kPvPMatrixSize];
    int32 i;
    for (i = 0; i < kPvPMatrixSize; ++i) idxTarget[i] = -1;

    for (size_t c = 0; c < rows[0].size(); ++c) {
        const std::string& n = rows[0][c];
        if (idxMyLv < 0 && _stricmp(n.c_str(), "MyLv") == 0) {
            idxMyLv = (int32)c;
            continue;
        }
        // FEATURE: lvgap-damage-curve -- column read: TargetLv<N> (1..150)
        if (n.size() > 8 && _strnicmp(n.c_str(), "TargetLv", 8) == 0) {
            int32 lv = atoi(n.c_str() + 8);
            if (lv >= 1 && lv < kPvPMatrixSize) {
                idxTarget[lv] = (int32)c;
            }
        }
    }
    if (idxMyLv < 0) {
        SHINELOG_WARN("LevelGapTable: %s missing MyLv column", rPath.c_str());
        return false;
    }

    int32 cellsRead = 0;
    int32 rowsRead  = 0;
    for (size_t r = 1; r < rows.size(); ++r) {
        const std::vector<std::string>& row = rows[r];
        if ((int32)row.size() <= idxMyLv) continue;
        const char* sMy = row[idxMyLv].c_str();
        if (!sMy[0]) continue;
        int32 myLv = atoi(sMy);
        if (myLv < 0 || myLv >= kPvPMatrixSize) continue;
        for (int32 tgt = 1; tgt < kPvPMatrixSize; ++tgt) {
            int32 c = idxTarget[tgt];
            if (c < 0) continue;
            if ((int32)row.size() <= c) continue;
            const std::string& cell = row[c];
            if (cell.empty()) continue;
            int32 mult = atoi(cell.c_str());
            // Matrix col-index is (tgt - 1) so TargetLv1 lands at [my][0].
            // Storing tgt directly (with tgt-1 -> col tgt-1) keeps the
            // GetMultiplier lookup symmetric on (myLv, defLv) values.
            // Convention: m_aPvP[my][def] where def = TargetLv<def>.
            // -> store at [my][tgt - 1].
            m_aPvP[myLv][tgt - 1] = mult;
            ++cellsRead;
        }
        ++rowsRead;
    }
    SHINELOG_INFO("LevelGapTable: %s loaded %d rows, %d cells",
                  rPath.c_str(), rowsRead, cellsRead);
    return cellsRead > 0;
}

bool LevelGapTable::Load(const std::string& rRoot) {
    bool any = false;
    any |= LoadCurve1D (rRoot + "\\DamageLvGapPVE.shn", m_aPvE);
    any |= LoadCurve1D (rRoot + "\\DamageLvGapEVP.shn", m_aEvP);
    any |= LoadMatrix2D(rRoot + "\\DamageLvGapPVP.shn");
    m_bLoaded = any;
    return any;
}

int32 LevelGapTable::GetMultiplier(int32 nAtkLevel, int32 nDefLevel,
                                   eLvGapType eType) const
{
    if (eType == LVGAP_PVP) {
        // 2-D matrix. Clamp both axes into [0, kPvPMatrixSize-1].
        int32 my  = nAtkLevel;
        int32 tgt = nDefLevel - 1;             // TargetLv1 -> col 0
        if (my  < 0)                  my  = 0;
        if (my  >= kPvPMatrixSize)    my  = kPvPMatrixSize - 1;
        if (tgt < 0)                  tgt = 0;
        if (tgt >= kPvPMatrixSize)    tgt = kPvPMatrixSize - 1;
        return m_aPvP[my][tgt];
    }
    // 1-D PvE / EvP curve.
    int32 gap = nAtkLevel - nDefLevel;
    if (gap < -kMaxLevelGap) gap = -kMaxLevelGap;
    if (gap >  kMaxLevelGap) gap =  kMaxLevelGap;
    int32 slot = gap + kLvGapOffset;
    if (slot < 0)                slot = 0;
    if (slot >= kLvGapTableSize) slot = kLvGapTableSize - 1;
    if (eType == LVGAP_EVP) return m_aEvP[slot];
    return m_aPvE[slot];
}

} // namespace shine
