// Server/DataReader/TableScriptFile.cpp
#include "TableScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <set>

namespace fiesta {

// ----- Pass 1.26 column auditor for the TS-format files (mirrors the
// ShnAudit hook in ShnRegistry). Stamp every (table, column) pair the
// runtime asks for via TsTable::GetCell/GetInt/GetStr; at the end of
// boot we walk every loaded file and warn for columns that were declared
// in a #ColumnName but never read.
namespace {
    static std::map<std::string, std::set<std::string> > s_kTsReads;
    static std::vector<TsTable>                          s_kTsLoaded;
    static bool                                          s_bTsAuditOn = true;
}
void TsAudit_Record(const std::string& rTable, const std::string& rCol) {
    if (!s_bTsAuditOn) return;
    s_kTsReads[rTable].insert(rCol);
}
void TsAudit_Reset() { s_kTsReads.clear(); s_kTsLoaded.clear(); }
void TsAudit_RegisterLoaded(const TsTable& rT) { s_kTsLoaded.push_back(rT); }
void TsAudit_VisitTable(const TsTable& rT) {
    std::map<std::string, std::set<std::string> >::const_iterator it
        = s_kTsReads.find(rT.kName);
    const std::set<std::string>* reads = (it == s_kTsReads.end()) ? NULL : &it->second;
    for (size_t c = 0; c < rT.kColumns.size(); ++c) {
        if (!reads || reads->find(rT.kColumns[c].kName) == reads->end()) {
            SHINELOG_WARN("TsAudit: %s.%s parsed but not consumed",
                          rT.kName.c_str(), rT.kColumns[c].kName.c_str());
        }
    }
}
void TsAudit_EmitReport() {
    // Walk every TS table loaded since the last TsAudit_Reset(), de-dup by
    // (table-name, column-set) so e.g. the per-map MobRegen tables don't
    // emit one warning per file -- the column shape is shared.
    std::set<std::string> seen;
    uint32 unread = 0;
    for (size_t i = 0; i < s_kTsLoaded.size(); ++i) {
        const TsTable& t = s_kTsLoaded[i];
        if (seen.find(t.kName) != seen.end()) continue;
        seen.insert(t.kName);
        std::map<std::string, std::set<std::string> >::const_iterator it
            = s_kTsReads.find(t.kName);
        const std::set<std::string>* reads = (it == s_kTsReads.end()) ? NULL : &it->second;
        for (size_t c = 0; c < t.kColumns.size(); ++c) {
            if (!reads || reads->find(t.kColumns[c].kName) == reads->end()) {
                SHINELOG_WARN("TsAudit: %s.%s parsed but not consumed",
                              t.kName.c_str(), t.kColumns[c].kName.c_str());
                ++unread;
            }
        }
    }
    SHINELOG_INFO("TsColumnAuditor: %u unique TS tables audited, %u unconsumed columns",
                  (uint32)seen.size(), unread);
}

// ---------------- TsTable ----------------
int TsTable::ColIndex(const char* szName) const {
    for (size_t i = 0; i < kColumns.size(); ++i)
        if (_stricmp(kColumns[i].kName.c_str(), szName) == 0) return (int)i;
    return -1;
}

bool TsTable::GetCell(size_t row, size_t col, std::string& rOut) const {
    if (row >= kRecords.size() || col >= kRecords[row].kCells.size()) return false;
    rOut = kRecords[row].kCells[col];
    return true;
}
bool TsTable::GetCell(size_t row, const char* szCol, std::string& rOut) const {
    int c = ColIndex(szCol); if (c < 0) return false;
    TsAudit_Record(kName, szCol);
    return GetCell(row, (size_t)c, rOut);
}
int64 TsTable::GetInt(size_t row, const char* szCol, int64 iDefault) const {
    std::string v; if (!GetCell(row, szCol, v) || v.empty() || v == "-") return iDefault;
    return _atoi64(v.c_str());
}
double TsTable::GetReal(size_t row, const char* szCol, double dDefault) const {
    std::string v; if (!GetCell(row, szCol, v) || v.empty() || v == "-") return dDefault;
    return atof(v.c_str());
}
std::string TsTable::GetStr(size_t row, const char* szCol, const char* szDefault) const {
    std::string v; if (!GetCell(row, szCol, v)) return szDefault ? szDefault : "";
    return v;
}

// ---------------- TableScriptFile ----------------
TableScriptFile::TableScriptFile() : m_kDelim(" \t,") {}

void TableScriptFile::Trim(std::string& s) {
    size_t a = 0; while (a < s.size() && (s[a] == ' ' || s[a] == '\t' || s[a] == '\r' || s[a] == '\n')) ++a;
    size_t b = s.size(); while (b > a && (s[b-1] == ' ' || s[b-1] == '\t' || s[b-1] == '\r' || s[b-1] == '\n')) --b;
    s = s.substr(a, b - a);
}

// Translate "\x20", "\o042", "\t", "\n" escape tokens into a literal string.
std::string TableScriptFile::ResolveEscape(const std::string& rTok) {
    std::string out;
    for (size_t i = 0; i < rTok.size(); ) {
        char c = rTok[i];
        if (c != '\\' || i + 1 >= rTok.size()) { out.push_back(c); ++i; continue; }
        char n = rTok[i+1];
        if (n == 'x' || n == 'X') {
            unsigned int v = 0; size_t j = i + 2; size_t k = 0;
            while (j < rTok.size() && k < 2) {
                char h = rTok[j];
                if      (h >= '0' && h <= '9') v = (v << 4) | (unsigned)(h - '0');
                else if (h >= 'a' && h <= 'f') v = (v << 4) | (unsigned)(h - 'a' + 10);
                else if (h >= 'A' && h <= 'F') v = (v << 4) | (unsigned)(h - 'A' + 10);
                else break;
                ++j; ++k;
            }
            if (k > 0) { out.push_back((char)(v & 0xFF)); i = j; continue; }
        } else if (n == 'o' || n == 'O') {
            unsigned int v = 0; size_t j = i + 2; size_t k = 0;
            while (j < rTok.size() && k < 3 && rTok[j] >= '0' && rTok[j] <= '7') {
                v = (v * 8) + (unsigned)(rTok[j] - '0'); ++j; ++k;
            }
            if (k > 0) { out.push_back((char)(v & 0xFF)); i = j; continue; }
        } else if (n == 't') { out.push_back('\t'); i += 2; continue; }
        else   if (n == 'n') { out.push_back('\n'); i += 2; continue; }
        else   if (n == 'r') { out.push_back('\r'); i += 2; continue; }
        else   if (n == '\\'){ out.push_back('\\'); i += 2; continue; }
        out.push_back(c); ++i;
    }
    return out;
}

// Tokenise on any of the delimiter characters; honors double-quoted strings.
void TableScriptFile::SplitFields(const std::string& rLine, const std::string& rDelim,
                                   std::vector<std::string>& rOut) {
    rOut.clear();
    bool inQuote = false;
    std::string cur;
    for (size_t i = 0; i < rLine.size(); ++i) {
        char c = rLine[i];
        if (inQuote) {
            if (c == '"') { inQuote = false; }
            else cur.push_back(c);
        } else if (c == '"') {
            inQuote = true;
        } else if (rDelim.find(c) != std::string::npos) {
            if (!cur.empty()) { rOut.push_back(cur); cur.clear(); }
        } else {
            cur.push_back(c);
        }
    }
    if (!cur.empty()) rOut.push_back(cur);
}

TsFieldType TableScriptFile::ParseType(const std::string& rTok, int& rLenOut) {
    rLenOut = 0;
    // Handle "STRING[33]" form.
    std::string base = rTok;
    size_t lb = base.find('[');
    if (lb != std::string::npos) {
        size_t rb = base.find(']', lb + 1);
        if (rb != std::string::npos)
            rLenOut = atoi(base.substr(lb + 1, rb - lb - 1).c_str());
        base = base.substr(0, lb);
    }
    // Normalize.
    std::string lo; lo.reserve(base.size());
    for (size_t i = 0; i < base.size(); ++i) lo.push_back((char)tolower((unsigned char)base[i]));

    if (lo == "byte")                                                     return TS_T_BYTE;
    if (lo == "word")                                                     return TS_T_WORD;
    if (lo == "dword" || lo == "dwrd" || lo == "uint" || lo == "uint32")  return TS_T_DWORD;
    if (lo == "index" || lo == "indx")                                    return TS_T_INDEX;
    if (lo == "string" || lo == "str")                                    return TS_T_STRING;
    if (lo == "float"  || lo == "single" || lo == "double" || lo == "real") return TS_T_FLOAT;
    return TS_T_UNKNOWN;
}

bool TableScriptFile::ApplyDirective(const std::string& rDir, const std::vector<std::string>& rArgs) {
    if (rDir == "ignore") {
        for (size_t i = 0; i < rArgs.size(); ++i) {
            std::string s = ResolveEscape(rArgs[i]);
            for (size_t j = 0; j < s.size(); ++j) m_kIgnore.push_back(s[j]);
        }
        return true;
    }
    if (rDir == "exchange") {
        if (rArgs.size() >= 2) {
            std::string from = ResolveEscape(rArgs[0]);
            std::string to   = ResolveEscape(rArgs[1]);
            m_kExchange.push_back(std::make_pair(from, to));
            // If a file declares "exchange # space", string cells will contain
            // spaces -- drop ' ' from the default delimiter set so we don't
            // accidentally tokenise dialog text on word boundaries. Tab + ','
            // remain valid separators.
            if (to == " ") {
                std::string clean;
                for (size_t i = 0; i < m_kDelim.size(); ++i)
                    if (m_kDelim[i] != ' ') clean.push_back(m_kDelim[i]);
                m_kDelim.swap(clean);
            }
        }
        return true;
    }
    if (rDir == "delimiter" || rDir == "delimeter") {
        if (!rArgs.empty()) {
            std::string s = ResolveEscape(rArgs[0]);
            // We always also accept ' ', '\t', ',' to be safe (real files mix these).
            m_kDelim = s;
            if (m_kDelim.find(' ')  == std::string::npos) m_kDelim.push_back(' ');
            if (m_kDelim.find('\t') == std::string::npos) m_kDelim.push_back('\t');
            if (m_kDelim.find(',')  == std::string::npos) m_kDelim.push_back(',');
        }
        return true;
    }
    if (rDir == "table") {
        if (!rArgs.empty()) BeginTable(rArgs[0]);
        return true;
    }
    if (rDir == "columntype") {
        return ApplyColumnTypes(rArgs);
    }
    if (rDir == "columnname") {
        return ApplyColumnNames(rArgs);
    }
    if (rDir == "record") {
        return AppendRecord(m_kCurTable, rArgs);
    }
    if (rDir == "recordin") {
        if (rArgs.empty()) return false;
        std::string tn = rArgs[0];
        std::vector<std::string> tail(rArgs.begin() + 1, rArgs.end());
        return AppendRecord(tn, tail);
    }
    if (rDir == "end") {
        return true; // EOF marker
    }
    if (rDir == "include") {
        // Not used by Shine TableScript files; ignore quietly.
        return true;
    }
    return false;
}

bool TableScriptFile::BeginTable(const std::string& rName) {
    m_kCurTable = rName;
    if (m_kIndex.find(rName) != m_kIndex.end()) return true;
    TsTable t; t.kName = rName;
    m_kIndex[rName] = m_kTables.size();
    m_kTables.push_back(t);
    return true;
}

bool TableScriptFile::ApplyColumnTypes(const std::vector<std::string>& rArgs) {
    if (m_kCurTable.empty()) return false;
    TsTable& t = m_kTables[m_kIndex[m_kCurTable]];
    t.kColumns.clear();
    for (size_t i = 0; i < rArgs.size(); ++i) {
        TsColumn c; c.iLen = 0;
        c.eType = ParseType(rArgs[i], c.iLen);
        char buf[16]; sprintf_s(buf, sizeof(buf), "Col%u", (unsigned)i);
        c.kName = buf;
        t.kColumns.push_back(c);
    }
    return true;
}

bool TableScriptFile::ApplyColumnNames(const std::vector<std::string>& rArgs) {
    if (m_kCurTable.empty()) return false;
    TsTable& t = m_kTables[m_kIndex[m_kCurTable]];
    // If column types weren't declared, create string columns to match.
    if (t.kColumns.empty()) {
        for (size_t i = 0; i < rArgs.size(); ++i) {
            TsColumn c; c.iLen = 0; c.eType = TS_T_STRING; c.kName = rArgs[i];
            t.kColumns.push_back(c);
        }
        return true;
    }
    for (size_t i = 0; i < rArgs.size() && i < t.kColumns.size(); ++i)
        t.kColumns[i].kName = rArgs[i];
    return true;
}

bool TableScriptFile::AppendRecord(const std::string& rTableName,
                                    const std::vector<std::string>& rFields) {
    if (rTableName.empty()) return false;
    std::map<std::string,size_t>::iterator it = m_kIndex.find(rTableName);
    if (it == m_kIndex.end()) {
        // Implicit table create (some files use #recordin without earlier #Table).
        BeginTable(rTableName);
        it = m_kIndex.find(rTableName);
    }
    TsTable& t = m_kTables[it->second];
    TsRecord r; r.kCells = rFields;
    t.kRecords.push_back(r);
    return true;
}

bool TableScriptFile::Load(const std::string& rPath) {
    m_kTables.clear(); m_kIndex.clear();
    m_kIgnore.clear(); m_kExchange.clear();
    m_kCurTable.clear();
    m_kDelim = " \t,";

    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) { SHINELOG_ERROR("TableScriptFile: cannot open %s", rPath.c_str()); return false; }

    char raw[4096];
    while (fgets(raw, sizeof(raw), fp)) {
        std::string line(raw);
        // Strip CR/LF.
        while (!line.empty() && (line[line.size()-1] == '\r' || line[line.size()-1] == '\n'))
            line.erase(line.size() - 1);
        // Strip trailing comment ';' (but only outside of quotes).
        {
            bool inQ = false; size_t cut = std::string::npos;
            for (size_t i = 0; i < line.size(); ++i) {
                char c = line[i];
                if (c == '"') inQ = !inQ;
                else if (c == ';' && !inQ) { cut = i; break; }
            }
            if (cut != std::string::npos) line.erase(cut);
        }
        // Apply #ignore (drop characters).
        if (!m_kIgnore.empty()) {
            std::string out; out.reserve(line.size());
            for (size_t i = 0; i < line.size(); ++i) {
                bool drop = false;
                for (size_t j = 0; j < m_kIgnore.size(); ++j)
                    if (line[i] == m_kIgnore[j]) { drop = true; break; }
                if (!drop) out.push_back(line[i]);
            }
            line.swap(out);
        }
        // Apply #exchange (textual replace), but only after we recognise the
        // directive itself -- so detect the directive on the *raw* (post-ignore)
        // line first by looking at the leading non-space char.
        std::string scan = line; Trim(scan);
        if (scan.empty()) continue;

        // Lookup: lines that *start* with '#' are directives.
        if (scan[0] == '#') {
            // Locate end of directive name (first whitespace).
            size_t e = 1;
            while (e < scan.size() && scan[e] != ' ' && scan[e] != '\t') ++e;
            std::string dname = scan.substr(1, e - 1);
            for (size_t i = 0; i < dname.size(); ++i) dname[i] = (char)tolower((unsigned char)dname[i]);
            // Skip whitespace.
            size_t s = e;
            while (s < scan.size() && (scan[s] == ' ' || scan[s] == '\t')) ++s;
            std::string body = scan.substr(s);
            std::vector<std::string> args;
            if (dname == "record" || dname == "recordin") {
                // Apply #exchange (textual replace) prior to field tokenisation
                // so '#'->' ' rewrites work inside string cells.
                for (size_t i = 0; i < m_kExchange.size(); ++i) {
                    const std::string& f = m_kExchange[i].first;
                    const std::string& t = m_kExchange[i].second;
                    if (f.empty()) continue;
                    std::string out; size_t k = 0;
                    while (k < body.size()) {
                        if (body.compare(k, f.size(), f) == 0) { out.append(t); k += f.size(); }
                        else                                   { out.push_back(body[k]); ++k; }
                    }
                    body.swap(out);
                }
                SplitFields(body, m_kDelim, args);
            } else {
                SplitFields(body, " \t", args);
            }
            ApplyDirective(dname, args);
            continue;
        }

        // Data line (rare -- files mostly use #Record / #recordin). We treat it
        // as an implicit record for the current table.
        if (m_kCurTable.empty()) continue;
        // Apply #exchange to pre-tokenisation form so '#'->' ' rewrites work.
        for (size_t i = 0; i < m_kExchange.size(); ++i) {
            const std::string& f = m_kExchange[i].first;
            const std::string& t = m_kExchange[i].second;
            if (f.empty()) continue;
            std::string out; size_t k = 0;
            while (k < line.size()) {
                if (line.compare(k, f.size(), f) == 0) { out.append(t); k += f.size(); }
                else                                  { out.push_back(line[k]); ++k; }
            }
            line.swap(out);
        }
        std::vector<std::string> cells;
        SplitFields(line, m_kDelim, cells);
        if (!cells.empty()) AppendRecord(m_kCurTable, cells);
    }
    fclose(fp);
    SHINELOG_INFO("TableScriptFile loaded '%s' (%u tables)", rPath.c_str(), (uint32)m_kTables.size());
    // Register every parsed table with the auditor so the boot-end walker
    // can diff #ColumnName declarations against the binders' read-set.
    for (size_t i = 0; i < m_kTables.size(); ++i) {
        TsAudit_RegisterLoaded(m_kTables[i]);
    }
    return true;
}

const TsTable* TableScriptFile::Find(const std::string& rName) const {
    std::map<std::string,size_t>::const_iterator it = m_kIndex.find(rName);
    return (it == m_kIndex.end()) ? NULL : &m_kTables[it->second];
}

} // namespace fiesta
