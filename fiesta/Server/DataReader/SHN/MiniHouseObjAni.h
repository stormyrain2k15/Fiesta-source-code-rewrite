// Server/DataReader/SHN/MiniHouseObjAni.h
// Auto-generated: one-file-per-SHN split for MiniHouseObjAni.shn
#ifndef SHINE_DATAREADER_SHN_MINIHOUSEOBJANI_H
#define SHINE_DATAREADER_SHN_MINIHOUSEOBJANI_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MiniHouseObjAniRow {
    uint16           uiHandle;
    uint16           uiItemID;
    uint8            uiAniGroupIDMaxNum;
    uint16           uiAniGroupID;
    uint32           uiEventCode;
    uint16           uiNextAniHandle;
    uint8            uiActorMaxNum;
    uint32           uiActor01;
    uint32           uiActor02;
    uint32           uiActor03;
    uint8            uiActeeMaxNum;
    uint32           uiActee01;
    uint32           uiActee02;
    uint32           uiActee03;
};

class MiniHouseObjAniShn {
public:
    static MiniHouseObjAniShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MiniHouseObjAniRow>& Rows() const { return m_kRows; }
private:
    std::vector<MiniHouseObjAniRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MINIHOUSEOBJANI_H
