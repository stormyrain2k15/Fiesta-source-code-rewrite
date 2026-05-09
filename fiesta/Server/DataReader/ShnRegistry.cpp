// Server/DataReader/ShnRegistry.cpp
#include "ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"
#include <windows.h>
#include <ctype.h>
#include <set>
#include <string>

namespace fiesta {

// ----- Column audit: track every read (table, column) so we can warn at
// boot for any column the binders never asked for. The "current" table is
// set as a side-effect of GetTable() and updated whenever a new ShnFile
// pointer is queried. Reads stamp into the per-table set.
namespace {
    static std::string                                    s_kAuditCurrent;
    static std::map<std::string, std::set<std::string> >  s_kAuditReads;
    static const ShnFile*                                 s_pkAuditLastFile = NULL;
}
void ShnAudit_BeginTable(const std::string& rT) { s_kAuditCurrent = rT; }
void ShnAudit_RecordRead(const std::string& rC) {
    if (!s_kAuditCurrent.empty()) s_kAuditReads[s_kAuditCurrent].insert(rC);
}
void ShnAudit_EmitReport(const ShnRegistry& rReg) {
    uint32 totalUnread = 0;
    uint32 totalAudited = 0;
    std::map<std::string, std::set<std::string> >::const_iterator it;
    for (it = s_kAuditReads.begin(); it != s_kAuditReads.end(); ++it) {
        // BLOCK rule: generic SHN tooling (audit, CSV export) MUST skip
        // quest/scenario SHNs. They're loaded by the runtime via the
        // dedicated QuestShnReader; a generic column walk would either
        // mis-parse them or report every column as "unconsumed".
        if (ShnRegistry::IsQuestShn(it->first)) continue;
        const ShnFile* t = rReg.GetTable(it->first);
        if (!t) continue;
        if (t->IsQuestDeferred()) continue;        // belt + suspenders
        ++totalAudited;
        const std::vector<ShnColumn>& cols = t->Columns();
        for (size_t c = 0; c < cols.size(); ++c) {
            if (it->second.find(cols[c].kName) == it->second.end()) {
                SHINELOG_WARN("ShnAudit: %s.%s parsed but not consumed",
                              it->first.c_str(), cols[c].kName.c_str());
                ++totalUnread;
            }
        }
    }
    SHINELOG_INFO("ShnColumnAuditor: %u tables audited, %u unconsumed columns",
                  totalAudited, totalUnread);

    // ----- Pass 2: registry-level "unowned table" audit ------------------
    // Column-read auditing only catches partial ownership of tables that
    // were already opened. Any SHN that loaded successfully but no
    // binder/system ever opened doesn't appear above -- and that is
    // exactly the signal we need to find missing-system gaps. Walk the
    // full registry once and flag anything that never showed up in
    // s_kAuditReads.
    uint32 unowned = 0;
    uint32 deferred = 0;
    for (ShnRegistry::iterator rit = rReg.begin(); rit != rReg.end(); ++rit) {
        const std::string& name = rit->first;
        const ShnFile*     f    = rit->second;
        // Skip the dedicated quest path -- those are owned by
        // QuestShnReader and won't appear in s_kAuditReads.
        if (ShnRegistry::IsQuestShn(name)) { ++deferred; continue; }
        if (f && f->IsQuestDeferred())     { ++deferred; continue; }
        if (s_kAuditReads.find(name) == s_kAuditReads.end()) {
            SHINELOG_WARN("ShnAudit: table '%s' loaded but no system "
                          "owns it (no GetTable/ShnGet* call observed)",
                          name.c_str());
            ++unowned;
        }
    }
    SHINELOG_INFO("ShnRegistryAuditor: %u unowned table(s), %u quest-deferred (skipped)",
                  unowned, deferred);
}

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
        // Quest / PineScript SHNs need a dedicated parser (separate
        // on-disk shape, scripted bodies). The runtime server LOADS them
        // -- quests must work end-to-end -- but they do not go through
        // the generic ShnFile parser; instead we tag them and hand the
        // caller a placeholder so a typed quest loader can pick up the
        // file later in boot. Generic CSV exporters / audit tools call
        // `ShnRegistry::IsQuestShn(stem)` to filter them out without
        // touching the file at all.
        if (ShnRegistry::IsQuestShn(stem)) {
            SHINELOG_INFO("ShnRegistry: registering quest/scenario SHN '%s' "
                          "(deferred to quest loader)", name.c_str());
            ShnFile* placeholder = new ShnFile();
            placeholder->MarkAsQuestDeferred(rDir + "\\" + name);
            std::map<std::string, ShnFile*>::iterator it = rOut.find(stem);
            if (it != rOut.end()) { delete it->second; rOut.erase(it); }
            rOut[stem] = placeholder;
            continue;
        }
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

bool ShnRegistry::IsQuestShn(const std::string& rStem) {
    std::string lower = rStem;
    for (size_t i = 0; i < lower.size(); ++i)
        lower[i] = (char)::tolower((unsigned char)lower[i]);
    return lower.find("quest") != std::string::npos
        || lower.find("pinescript") != std::string::npos;
}

size_t ShnRegistry::LoadAll(const std::string& rRoot) {
    EnumerateShn(rRoot + "\\Shine",      m_kAll);
    EnumerateShn(rRoot + "\\Shine-1",    m_kAll);
    EnumerateShn(rRoot + "\\Shine\\View",m_kAll);   // Client-shape view tables
    SHINELOG_INFO("ShnRegistry: %u tables loaded from %s",
                  (uint32)m_kAll.size(), rRoot.c_str());
    return m_kAll.size();
}

const ShnFile* ShnRegistry::GetTable(const std::string& rName) const {
    std::map<std::string, ShnFile*>::const_iterator it = m_kAll.find(rName);
    const ShnFile* p = (it == m_kAll.end()) ? NULL : it->second;
    if (p) {
        // Side-effect: set the audit "current table" so downstream
        // ShnGet*() calls record their column reads against `rName`.
        ShnAudit_BeginTable(rName);
        s_pkAuditLastFile = p;
    }
    return p;
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
    if (&rTab == s_pkAuditLastFile) ShnAudit_RecordRead(rColumn);
    int32 c = FindCol(rTab, rColumn);
    if (c < 0) return std::string();
    if (uiRow >= rTab.Rows().size()) return std::string();
    const std::vector<ShnValue>& row = rTab.Rows()[uiRow];
    if ((size_t)c >= row.size()) return std::string();
    return row[c].kStr;
}
uint32 ShnGetU32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn) {
    if (&rTab == s_pkAuditLastFile) ShnAudit_RecordRead(rColumn);
    int32 c = FindCol(rTab, rColumn);
    if (c < 0) return 0u;
    if (uiRow >= rTab.Rows().size()) return 0u;
    const std::vector<ShnValue>& row = rTab.Rows()[uiRow];
    if ((size_t)c >= row.size()) return 0u;
    return (uint32)row[c].iVal;
}
int32 ShnGetI32(const ShnFile& rTab, size_t uiRow, const std::string& rColumn) {
    if (&rTab == s_pkAuditLastFile) ShnAudit_RecordRead(rColumn);
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
