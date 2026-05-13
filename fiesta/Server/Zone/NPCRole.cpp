// Server/Zone/NPCRole.cpp
#include "NPCRole.h"
#include <string.h>

namespace shine {

eNpcRole NPCRole::ParseRole(const std::string& r) {
    if (_stricmp(r.c_str(), "ClientMenu")   == 0) return NPC_ROLE_CLIENT_MENU;
    if (_stricmp(r.c_str(), "NPCMenu")      == 0) return NPC_ROLE_NPC_MENU;
    if (_stricmp(r.c_str(), "QuestNpc")     == 0) return NPC_ROLE_QUEST;
    if (_stricmp(r.c_str(), "StoreManager") == 0) return NPC_ROLE_STORE;
    if (_stricmp(r.c_str(), "Merchant")     == 0) return NPC_ROLE_MERCHANT;
    if (_stricmp(r.c_str(), "Gate")         == 0) return NPC_ROLE_GATE;
    if (_stricmp(r.c_str(), "IDGate")       == 0) return NPC_ROLE_ID_GATE;
    if (_stricmp(r.c_str(), "ModeIDGate")   == 0) return NPC_ROLE_MODE_ID_GATE;
    if (_stricmp(r.c_str(), "RandomGate")   == 0) return NPC_ROLE_RANDOM_GATE;
    if (_stricmp(r.c_str(), "Guard")        == 0) return NPC_ROLE_GUARD;
    return NPC_ROLE_NONE;
}

eMerchantKind NPCRole::ParseMerchantKind(const std::string& a) {
    if (_stricmp(a.c_str(), "Weapon")      == 0) return MK_WEAPON;
    if (_stricmp(a.c_str(), "WeaponTitle") == 0) return MK_WEAPON_TITLE;
    if (_stricmp(a.c_str(), "Item")        == 0) return MK_ITEM;
    if (_stricmp(a.c_str(), "Skill")       == 0) return MK_SKILL;
    if (_stricmp(a.c_str(), "Guild")       == 0) return MK_GUILD;
    if (_stricmp(a.c_str(), "SoulStone")   == 0) return MK_SOUL_STONE;
    if (_stricmp(a.c_str(), "PowerStone")  == 0) return MK_POWER_STONE;
    if (_stricmp(a.c_str(), "GuardStone")  == 0) return MK_GUARD_STONE;
    if (_stricmp(a.c_str(), "Stone")       == 0) return MK_GENERIC_STONE;
    if (_stricmp(a.c_str(), "Mount")       == 0) return MK_MOUNT;
    if (_stricmp(a.c_str(), "Pet")         == 0) return MK_PET;
    if (_stricmp(a.c_str(), "Costume")     == 0) return MK_COSTUME;
    if (_stricmp(a.c_str(), "Title")       == 0) return MK_TITLE;
    if (_stricmp(a.c_str(), "ItemCraft")   == 0) return MK_ITEM_CRAFT;
    return MK_NONE;
}

const char* NPCRole::RoleName(eNpcRole e) {
    switch (e) {
        case NPC_ROLE_CLIENT_MENU:  return "ClientMenu";
        case NPC_ROLE_NPC_MENU:     return "NPCMenu";
        case NPC_ROLE_QUEST:        return "QuestNpc";
        case NPC_ROLE_STORE:        return "StoreManager";
        case NPC_ROLE_MERCHANT:     return "Merchant";
        case NPC_ROLE_GATE:         return "Gate";
        case NPC_ROLE_ID_GATE:      return "IDGate";
        case NPC_ROLE_MODE_ID_GATE: return "ModeIDGate";
        case NPC_ROLE_RANDOM_GATE:  return "RandomGate";
        case NPC_ROLE_GUARD:        return "Guard";
        default:                    return "None";
    }
}
const char* NPCRole::MerchantKindName(eMerchantKind e) {
    switch (e) {
        case MK_WEAPON:        return "Weapon";
        case MK_WEAPON_TITLE:  return "WeaponTitle";
        case MK_ITEM:          return "Item";
        case MK_SKILL:         return "Skill";
        case MK_GUILD:         return "Guild";
        case MK_SOUL_STONE:    return "SoulStone";
        case MK_POWER_STONE:   return "PowerStone";
        case MK_GUARD_STONE:   return "GuardStone";
        case MK_GENERIC_STONE: return "Stone";
        case MK_MOUNT:         return "Mount";
        case MK_PET:           return "Pet";
        case MK_COSTUME:       return "Costume";
        case MK_TITLE:         return "Title";
        case MK_ITEM_CRAFT:    return "ItemCraft";
        default:               return "None";
    }
}

} // namespace shine
