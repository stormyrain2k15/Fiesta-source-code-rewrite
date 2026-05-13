// Server/DataReader/SHN/ItemUseEffect.h
// Auto-generated: one-file-per-SHN split for ItemUseEffect.shn
#ifndef SHINE_DATAREADER_SHN_ITEMUSEEFFECT_H
#define SHINE_DATAREADER_SHN_ITEMUSEEFFECT_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

struct ItemUseEffectRow {
    std::string      kItemIndex;
    uint32           uiUseEffectA;
    uint16           uiUseValueA;
    uint32           uiUseEffectB;
    uint16           uiUseValueB;
    uint32           uiUseEffectC;
    uint16           uiUseValueC;
    std::string      kUseAbstateName;
    uint32           uiUseAbstateIndex;
};

class ItemUseEffectShn {
public:
    static ItemUseEffectShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemUseEffectRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemUseEffectRow>         m_kRows;
};

} // namespace shine
#endif // SHINE_DATAREADER_SHN_ITEMUSEEFFECT_H
