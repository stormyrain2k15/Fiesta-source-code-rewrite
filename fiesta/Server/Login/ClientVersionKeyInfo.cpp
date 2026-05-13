// Server/Login/ClientVersionKeyInfo.cpp
#include "ClientVersionKeyInfo.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <ctype.h>

namespace shine {

ClientVersionKeyInfo& ClientVersionKeyInfo::Get() { static ClientVersionKeyInfo s; return s; }

bool ClientVersionKeyInfo::Load(const char* szPath) {
    FILE* fp = NULL; fopen_s(&fp, szPath, "rb");
    if (!fp) { SHINELOG_WARN("ClientVersionKeyInfo: missing %s", szPath); return false; }
    char buf[64]; size_t n = fread(buf, 1, sizeof(buf) - 1, fp); fclose(fp);
    buf[n] = 0;
    // Trim whitespace/newlines.
    size_t a = 0, b = n;
    while (a < b && isspace((unsigned char)buf[a])) ++a;
    while (b > a && isspace((unsigned char)buf[b-1])) --b;
    m_kKey.assign(buf + a, b - a);
    SHINELOG_INFO("ClientVersionKeyInfo loaded: %s", m_kKey.c_str());
    return !m_kKey.empty();
}

bool ClientVersionKeyInfo::IsAcceptable(const std::string& rClientKey) const {
    // Exact-match policy. Future patch days swap the file out atomically.
    return !m_kKey.empty() && rClientKey == m_kKey;
}

} // namespace shine
