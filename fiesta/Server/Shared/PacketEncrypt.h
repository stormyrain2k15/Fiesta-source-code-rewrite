// Server/Shared/PacketEncrypt.h
// per-session stream cipher seeded from CToken seed exchange.
#ifndef FIESTA_PACKETENCRYPT_H
#define FIESTA_PACKETENCRYPT_H
#include "ShineTypes.h"

namespace fiesta {

class PacketEncrypt {
public:
    PacketEncrypt();
    void Init(uint32 uiSeed);-- per-session 32-bit seed from CToken
    void Encrypt(uint8* pBuf, size_t uiLen);
    void Decrypt(uint8* pBuf, size_t uiLen);  // symmetric stream
private:
    uint32 m_auiKeySched[256];
    uint32 m_uiCounter;
    void   Schedule(uint32 uiSeed);
};

} // namespace fiesta
#endif
