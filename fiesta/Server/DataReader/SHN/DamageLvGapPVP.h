// Server/DataReader/SHN/DamageLvGapPVP.h
// Auto-generated: one-file-per-SHN split for DamageLvGapPVP.shn
#ifndef FIESTA_DATAREADER_SHN_DAMAGELVGAPPVP_H
#define FIESTA_DATAREADER_SHN_DAMAGELVGAPPVP_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct DamageLvGapPVPRow {
    uint8            uiMyLv;
    uint16           uiTargetLv1;
    uint16           uiTargetLv2;
    uint16           uiTargetLv3;
    uint16           uiTargetLv4;
    uint16           uiTargetLv5;
    uint16           uiTargetLv6;
    uint16           uiTargetLv7;
    uint16           uiTargetLv8;
    uint16           uiTargetLv9;
    uint16           uiTargetLv10;
    uint16           uiTargetLv11;
    uint16           uiTargetLv12;
    uint16           uiTargetLv13;
    uint16           uiTargetLv14;
    uint16           uiTargetLv15;
    uint16           uiTargetLv16;
    uint16           uiTargetLv17;
    uint16           uiTargetLv18;
    uint16           uiTargetLv19;
    uint16           uiTargetLv20;
    uint16           uiTargetLv21;
    uint16           uiTargetLv22;
    uint16           uiTargetLv23;
    uint16           uiTargetLv24;
    uint16           uiTargetLv25;
    uint16           uiTargetLv26;
    uint16           uiTargetLv27;
    uint16           uiTargetLv28;
    uint16           uiTargetLv29;
    uint16           uiTargetLv30;
    uint16           uiTargetLv31;
    uint16           uiTargetLv32;
    uint16           uiTargetLv33;
    uint16           uiTargetLv34;
    uint16           uiTargetLv35;
    uint16           uiTargetLv36;
    uint16           uiTargetLv37;
    uint16           uiTargetLv38;
    uint16           uiTargetLv39;
    uint16           uiTargetLv40;
    uint16           uiTargetLv41;
    uint16           uiTargetLv42;
    uint16           uiTargetLv43;
    uint16           uiTargetLv44;
    uint16           uiTargetLv45;
    uint16           uiTargetLv46;
    uint16           uiTargetLv47;
    uint16           uiTargetLv48;
    uint16           uiTargetLv49;
    uint16           uiTargetLv50;
    uint16           uiTargetLv51;
    uint16           uiTargetLv52;
    uint16           uiTargetLv53;
    uint16           uiTargetLv54;
    uint16           uiTargetLv55;
    uint16           uiTargetLv56;
    uint16           uiTargetLv57;
    uint16           uiTargetLv58;
    uint16           uiTargetLv59;
    uint16           uiTargetLv60;
    uint16           uiTargetLv61;
    uint16           uiTargetLv62;
    uint16           uiTargetLv63;
    uint16           uiTargetLv64;
    uint16           uiTargetLv65;
    uint16           uiTargetLv66;
    uint16           uiTargetLv67;
    uint16           uiTargetLv68;
    uint16           uiTargetLv69;
    uint16           uiTargetLv70;
    uint16           uiTargetLv71;
    uint16           uiTargetLv72;
    uint16           uiTargetLv73;
    uint16           uiTargetLv74;
    uint16           uiTargetLv75;
    uint16           uiTargetLv76;
    uint16           uiTargetLv77;
    uint16           uiTargetLv78;
    uint16           uiTargetLv79;
    uint16           uiTargetLv80;
    uint16           uiTargetLv81;
    uint16           uiTargetLv82;
    uint16           uiTargetLv83;
    uint16           uiTargetLv84;
    uint16           uiTargetLv85;
    uint16           uiTargetLv86;
    uint16           uiTargetLv87;
    uint16           uiTargetLv88;
    uint16           uiTargetLv89;
    uint16           uiTargetLv90;
    uint16           uiTargetLv91;
    uint16           uiTargetLv92;
    uint16           uiTargetLv93;
    uint16           uiTargetLv94;
    uint16           uiTargetLv95;
    uint16           uiTargetLv96;
    uint16           uiTargetLv97;
    uint16           uiTargetLv98;
    uint16           uiTargetLv99;
    uint16           uiTargetLv100;
    uint16           uiTargetLv101;
    uint16           uiTargetLv102;
    uint16           uiTargetLv103;
    uint16           uiTargetLv104;
    uint16           uiTargetLv105;
    uint16           uiTargetLv106;
    uint16           uiTargetLv107;
    uint16           uiTargetLv108;
    uint16           uiTargetLv109;
    uint16           uiTargetLv110;
    uint16           uiTargetLv111;
    uint16           uiTargetLv112;
    uint16           uiTargetLv113;
    uint16           uiTargetLv114;
    uint16           uiTargetLv115;
    uint16           uiTargetLv116;
    uint16           uiTargetLv117;
    uint16           uiTargetLv118;
    uint16           uiTargetLv119;
    uint16           uiTargetLv120;
    uint16           uiTargetLv121;
    uint16           uiTargetLv122;
    uint16           uiTargetLv123;
    uint16           uiTargetLv124;
    uint16           uiTargetLv125;
    uint16           uiTargetLv126;
    uint16           uiTargetLv127;
    uint16           uiTargetLv128;
    uint16           uiTargetLv129;
    uint16           uiTargetLv130;
    uint16           uiTargetLv131;
    uint16           uiTargetLv132;
    uint16           uiTargetLv133;
    uint16           uiTargetLv134;
    uint16           uiTargetLv135;
    uint16           uiTargetLv136;
    uint16           uiTargetLv137;
    uint16           uiTargetLv138;
    uint16           uiTargetLv139;
    uint16           uiTargetLv140;
    uint16           uiTargetLv141;
    uint16           uiTargetLv142;
    uint16           uiTargetLv143;
    uint16           uiTargetLv144;
    uint16           uiTargetLv145;
    uint16           uiTargetLv146;
    uint16           uiTargetLv147;
    uint16           uiTargetLv148;
    uint16           uiTargetLv149;
    uint16           uiTargetLv150;
};

class DamageLvGapPVPShn {
public:
    static DamageLvGapPVPShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<DamageLvGapPVPRow>& Rows() const { return m_kRows; }
private:
    std::vector<DamageLvGapPVPRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_DAMAGELVGAPPVP_H
