// Server/DataReader/SHN/ItemDismantle.h
// Auto-generated: one-file-per-SHN split for ItemDismantle.shn
#ifndef FIESTA_DATAREADER_SHN_ITEMDISMANTLE_H
#define FIESTA_DATAREADER_SHN_ITEMDISMANTLE_H
#include "../../Shared/ShineTypes.h"
#include "../../DataReader/ShnRegistry.h"
#include <map>
#include <string>
#include <vector>

namespace fiesta {

struct ItemDismantleRow {
    uint8            uiID;
    uint8            uiGrade;
    int32            iArmor;
    int32            iUnkCol3;
    int32            iUnkCol4;
    int32            iUnkCol5;
    int32            iUnkCol6;
    int32            iBoot;
    int32            iUnkCol8;
    int32            iUnkCol9;
    int32            iUnkCol10;
    int32            iUnkCol11;
    int32            iShield;
    int32            iUnkCol13;
    int32            iUnkCol14;
    int32            iUnkCol15;
    int32            iUnkCol16;
    int32            iWeapon;
    int32            iUnkCol18;
    int32            iUnkCol19;
    int32            iUnkCol20;
    int32            iUnkCol21;
    int32            iAmulet;
    int32            iUnkCol23;
    int32            iUnkCol24;
    int32            iUnkCol25;
    int32            iUnkCol26;
};

class ItemDismantleShn {
public:
    static ItemDismantleShn& Get();
    void Load();
    const ItemDismantleRow* FindById(uint32 uiID) const;
    uint32 Count() const { return (uint32)m_kRows.size(); }
    const std::vector<ItemDismantleRow>& Rows() const { return m_kRows; }
private:
    std::vector<ItemDismantleRow>         m_kRows;
    std::map<uint32,      size_t>  m_kById;
};

} // namespace fiesta
#endif // FIESTA_DATAREADER_SHN_ITEMDISMANTLE_H
