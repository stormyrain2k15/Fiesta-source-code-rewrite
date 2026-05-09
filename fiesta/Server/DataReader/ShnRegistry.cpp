// Server/DataReader/ShnRegistry.cpp
#include "ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>

namespace fiesta {

ShnRegistry& ShnRegistry::Get() { static ShnRegistry s; return s; }

ShnRegistry::~ShnRegistry() {
    for (std::map<std::string, ShnFile*>::iterator it = m_kAll.begin();
         it != m_kAll.end(); ++it) delete it->second;
    m_kAll.clear();
}

static void EnumerateShn(const std::string& rDir, std::map<std::string, ShnFile*>& rOut) {
    std::string pat = rDir + "\\*.shn";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pat.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return;
    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        std::string name = fd.cFileName;
        size_t dot = name.rfind('.');
        if (dot == std::string::npos) continue;
        std::string stem = name.substr(0, dot);
        ShnFile* f = new ShnFile();
        if (!f->LoadFromFile(rDir + "\\" + name)) {
            SHINELOG_WARN("ShnRegistry: parse failed for %s", name.c_str());
            delete f;
            continue;
        }
        // Last write wins; the caller's directory order decides priority.
        std::map<std::string, ShnFile*>::iterator it = rOut.find(stem);
        if (it != rOut.end()) { delete it->second; rOut.erase(it); }
        rOut[stem] = f;
    } while (FindNextFileA(h, &fd));
    FindClose(h);
}

size_t ShnRegistry::LoadAll(const std::string& rRoot) {
    EnumerateShn(rRoot + "\\Shine",   m_kAll);
    EnumerateShn(rRoot + "\\Shine-1", m_kAll);
    SHINELOG_INFO("ShnRegistry: %u tables loaded from %s",
                  (uint32)m_kAll.size(), rRoot.c_str());
    return m_kAll.size();
}

const ShnFile* ShnRegistry::GetTable(const std::string& rName) const {
    std::map<std::string, ShnFile*>::const_iterator it = m_kAll.find(rName);
    return (it == m_kAll.end()) ? NULL : it->second;
}

bool ShnRegistry::ColumnIndex(const std::string& rTable, const std::string& rColumn,
                              uint32& uiOut) const
{
    const ShnFile* t = GetTable(rTable);
    if (!t) return false;
    const std::vector<ShnColumn>& cols = t->Columns();
    for (uint32 i = 0; i < cols.size(); ++i) {
        if (cols[i].kName == rColumn) { uiOut = i; return true; }
    }
    return false;
}

namespace {
    static int32 FindCol(const ShnFile& rTab, const std::string& rColumn) {
        const std::vector<ShnColumn>& cols = rTab.Columns();
        for (size_t i = 0; i < cols.size(); ++i) {
            if (cols[i].kName == rColumn) return (int32)i;
        }
        return -1;
    }
}

std::string ShnGetStr(const ShnFile& rTab, size_t uiRow, const std::string& rColumn) {
    int32 c = FindCol(rTab, rColumn);
    if (c < 0) return std::string();
    if (uiRow >= rTab.Rows().size()) return std::string();
    const std::vector<ShnValue>& row = rTab.Rows()[uiRow];
    if ((size_t)c >= row.size()) return std::string();
    return row[c].kStr;
}
uint32 ShnGetU32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn) {
    int32 c = FindCol(rTab, rColumn);
    if (c < 0) return 0u;
    if (uiRow >= rTab.Rows().size()) return 0u;
    const std::vector<ShnValue>& row = rTab.Rows()[uiRow];
    if ((size_t)c >= row.size()) return 0u;
    return (uint32)row[c].iVal;
}
int32 ShnGetI32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn) {
    int32 c = FindCol(rTab, rColumn);
    if (c < 0) return 0;
    if (uiRow >= rTab.Rows().size()) return 0;
    const std::vector<ShnValue>& row = rTab.Rows()[uiRow];
    if ((size_t)c >= row.size()) return 0;
    return (int32)row[c].iVal;
}
float ShnGetF32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn) {
    int32 c = FindCol(rTab, rColumn);
    if (c < 0) return 0.0f;
    if (uiRow >= rTab.Rows().size()) return 0.0f;
    const std::vector<ShnValue>& row = rTab.Rows()[uiRow];
    if ((size_t)c >= row.size()) return 0.0f;
    return row[c].fVal;
}

} // namespace fiesta
