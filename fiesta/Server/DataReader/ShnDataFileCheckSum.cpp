// Server/DataReader/ShnDataFileCheckSum.cpp
#include "ShnDataFileCheckSum.h"
#include "../Shared/MD5Checksum.h"
#include "../Shared/ShineLogSystem.h"
#include <string.h>
#include <ctype.h>

namespace fiesta {

ShnDataFileCheckSum::ShnDataFileCheckSum() {}

static std::string ToLower(const std::string& s) {
    std::string r(s); for (size_t i = 0; i < r.size(); ++i) r[i] = (char)tolower((unsigned char)r[i]);
    return r;
}

bool ShnDataFileCheckSum::IsQuestSHN(const std::string& rLogical) const {
    // Conservative guard. Spec rule 02: quest SHNs are protected.
    std::string s = ToLower(rLogical);
    if (s.find("quest") != std::string::npos) return true;
    if (s.find("pinescript") != std::string::npos) return true;
    if (s == "questdata" || s == "questdialog" || s == "questspecies"
        || s == "questevent" || s == "questframework") return true;
    return false;
}

bool ShnDataFileCheckSum::IsAllowed(const std::string& rLogical) const {
    return !IsQuestSHN(rLogical);
}

void ShnDataFileCheckSum::SetExpected(const std::string& rLogical, const std::string& rHex) {
    m_kExpectedMd5[ToLower(rLogical)] = ToLower(rHex);
}

bool ShnDataFileCheckSum::HasExpected(const std::string& rLogical) const {
    return m_kExpectedMd5.find(ToLower(rLogical)) != m_kExpectedMd5.end();
}

bool ShnDataFileCheckSum::Verify(const std::string& rLogical, const void* pv, size_t n) const {
    std::map<std::string, std::string>::const_iterator it = m_kExpectedMd5.find(ToLower(rLogical));
    if (it == m_kExpectedMd5.end()) return true; // no expectation registered -> permissive
    uint8 d[16]; MD5Checksum::Compute(pv, n, d);
    std::string got = MD5Checksum::ToHex(d);
    bool ok = (got == it->second);
    if (!ok) SHINELOG_ERROR("Checksum mismatch for %s -- got %s expected %s",
                            rLogical.c_str(), got.c_str(), it->second.c_str());
    return ok;
}

} // namespace fiesta
