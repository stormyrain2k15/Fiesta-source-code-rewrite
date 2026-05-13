// Server/Shared/ConfigParser.cpp
#include "ConfigParser.h"
#include "ShineLogSystem.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

namespace shine {

ConfigParser::ConfigParser() : m_bInDefine(false), m_bDoneEnd(false) {}

static std::string Trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && (s[a] == ' ' || s[a] == '\t')) ++a;
    while (b > a && (s[b-1] == ' ' || s[b-1] == '\t' || s[b-1] == '\r' || s[b-1] == '\n')) --b;
    return s.substr(a, b - a);
}

static std::string DirOf(const std::string& path) {
    size_t i = path.find_last_of("\\/");
    return (i == std::string::npos) ? std::string() : path.substr(0, i);
}

static std::string JoinPath(const std::string& dir, const std::string& rel) {
    if (rel.size() >= 2 && (rel[0] == '/' || rel[0] == '\\' || rel[1] == ':')) return rel;
    if (dir.empty()) return rel;
    char back = dir[dir.size() - 1];
    if (back == '/' || back == '\\') return dir + rel;
    return dir + "\\" + rel;
}

// Tokenise a record-data line: comma-separated mix of "string", number, ;comment-tail.
static std::vector<CfgValue> Tokenise(const std::string& sIn) {
    std::vector<CfgValue> out;
    // Strip trailing ';' comment.
    std::string s = sIn;
    {
        bool inStr = false;
        for (size_t i = 0; i < s.size(); ++i) {
            if (s[i] == '"') inStr = !inStr;
            else if (s[i] == ';' && !inStr) { s = s.substr(0, i); break; }
        }
    }
    size_t i = 0;
    while (i < s.size()) {
        while (i < s.size() && (s[i] == ' ' || s[i] == '\t' || s[i] == ',')) ++i;
        if (i >= s.size()) break;
        CfgValue v;
        if (s[i] == '"') {
            ++i;
            std::string acc;
            while (i < s.size() && s[i] != '"') { acc.push_back(s[i++]); }
            if (i < s.size()) ++i; // closing quote
            v.eType = CFT_STRING; v.kStr = acc;
        } else {
            std::string acc;
            while (i < s.size() && s[i] != ',' && s[i] != ' ' && s[i] != '\t') {
                acc.push_back(s[i++]);
            }
            // detect numeric vs string
            bool num = !acc.empty();
            for (size_t k = 0; k < acc.size(); ++k) {
                char c = acc[k];
                bool ok = (c >= '0' && c <= '9') || (k == 0 && (c == '-' || c == '+'));
                if (!ok) { num = false; break; }
            }
            if (num) {
                v.eType = CFT_INTEGER; v.iNum = (int64)strtoll(acc.c_str(), NULL, 10);
            } else {
                v.eType = CFT_STRING; v.kStr = acc;
            }
        }
        out.push_back(v);
    }
    return out;
}

bool ConfigParser::Load(const std::string& rPath) {
    std::vector<std::string> stk;
    return LoadInternal(rPath, stk);
}

bool ConfigParser::LoadInternal(const std::string& rPath, std::vector<std::string>& stk) {
    for (size_t i = 0; i < stk.size(); ++i)
        if (stk[i] == rPath) { SHINELOG_WARN("ConfigParser: include cycle on %s", rPath.c_str()); return false; }
    stk.push_back(rPath);

    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) { SHINELOG_WARN("ConfigParser: cannot open %s", rPath.c_str()); stk.pop_back(); return false; }
    std::string dir = DirOf(rPath);
    char line[2048];
    while (fgets(line, sizeof(line), fp)) {
        std::string s = Trim(std::string(line));
        if (s.empty() || s[0] == ';') continue;
        if (m_bDoneEnd) break;
        if (!ParseLine(s, dir, stk)) {
            SHINELOG_WARN("ConfigParser: bad line in %s : %s", rPath.c_str(), s.c_str());
        }
    }
    fclose(fp);
    stk.pop_back();
    return true;
}

bool ConfigParser::ParseLine(const std::string& s, const std::string& rDir,
                             std::vector<std::string>& stk) {
    // Directives.
    if (s == "#END")       { m_bDoneEnd = true; return true; }
    if (s == "#ENDDEFINE") { m_bInDefine = false; return true; }
    if (s.size() > 8 && s.compare(0, 8, "#DEFINE ") == 0) {
        std::string nm = Trim(s.substr(8));
        m_kCurrentDefine = nm;
        m_bInDefine = true;
        m_kSchema[nm].clear();
        return true;
    }
    if (s.size() > 9 && s.compare(0, 9, "#include ") == 0) {
        std::string rest = Trim(s.substr(9));
        if (!rest.empty() && rest[0] == '"') {
            size_t e = rest.find('"', 1);
            if (e == std::string::npos) return false;
            std::string rel = rest.substr(1, e - 1);
            return LoadInternal(JoinPath(rDir, rel), stk);
        }
        return false;
    }
    if (m_bInDefine) {
        // Schema field type lines:  <STRING> / <INTEGER>
        std::string t = Trim(s);
        if (t == "<STRING>")  m_kSchema[m_kCurrentDefine].push_back(CFT_STRING);
        else if (t == "<INTEGER>") m_kSchema[m_kCurrentDefine].push_back(CFT_INTEGER);
        else return false;
        return true;
    }
    // Data record line:  RECNAME  arg, arg, "arg" ...
    size_t spc = 0;
    while (spc < s.size() && s[spc] != ' ' && s[spc] != '\t') ++spc;
    std::string nm = s.substr(0, spc);
    std::string args = (spc < s.size()) ? Trim(s.substr(spc)) : std::string();
    return ParseRecordLine(nm, args);
}

bool ConfigParser::ParseRecordLine(const std::string& nm, const std::string& args) {
    std::vector<CfgValue> v = Tokenise(args);
    m_kRecords[nm].push_back(v);
    return true;
}

const std::vector<CfgRecord>& ConfigParser::Records(const std::string& nm) const {
    std::map<std::string, std::vector<CfgRecord> >::const_iterator it = m_kRecords.find(nm);
    return (it == m_kRecords.end()) ? m_kEmpty : it->second;
}

const CfgRecord* ConfigParser::First(const std::string& nm) const {
    std::map<std::string, std::vector<CfgRecord> >::const_iterator it = m_kRecords.find(nm);
    if (it == m_kRecords.end() || it->second.empty()) return NULL;
    return &it->second.front();
}

const std::vector<CfgFieldType>* ConfigParser::Schema(const std::string& nm) const {
    std::map<std::string, std::vector<CfgFieldType> >::const_iterator it = m_kSchema.find(nm);
    return (it == m_kSchema.end()) ? NULL : &it->second;
}

} // namespace shine
