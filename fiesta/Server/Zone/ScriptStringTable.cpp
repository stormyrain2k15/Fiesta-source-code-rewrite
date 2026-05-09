// Server/Zone/ScriptStringTable.cpp
#include "ScriptStringTable.h"
#include "../DataReader/TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>

namespace fiesta {

bool ScriptStringTable::Load(const std::string& rPath) {
    m_kEntries.clear();
    TableScriptFile f;
    if (!f.Load(rPath)) return false;
    // Most files declare "Script". A few use a per-event table name -- accept
    // any single-table file and treat its first table as the script table.
    const TsTable* t = f.Find("Script");
    if (!t && f.Count() > 0) t = &f.At(0);
    if (!t) return false;
    int idxKey = t->ColIndex("ScrIndex");
    int idxVal = t->ColIndex("ScrString");
    if (idxKey < 0) idxKey = 0;
    if (idxVal < 0) idxVal = (idxKey == 0) ? 1 : 0;
    for (size_t r = 0; r < t->kRecords.size(); ++r) {
        if ((size_t)idxKey >= t->kRecords[r].kCells.size()) continue;
        if ((size_t)idxVal >= t->kRecords[r].kCells.size()) continue;
        const std::string& k = t->kRecords[r].kCells[idxKey];
        const std::string& v = t->kRecords[r].kCells[idxVal];
        if (!k.empty()) m_kEntries[k] = v;
    }
    return true;
}

bool ScriptStringTable::Lookup(const std::string& rIndex, std::string& rOut) const {
    std::map<std::string, std::string>::const_iterator it = m_kEntries.find(rIndex);
    if (it == m_kEntries.end()) return false;
    rOut = it->second; return true;
}

ScriptStringBox& ScriptStringBox::Get() { static ScriptStringBox s; return s; }

bool ScriptStringBox::LoadAll(const std::string& rRoot) {
    Clear();
    std::string dir = rRoot + "\\Script";
    std::string pat = dir + "\\*.txt";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return false;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        std::string p = dir + "\\" + fd.cFileName;
        ScriptStringTable* tab = new ScriptStringTable();
        if (!tab->Load(p)) { delete tab; continue; }
        m_kTables.push_back(tab);
        // Flatten -- later overrides earlier.
        // (We can't iterate the private map; reload via Lookup keys.)
        // For efficiency we re-parse just to merge into the flat map:
        TableScriptFile f; if (!f.Load(p)) continue;
        const TsTable* t = f.Find("Script"); if (!t && f.Count() > 0) t = &f.At(0);
        if (!t) continue;
        int idxKey = t->ColIndex("ScrIndex"); if (idxKey < 0) idxKey = 0;
        int idxVal = t->ColIndex("ScrString"); if (idxVal < 0) idxVal = (idxKey == 0) ? 1 : 0;
        for (size_t r = 0; r < t->kRecords.size(); ++r) {
            if ((size_t)idxKey >= t->kRecords[r].kCells.size()) continue;
            if ((size_t)idxVal >= t->kRecords[r].kCells.size()) continue;
            const std::string& k = t->kRecords[r].kCells[idxKey];
            const std::string& v = t->kRecords[r].kCells[idxVal];
            if (k.empty()) continue;
            std::map<std::string,size_t>::iterator it = m_kFlat.find(k);
            if (it == m_kFlat.end()) {
                m_kFlat[k] = m_kFlatVal.size();
                m_kFlatVal.push_back(v);
            } else {
                m_kFlatVal[it->second] = v;
            }
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
    SHINELOG_INFO("ScriptStringBox loaded %u tables, %u unique entries",
                  (uint32)m_kTables.size(), (uint32)m_kFlat.size());
    return !m_kTables.empty();
}

bool ScriptStringBox::Lookup(const std::string& rIndex, std::string& rOut) const {
    std::map<std::string, size_t>::const_iterator it = m_kFlat.find(rIndex);
    if (it == m_kFlat.end()) return false;
    rOut = m_kFlatVal[it->second]; return true;
}

void ScriptStringBox::Clear() {
    for (size_t i = 0; i < m_kTables.size(); ++i) delete m_kTables[i];
    m_kTables.clear();
    m_kFlat.clear();
    m_kFlatVal.clear();
}

} // namespace fiesta
