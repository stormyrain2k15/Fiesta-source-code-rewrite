// Server/Login/ClientVersionKeyInfo.h
// single-token version key file used by Login to validate the client
// build during NC_USER_VERSION_REQ. File format is a 14-character date+revision
// token followed by newline (e.g., "10022024000000").
// EDITABLE TUNABLES (change without touching business logic):
//   * kClientID            -- Gamigo regional client id (US / EU / etc.).
//                             32-bit value sent with NC_USER_LOGIN_REQ.
//   * kClientBuildToken    -- 14-char date+rev token Login compares against.
//   * kMapLoginShaToken    -- 326-char SHA1 of the client PE first 4096 bytes,
//                             cached because clients always send the same
//                             value per build.
#ifndef FIESTA_LOGIN_CLIENTVERSIONKEYINFO_H
#define FIESTA_LOGIN_CLIENTVERSIONKEYINFO_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

// =============================================================================
//  EDITABLE BUILD CONSTANTS
//  Set these to match the client you're targeting. They live here so the
//  rest of the server tree never needs to change when you bump the client
//  build or switch regions.
// =============================================================================

// Numeric region/client id sent in NC_USER_LOGIN_REQ. Set once per region.
// (Live game ships several variants; the value here is just a placeholder.)
const uint32 kClientID            = 0x00000000;

// 14-character build token Login validates.
const char* const kClientBuildToken = "10022024000000";

// Static "client PE SHA1" sent inside NC_MAP_LOGIN_REQ. Captured once per
// client build and reused for the whole session.
const int   kMapLoginShaLen       = 326;
const char* const kMapLoginShaToken =
    "82ff8a3ac5bcbc3ddf944bbc085e017d2329a707fd18d89a4dcc1b40b1b9150247cb62ddaf8656737b4ef5"
    "86a04ca6d40994dad6f41c1ced5865d4ba905a01fb91dc5d8cbc1f97495aae0eaa8ba82245073b53a9b86"
    "d6b628242fa237ad636e11046be5cd5eb1c8fd6b774417b935cb68bab0cfbe71c71ebb89862bb10acecda"
    "9940006b5a8996ddee4087b60acde3ff1e347edf8404659a96942bd0b52b19ed4bed62";

// Length of the encrypted handshake token between Zone and WM (first packet
// after Zone connects). Editing this requires matching the client.
const int   kWmHandshakeTokenLen  = 80;

// =============================================================================
class ClientVersionKeyInfo {
public:
    static ClientVersionKeyInfo& Get();
    bool   Load(const char* szPath);
    const std::string& Key() const { return m_kKey; }
    bool   IsAcceptable(const std::string& rClientKey) const;

    // Convenience accessors for the editable build constants.
    static uint32      ClientID()           { return kClientID; }
    static const char* BuildToken()         { return kClientBuildToken; }
    static const char* MapLoginShaToken()   { return kMapLoginShaToken; }
    static int         MapLoginShaLen()     { return kMapLoginShaLen; }

private:
    ClientVersionKeyInfo() {}
    std::string m_kKey;
};

} // namespace fiesta
#endif
