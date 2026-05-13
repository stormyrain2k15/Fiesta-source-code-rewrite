// Server/Zone/MoverTables.cpp
#include "MoverTables.h"
#include "../DataReader/ShnFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>
#include <string.h>

namespace shine {

namespace {
int idxOf(const std::vector<std::string>& hdr, const char* a, const char* b = NULL, const char* c = NULL) {
    for (size_t i = 0; i < hdr.size(); ++i) {
        if (_stricmp(hdr[i].c_str(), a) == 0) return (int)i;
        if (b && _stricmp(hdr[i].c_str(), b) == 0) return (int)i;
        if (c && _stricmp(hdr[i].c_str(), c) == 0) return (int)i;
    }
    return -1;
}
int32 cellInt(const std::vector<std::string>& row, int col, int32 def) {
    if (col < 0 || (size_t)col >= row.size() || row[col].empty() || row[col] == "-") return def;
    return atoi(row[col].c_str());
}
} // anon

// ---------- MoverMain ----------
MoverMainTable& MoverMainTable::Get() { static MoverMainTable s; return s; }

bool MoverMainTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    std::string path = rRoot + "\\MoverMain.shn";
    ShnFile f;
    if (!f.LoadFromFile(path)) {
        SHINELOG_WARN("MoverMain.shn missing -- characters will use fallback growth");
        return false;
    }
    std::vector<std::vector<std::string> > rows;
    f.ExportAsStringRows(rows);
    if (rows.size() < 2) return false;
    const std::vector<std::string>& hdr = rows[0];
    int iClass = idxOf(hdr, "Class", "BasicClass", "ClassNo");
    int iLevel = idxOf(hdr, "Level", "Lv");
    int iHP    = idxOf(hdr, "MaxHP", "BaseHP", "HP");
    int iSP    = idxOf(hdr, "MaxSP", "BaseSP", "SP");
    int iATK   = idxOf(hdr, "ATK",  "BaseATK");
    int iDEF   = idxOf(hdr, "DEF",  "BaseDEF");
    int iMATK  = idxOf(hdr, "MATK", "BaseMATK");
    int iMDEF  = idxOf(hdr, "MDEF", "BaseMDEF");
    int iMove  = idxOf(hdr, "MoveSpeed", "WalkSpeed");
    int iCap   = idxOf(hdr, "MaxStat", "StatCap");

    for (size_t r = 1; r < rows.size(); ++r) {
        const std::vector<std::string>& row = rows[r];
        if (row.size() < hdr.size() / 2) continue;
        LegacyMoverMainRow x;
        x.bClass         = (uint8) cellInt(row, iClass, 0);
        x.uiLevel        = (uint16)cellInt(row, iLevel, 1);
        x.nBaseHP        = cellInt(row, iHP,  100);
        x.nBaseSP        = cellInt(row, iSP,   50);
        x.nBaseATK       = cellInt(row, iATK,   5);
        x.nBaseDEF       = cellInt(row, iDEF,   2);
        x.nBaseMATK      = cellInt(row, iMATK,  3);
        x.nBaseMDEF      = cellInt(row, iMDEF,  2);
        x.nBaseMoveSpeed = cellInt(row, iMove, 200);
        x.nMaxStatTotal  = cellInt(row, iCap, 9999);
        uint32 key = ((uint32)x.bClass << 16) | (uint32)x.uiLevel;
        m_kRows[key] = x;
    }
    SHINELOG_INFO("MoverMain: %u rows", (uint32)m_kRows.size());
    return true;
}

const LegacyMoverMainRow* MoverMainTable::Find(uint8 bClass, uint16 uiLevel) const {
    uint32 key = ((uint32)bClass << 16) | (uint32)uiLevel;
    std::map<uint32, LegacyMoverMainRow>::const_iterator it = m_kRows.find(key);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

void MoverMainTable::FillRaw(RAWCHARSTAT* pOut, uint8 bClass, uint16 uiLevel) const {
    if (!pOut) return;
    pOut->nLevel = uiLevel;
    pOut->bClass = bClass;
    const LegacyMoverMainRow* p = Find(bClass, uiLevel);
    if (!p) {
        // Sane fallback so combat continues if MoverMain is absent.
        pOut->nBaseHP = 100 + uiLevel * 50;
        pOut->nBaseSP =  50 + uiLevel * 25;
        pOut->nBaseATK = 5 + uiLevel * 2;
        pOut->nBaseDEF = 2 + uiLevel * 1;
        pOut->nBaseMATK = 3 + uiLevel * 2;
        pOut->nBaseMDEF = 2 + uiLevel * 1;
        pOut->nBaseMoveSpeed = 200;
        return;
    }
    pOut->nBaseHP        = p->nBaseHP;
    pOut->nBaseSP        = p->nBaseSP;
    pOut->nBaseATK       = p->nBaseATK;
    pOut->nBaseDEF       = p->nBaseDEF;
    pOut->nBaseMATK      = p->nBaseMATK;
    pOut->nBaseMDEF      = p->nBaseMDEF;
    pOut->nBaseMoveSpeed = p->nBaseMoveSpeed;
}

// ---------- MoverAbility ----------
MoverAbilityTable& MoverAbilityTable::Get() { static MoverAbilityTable s; return s; }

bool MoverAbilityTable::Load(const std::string& rRoot) {
    m_kRows.clear();
    std::string path = rRoot + "\\MoverAbility.shn";
    ShnFile f;
    if (!f.LoadFromFile(path)) {
        SHINELOG_WARN("MoverAbility.shn missing -- fallback per-stat values from BattleTunables.");
        return false;
    }
    std::vector<std::vector<std::string> > rows;
    f.ExportAsStringRows(rows);
    if (rows.size() < 2) return false;
    const std::vector<std::string>& hdr = rows[0];
    int iClass    = idxOf(hdr, "Class", "BasicClass", "ClassNo");
    int iHpEND    = idxOf(hdr, "HpPerEND", "HpEND");
    int iSpINT    = idxOf(hdr, "SpPerINT", "SpINT");
    int iAtkSTR   = idxOf(hdr, "AtkPerSTR", "AtkSTR");
    int iMatkINT  = idxOf(hdr, "MatkPerINT", "MatkINT");
    int iDefEND   = idxOf(hdr, "DefPerEND", "DefEND");
    int iHitDEX   = idxOf(hdr, "HitPerDEX", "HitDEX");
    int iCritDEX  = idxOf(hdr, "CritPerDEX", "CritDEX");
    int iDodgeDEX = idxOf(hdr, "DodgePerDEX", "DodgeDEX");
    int iHpFlat   = idxOf(hdr, "MaxHPFlat", "MaxHpFlat");
    int iSpFlat   = idxOf(hdr, "MaxSPFlat", "MaxSpFlat");

    for (size_t r = 1; r < rows.size(); ++r) {
        const std::vector<std::string>& row = rows[r];
        LegacyMoverAbilityRow x;
        x.bClass        = (uint8)cellInt(row, iClass, 0);
        x.nHpPerEND     = cellInt(row, iHpEND,   14);
        x.nSpPerINT     = cellInt(row, iSpINT,   12);
        x.nAtkPerSTR    = cellInt(row, iAtkSTR,   2);
        x.nMatkPerINT   = cellInt(row, iMatkINT,  2);
        x.nDefPerEND    = cellInt(row, iDefEND,   1);
        x.nHitPerDEX    = cellInt(row, iHitDEX,   5);
        x.nCritPerDEX   = cellInt(row, iCritDEX,  3);
        x.nDodgePerDEX  = cellInt(row, iDodgeDEX, 4);
        x.nMaxHpFlat    = cellInt(row, iHpFlat,   0);
        x.nMaxSpFlat    = cellInt(row, iSpFlat,   0);
        m_kRows[x.bClass] = x;
    }
    SHINELOG_INFO("MoverAbility: %u classes", (uint32)m_kRows.size());
    return true;
}

const LegacyMoverAbilityRow* MoverAbilityTable::Find(uint8 bClass) const {
    std::map<uint8, LegacyMoverAbilityRow>::const_iterator it = m_kRows.find(bClass);
    return (it == m_kRows.end()) ? NULL : &it->second;
}

} // namespace shine
