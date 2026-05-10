// Server/DataReader/SHN/UseClassTypeInfo.h
// Auto-generated: one-file-per-SHN split for UseClassTypeInfo.shn
#ifndef FIESTA_DATAREADER_SHN_USECLASSTYPEINFO_H
#define FIESTA_DATAREADER_SHN_USECLASSTYPEINFO_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct UseClassTypeInfoRow {
    uint32           uiUseClass;
    uint8            uiFig;
    uint8            uiCfig;
    uint8            uiWar;
    uint8            uiGla;
    uint8            uiKni;
    uint8            uiCle;
    uint8            uiHcle;
    uint8            uiPal;
    uint8            uiHol;
    uint8            uiGua;
    uint8            uiArc;
    uint8            uiHarc;
    uint8            uiSco;
    uint8            uiSha;
    uint8            uiRan;
    uint8            uiMag;
    uint8            uiWmag;
    uint8            uiEnc;
    uint8            uiWarl;
    uint8            uiWiz;
    uint8            uiJok;
    uint8            uiChs;
    uint8            uiCru;
    uint8            uiCls;
    uint8            uiAss;
    uint8            uiSen;
    uint8            uiSav;
};

class UseClassTypeInfoShn {
public:
    static UseClassTypeInfoShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<UseClassTypeInfoRow>& Rows() const { return m_kRows; }
private:
    std::vector<UseClassTypeInfoRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_USECLASSTYPEINFO_H
