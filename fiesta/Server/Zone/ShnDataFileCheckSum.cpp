// Server/Zone/ShnDataFileCheckSum.cpp
// MD5 over each .shn at boot; mismatch (if a CHECKSUMS file is present)
// emits a critical log and refuses to start. Optional -- absence of
// CHECKSUMS means the audit is skipped.
#include "../Shared/ShineLogSystem.h"
#include "../Shared/Md5/MD5Checksum.h"
#include <stdio.h>

namespace fiesta {

class ShnDataFileCheckSum {
public:
    static bool VerifyOne(const std::string& rPath, const std::string& rExpected);
};

bool ShnDataFileCheckSum::VerifyOne(const std::string& rPath, const std::string& rExpected) {
    if (rExpected.empty()) return true;
    FILE* fp = fopen(rPath.c_str(), "rb");
    if (!fp) return false;
    fseek(fp, 0, SEEK_END); long n = ftell(fp); fseek(fp, 0, SEEK_SET);
    std::string s; s.resize((size_t)n);
    if (n > 0) fread(&s[0], 1, (size_t)n, fp);
    fclose(fp);
    std::string actual = MD5Checksum::HexOf(s);
    if (actual != rExpected) {
        SHINELOG_FATAL("CHECKSUM mismatch '%s' got=%s want=%s",
                       rPath.c_str(), actual.c_str(), rExpected.c_str());
        return false;
    }
    return true;
}

} // namespace fiesta
