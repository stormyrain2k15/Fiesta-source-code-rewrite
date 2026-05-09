// Server/Shared/ServerInfo.cpp
#include "ServerInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

namespace fiesta {

std::string ServerInfo::s_kEmpty;

ServerInfo::ServerInfo() {}

static std::string Trim(const std::string& s) {
    size_t a = 0, b = s.size();
    while (a < b && isspace((unsigned char)s[a])) ++a;
    while (b > a && isspace((unsigned char)s[b-1])) --b;
    return s.substr(a, b - a);
}

bool ServerInfo::Load(const char* szPath) {
    FILE* fp = NULL; fopen_s(&fp, szPath, "rb");
    if (!fp) return false;
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        std::string s(line);
        size_t hash = s.find('#');
        if (hash != std::string::npos) s = s.substr(0, hash);
        size_t eq = s.find('=');
        if (eq == std::string::npos) continue;
        std::string k = Trim(s.substr(0, eq));
        std::string v = Trim(s.substr(eq + 1));
        if (!k.empty()) m_kKv[k] = v;
    }
    fclose(fp);
    return true;
}

const std::string& ServerInfo::GetString(const std::string& k, const std::string& d) const {
    std::map<std::string, std::string>::const_iterator it = m_kKv.find(k);
    if (it == m_kKv.end()) { s_kEmpty = d; return s_kEmpty; }
    return it->second;
}
int    ServerInfo::GetInt (const std::string& k, int    d) const { const std::string& v = GetString(k); return v.empty() ? d : atoi(v.c_str()); }
uint16 ServerInfo::GetU16 (const std::string& k, uint16 d) const { return (uint16)GetInt(k, d); }
bool   ServerInfo::GetBool(const std::string& k, bool   d) const {
    const std::string& v = GetString(k);
    if (v.empty()) return d;
    return v=="1"||v=="true"||v=="True"||v=="TRUE"||v=="yes";
}

} // namespace fiesta
