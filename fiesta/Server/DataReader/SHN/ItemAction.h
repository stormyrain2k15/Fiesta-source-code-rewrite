// Server/DataReader/SHN/ItemAction.h
// Auto-generated: one-file-per-SHN split for ItemAction.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMACTION_H
#define FIESTA_DATAREADER_SHN_ITEMACTION_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemActionRow {
    uint16           uiItemActionID;
    uint16           uiConditionID;
    uint16           uiEffectID;
    uint32           uiCoolTime;
    uint16           uiCoolGroupActionID;
};

class ItemActionShn {
public:
    static ItemActionShn& Get();
    void Load();
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemActionRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemActionRow>         m_kRows;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMACTION_H
