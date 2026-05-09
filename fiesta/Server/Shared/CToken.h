// Server/Shared/CToken.h
// Login -> WM transfer token. 16-byte secret + tagged user/account/world.
#ifndef FIESTA_CTOKEN_H
#define FIESTA_CTOKEN_H
#include "ShineTypes.h"

namespace fiesta {

class CToken {
public:
    CToken();
    void   Reset();
    void   Generate(AccountID aid, uint32 uiWorldId);  // fills random secret
    bool   Verify  (AccountID aid, uint32 uiWorldId, const uint8 aSecret[16]) const;

    AccountID GetAccount()   const { return m_uiAccount; }
    uint32    GetWorldId()   const { return m_uiWorldId; }
    const uint8* GetSecret() const { return m_aSecret; }
    uint64    GetIssuedMs()  const { return m_uiIssuedMs; }
private:
    AccountID m_uiAccount;
    uint32    m_uiWorldId;
    uint8     m_aSecret[16];
    uint64    m_uiIssuedMs;
};

} // namespace fiesta
#endif
