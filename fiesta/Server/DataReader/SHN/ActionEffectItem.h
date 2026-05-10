// Server/DataReader/SHN/ActionEffectItem.h
// Auto-generated: one-file-per-SHN split for ActionEffectItem.shn
#ifndef FIESTA_DATAREADER_SHN_ACTIONEFFECTITEM_H
#define FIESTA_DATAREADER_SHN_ACTIONEFFECTITEM_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ActionEffectItemRow {
    uint16           uiSHINE_ITEM_ID;
    uint16           uiItemActionID;
    uint8            uiEnchantMaxLot;
};

class ActionEffectItemShn {
public:
    static ActionEffectItemShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ActionEffectItemRow>& Rows() const { return m_kRows; }
private:
    std::vector<ActionEffectItemRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ACTIONEFFECTITEM_H
