// Server/Zone/CharLogin.h
// CharLogin / CharMapMarking / BriefInfoExchange.
#ifndef SHINE_ZONE_CHARLOGIN_H
#define SHINE_ZONE_CHARLOGIN_H
#include "ZoneServer.h"

namespace shine {

void CharLogin           (ClientSession* pkSess, const GPacket& rPkt);
void CharLogout          (ClientSession* pkSess, const GPacket& rPkt);
void CharMapMarking      (ClientSession* pkSess, const GPacket& rPkt);
void BriefInfoExchange   (ClientSession* pkSess, const GPacket& rPkt);

} // namespace shine
#endif
