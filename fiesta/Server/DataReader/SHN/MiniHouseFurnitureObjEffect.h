// Server/DataReader/SHN/MiniHouseFurnitureObjEffect.h
// Auto-generated: one-file-per-SHN split for MiniHouseFurnitureObjEffect.shn
#ifndef SHINE_DATAREADER_SHN_MINIHOUSEFURNITUREOBJEFFECT_H
#define SHINE_DATAREADER_SHN_MINIHOUSEFURNITUREOBJEFFECT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct MiniHouseFurnitureObjEffectRow {
    uint16           uiHandle;
    std::string      kItemID;
    uint32           uiEffectEnum;
    std::string      kEffectIndex;
    int32            iApplyRange;
    int32            iUnkCol5;
    int32            iUnkCol6;
    int32            iUnkCol7;
    int32            iUnkCol8;
    int32            iUseRange;
    int32            iUnkCol10;
    int32            iUnkCol11;
    int32            iUnkCol12;
    int32            iUnkCol13;
    std::string      kNeedItem;
    uint32           uiNeedMoney;
    std::string      kEffectName;
    std::string      kEffectSound;
};

class MiniHouseFurnitureObjEffectShn {
public:
    static MiniHouseFurnitureObjEffectShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<MiniHouseFurnitureObjEffectRow>& Rows() const { return m_kRows; }
private:
    std::vector<MiniHouseFurnitureObjEffectRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_MINIHOUSEFURNITUREOBJEFFECT_H
