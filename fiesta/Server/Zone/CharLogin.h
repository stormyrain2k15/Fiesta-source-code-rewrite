// Server/Zone/CharLogin.h
// 06 -- CharLogin / CharMapMarking / BriefInfoExchange.
// EVIDENCE: PDB_CONFIRMED  symbol: CharLogin, CharMapMarking, BriefInfoExchange
#ifndef FIESTA_ZONE_CHARLOGIN_H
#define FIESTA_ZONE_CHARLOGIN_H
#include "ZoneServer.h"

namespace fiesta {

void CharLogin           (ClientSession* pkSess, const GPacket& rPkt);
void CharLogout          (ClientSession* pkSess, const GPacket& rPkt);
void CharMapMarking      (ClientSession* pkSess, const GPacket& rPkt);
void BriefInfoExchange   (ClientSession* pkSess, const GPacket& rPkt);

} // namespace fiesta
#endif
