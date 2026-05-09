// Server/DataReader/ShnDataFileCheckSum.h
// guard that refuses unsafe parsing of quest SHNs and verifies file integrity.
//   Spec rule: quest SHNs are not parsed by this codebase.
#ifndef FIESTA_SHNDATAFILECHECKSUM_H
#define FIESTA_SHNDATAFILECHECKSUM_H
#include "../Shared/ShineTypes.h"
#include "../Shared/MD5Checksum.h"
#include <map>
#include <string>

namespace fiesta {

class ShnDataFileCheckSum {
public:
    ShnDataFileCheckSum();

    // Logical-name allow/deny list.
    bool   IsQuestSHN(const std::string& rLogicalName) const;
    bool   IsAllowed (const std::string& rLogicalName) const;

    // Verify file MD5 against an expected list (loaded from CheckSum.shn / .txt).
    bool   Verify(const std::string& rLogicalName, const void* pv, size_t n) const;
    void   SetExpected(const std::string& rLogical, const std::string& rHex);
    bool   HasExpected(const std::string& rLogical) const;
private:
    std::map<std::string, std::string> m_kExpectedMd5; // logical -> hex
};

} // namespace fiesta
#endif
