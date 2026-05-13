// Server/Zone/NPCRole.h
// canonical NPC role enum + classifier.
// Source-of-truth: World/NPC.txt's `Role` (column 9) and `RoleArg0`
// (column 10). Verified via grep over the supplied NPC.txt -- the unique
// Role values present are:
//   ClientMenu       ; in-town main menu
//   Merchant         ; vendor (RoleArg0 narrows to Weapon/Item/SoulStone/...)
//   NPCMenu          ; non-merchant menu container (Guild, Tournament, etc.)
//   QuestNpc         ; talks-to-start-quest container
//   StoreManager     ; warehouse/bank
//   Gate             ; instanced map portal (always-on)
//   IDGate           ; instance-dungeon portal (cooldown / level gated)
//   ModeIDGate       ; mode-instance portal (KQ/PvP/Holy etc.)
//   RandomGate       ; teleport to one of N exits
//   Guard            ; auto-attack guard mob (RoleArg0 holds aggro target)
// Plus the merchant sub-types observed in RoleArg0:
//   Weapon, WeaponTitle, Item, Skill, Guild, SoulStone, PowerStone,
//   GuardStone, Stone, Mount, Pet, Costume, Title, ItemCraft.
#ifndef SHINE_ZONE_NPCROLE_H
#define SHINE_ZONE_NPCROLE_H
#include "../Shared/ShineTypes.h"
#include <string>

namespace shine {

enum eNpcRole {
    NPC_ROLE_NONE         = 0,
    NPC_ROLE_CLIENT_MENU  = 1,    // ClientMenu
    NPC_ROLE_NPC_MENU     = 2,    // NPCMenu
    NPC_ROLE_QUEST        = 3,    // QuestNpc
    NPC_ROLE_STORE        = 4,    // StoreManager (bank / warehouse)
    NPC_ROLE_MERCHANT     = 5,    // Merchant (RoleArg0 narrows further)
    NPC_ROLE_GATE         = 6,    // Gate
    NPC_ROLE_ID_GATE      = 7,    // IDGate
    NPC_ROLE_MODE_ID_GATE = 8,    // ModeIDGate
    NPC_ROLE_RANDOM_GATE  = 9,    // RandomGate
    NPC_ROLE_GUARD        = 10    // Guard
};

// Merchant sub-roles (RoleArg0).
enum eMerchantKind {
    MK_NONE           = 0,
    MK_WEAPON         = 1,
    MK_WEAPON_TITLE   = 2,
    MK_ITEM           = 3,
    MK_SKILL          = 4,
    MK_GUILD          = 5,
    MK_SOUL_STONE     = 6,
    MK_POWER_STONE    = 7,
    MK_GUARD_STONE    = 8,
    MK_GENERIC_STONE  = 9,        // RoleArg0 == "Stone"
    MK_MOUNT          = 10,
    MK_PET            = 11,
    MK_COSTUME        = 12,
    MK_TITLE          = 13,
    MK_ITEM_CRAFT     = 14
};

class NPCRole {
public:
    static eNpcRole       ParseRole(const std::string& rRole);
    static eMerchantKind  ParseMerchantKind(const std::string& rRoleArg0);
    static const char*    RoleName(eNpcRole e);
    static const char*    MerchantKindName(eMerchantKind e);
};

} // namespace shine
#endif
