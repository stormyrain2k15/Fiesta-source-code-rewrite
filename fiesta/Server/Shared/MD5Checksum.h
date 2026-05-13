// Server/Shared/MD5Checksum.h
// MD5 used for SHN data file checksum + token seed.
#ifndef SHINE_MD5CHECKSUM_H
#define SHINE_MD5CHECKSUM_H
#include "ShineTypes.h"

namespace shine {

class MD5Checksum {
public:
    MD5Checksum();
    void   Reset();
    void   Update(const void* pvData, size_t uiLen);
    void   Finalize(uint8 aDigest[16]);
    static void Compute(const void* pvData, size_t uiLen, uint8 aDigest[16]);
    static std::string ToHex(const uint8 aDigest[16]);
private:
    uint32 m_auiState[4];
    uint32 m_auiCount[2];
    uint8  m_aBuffer[64];
    void   Transform(const uint8* pBlock);
};

} // namespace shine
#endif
