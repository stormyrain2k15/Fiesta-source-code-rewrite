// Server/Zone/ItemAttributeClass.cpp
// Attribute-class taxonomy -- maps an item's `Class` column to the high
// level attribute family (weapon-blunt, weapon-blade, armor-light,
// armor-heavy, accessory, consumable, material, quest, special).
#include "../Shared/ShineTypes.h"
namespace fiesta {
class ItemAttributeClass {
public:
    enum eFamily { FAM_WEAPON, FAM_ARMOR, FAM_ACCESSORY, FAM_CONSUMABLE, FAM_MATERIAL, FAM_QUEST, FAM_SPECIAL };
    static eFamily FamilyOf(uint32 uiClass) {
        if (uiClass >= 1 && uiClass <= 99)   return FAM_WEAPON;
        if (uiClass >= 100 && uiClass <= 199) return FAM_ARMOR;
        if (uiClass >= 200 && uiClass <= 249) return FAM_ACCESSORY;
        if (uiClass >= 250 && uiClass <= 299) return FAM_CONSUMABLE;
        if (uiClass >= 300 && uiClass <= 349) return FAM_MATERIAL;
        if (uiClass >= 350 && uiClass <= 399) return FAM_QUEST;
        return FAM_SPECIAL;
    }
};
} // namespace fiesta
