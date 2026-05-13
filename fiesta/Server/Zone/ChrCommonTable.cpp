// Server/Zone/ChrCommonTable.cpp
#include "ChrCommonTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdlib.h>
#include <string.h>

namespace shine {

ChrCommonTable& ChrCommonTable::Get() { static ChrCommonTable s; return s; }

bool ChrCommonTable::LoadFreeStat(TableScriptFile& f, const char* szTable, eStat eS) {
    const TsTable* t = f.Find(szTable);
    if (!t) return false;
    m_kFree[eS].clear();
    m_kFreeColNames[eS].clear();
    for (size_t c = 0; c < t->kColumns.size(); ++c)
        m_kFreeColNames[eS].push_back(t->kColumns[c].kName);
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        FreeStatRow row;
        // Column 0 is "Points" / "stat" -- some tables use different names.
        std::string s0;
        if (!t->GetCell(r, (size_t)0, s0)) continue;
        row.uiPoints = (uint16)atoi(s0.c_str());
        for (size_t c = 0; c < t->kRecords[r].kCells.size(); ++c) {
            const std::string& v = t->kRecords[r].kCells[c];
            row.kCols.push_back(v.empty() || v == "-" ? 0 : atoi(v.c_str()));
        }
        m_kFree[eS][row.uiPoints] = row;
    }
    return true;
}

bool ChrCommonTable::Load(const std::string& rRoot) {
    m_kCommon.clear(); m_kStat.clear();
    for (int i = 0; i < 5; ++i) { m_kFree[i].clear(); m_kFreeColNames[i].clear(); }

    TableScriptFile f;
    std::string path = rRoot + "\\World\\ChrCommon.txt";
    if (!f.Load(path)) return false;

    if (const TsTable* t = f.Find("Common")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            std::string k = t->GetStr(r, "key");
            int32 v = (int32)t->GetInt(r, "argument");
            if (!k.empty()) m_kCommon[k] = v;
        }
    }
    if (const TsTable* t = f.Find("StatTable")) {
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            StatRow row; memset(&row, 0, sizeof(row));
            row.uiLevel    = (uint16)t->GetInt(r, "level");
            row.uiNextExp  = (uint32)t->GetInt(r, "NextExp");
            // The middle 30+ columns are per-class -- we capture by ordinal.
            for (size_t c = 0; c < t->kRecords[r].kCells.size() && c < 32; ++c) {
                const std::string& v = t->kRecords[r].kCells[c];
                row.aClassValue[c] = v.empty() || v == "-" ? 0 : (uint32)atoi(v.c_str());
            }
            row.uiExpLostAtPvP = (uint32)t->GetInt(r, "ExpLostAtPvP");
            if (row.uiLevel > 0) m_kStat[row.uiLevel] = row;
        }
    }
    LoadFreeStat(f, "FreeStatStrTable", STAT_STR);
    LoadFreeStat(f, "FreeStatIntTable", STAT_INT);
    LoadFreeStat(f, "FreeStatDexTable", STAT_DEX);
    LoadFreeStat(f, "FreeStatConTable", STAT_CON);
    LoadFreeStat(f, "FreeStatMenTable", STAT_MEN);

    SHINELOG_INFO("ChrCommon: %u common, %u levels, free-stat sizes %u/%u/%u/%u/%u",
                  (uint32)m_kCommon.size(), (uint32)m_kStat.size(),
                  (uint32)m_kFree[0].size(), (uint32)m_kFree[1].size(),
                  (uint32)m_kFree[2].size(), (uint32)m_kFree[3].size(),
                  (uint32)m_kFree[4].size());
    return true;
}

int32 ChrCommonTable::GetCommon(const char* szKey, int32 iDefault) const {
    std::map<std::string, int32>::const_iterator it = m_kCommon.find(szKey);
    return (it == m_kCommon.end()) ? iDefault : it->second;
}

const ChrCommonTable::StatRow* ChrCommonTable::StatRowByLevel(uint16 uiLevel) const {
    std::map<uint16, StatRow>::const_iterator it = m_kStat.find(uiLevel);
    return (it == m_kStat.end()) ? NULL : &it->second;
}

const ChrCommonTable::FreeStatRow* ChrCommonTable::FreeStatRowByPoints(eStat eS, uint16 p) const {
    if (eS < 0 || eS >= 5) return NULL;
    std::map<uint16, FreeStatRow>::const_iterator it = m_kFree[eS].find(p);
    return (it == m_kFree[eS].end()) ? NULL : &it->second;
}

// Resolve named column. Returns 0 if absent.
static int32 NamedCol(const std::vector<std::string>& names,
                      const ChrCommonTable::FreeStatRow* row, const char* szCol) {
    if (!row) return 0;
    for (size_t i = 0; i < names.size() && i < row->kCols.size(); ++i)
        if (_stricmp(names[i].c_str(), szCol) == 0) return row->kCols[i];
    return 0;
}

int32 ChrCommonTable::WCPerStr(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_STR], FreeStatRowByPoints(STAT_STR, p), "WC");
}
int32 ChrCommonTable::MAPerInt(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_INT], FreeStatRowByPoints(STAT_INT, p), "MA");
}
int32 ChrCommonTable::SPPerInt(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_INT], FreeStatRowByPoints(STAT_INT, p), "MaxSP");
}
int32 ChrCommonTable::HitPerDex(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_DEX], FreeStatRowByPoints(STAT_DEX, p), "ToHit");
}
int32 ChrCommonTable::BlockPerDex(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_DEX], FreeStatRowByPoints(STAT_DEX, p), "ToBlock");
}
int32 ChrCommonTable::ACPerCon(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_CON], FreeStatRowByPoints(STAT_CON, p), "AC");
}
int32 ChrCommonTable::HPPerCon(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_CON], FreeStatRowByPoints(STAT_CON, p), "MaxHP");
}
int32 ChrCommonTable::MRPerMen(uint16 p) const {
    return NamedCol(m_kFreeColNames[STAT_MEN], FreeStatRowByPoints(STAT_MEN, p), "MR");
}

} // namespace shine
