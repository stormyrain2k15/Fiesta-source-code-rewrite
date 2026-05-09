// Server/Common/NETCOMMAND.h
// canonical NC opcode enum.
// Numeric values pinned from FiestaShark live captures + recovered Login.pdb /
// WorldManager.pdb / 5ZoneServer2.pdb symbol set (5087 unique symbols).
// Opcodes marked CONFIRMED are from live captures. Opcodes left at the per-
// family base allocation are name-only -- pin via capture as you see them.
// EDIT-IN-PLACE values:
//   * Family bases below: change if a server build moves a whole subsystem.
//   * `kClientID` / `kClientBuildToken` in Login/ClientVersionKeyInfo.h
//   * Damage formula constants in Server/Zone/BattleTunables.h
#ifndef FIESTA_NETCOMMAND_H
#define FIESTA_NETCOMMAND_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

// ----------------------------------------------------------------------------
// Family bases (provisional).
// ----------------------------------------------------------------------------
enum {
    NC_FAMILY_MISC      = 0x0800,   // 0x080E NC_MISC_GAMETIME_ACK confirmed
    NC_FAMILY_USER      = 0x0C00,   // 0x0C01..0x0C1C confirmed (Login server)
    NC_FAMILY_CHAR      = 0x1000,   // 0x1003 NC_CHAR_LOGIN_ACK confirmed
    NC_FAMILY_MAP       = 0x1000,   // 0x1038..0x10D7 confirmed (Zone)
    NC_FAMILY_ACT       = 0x1100,
    NC_FAMILY_BAT       = 0x1200,
    NC_FAMILY_SKILL     = 0x1300,
    NC_FAMILY_ABSTATE   = 0x1400,
    NC_FAMILY_ITEM      = 0x1500,
    NC_FAMILY_NPC       = 0x1600,
    NC_FAMILY_QUEST     = 0x1700,
    NC_FAMILY_PARTY     = 0x1800,
    NC_FAMILY_GUILD     = 0x1900,
    NC_FAMILY_TRADE     = 0x1A00,
    NC_FAMILY_BOOTH     = 0x1B00,
    NC_FAMILY_AUCTION   = 0x1C00,
    NC_FAMILY_KQ        = 0x1D00,
    NC_FAMILY_MID       = 0x1E00,
    NC_FAMILY_PET       = 0x1F00,
    NC_FAMILY_WM        = 0x2000,   // 0x200F NC_WM_HANDSHAKE_REQ confirmed
    NC_FAMILY_GAMBLE    = 0x2100,
    NC_FAMILY_TITLE     = 0x2200,
    NC_FAMILY_PROD      = 0x2300,
    NC_FAMILY_CHAT      = 0x2400,
    NC_FAMILY_FRIEND    = 0x2500,
    NC_FAMILY_HOLY      = 0x2600,
    NC_FAMILY_PRISON    = 0x2700,
    NC_FAMILY_OPTOOL    = 0x3000,
    NC_FAMILY_INTERSVR  = 0x4000,
    NC_FAMILY_OPTIMP    = 0x7000,   // 0x7032..0x7034 / 0x700D / 0x7C07 confirmed
    NC_FAMILY_TICK      = 0x7C00    // 0x7C07 NC_MAP_WORLDTICK_CMD confirmed
};

// ----------------------------------------------------------------------------
// Opcode set. Values flagged "CONFIRMED" are from live FSB captures.
// All others are names recovered from PDB; pin numeric value from capture.
// ----------------------------------------------------------------------------
enum NETCOMMAND {
    // ---- Login server (port 9010) ---------------------------------------
    NC_USER_VERSION_REQ             = 0x0C01,    // CONFIRMED -> version hello
    // PDB_NAME_ALIAS: original NA2016 symbol is NC_USER_CLIENT_VERSION_CHECK_REQ
    // (verified against CParserClient::fc_NC_USER_CLIENT_VERSION_CHECK_REQ
    // in Login.pdb). Keep both names available so handlers that match
    // PDB nomenclature link cleanly.
    NC_USER_CLIENT_VERSION_CHECK_REQ = NC_USER_VERSION_REQ,
    NC_USER_SEED_ACK                = 0x0C03,    // CONFIRMED <- cipher seed
    NC_USER_XTRAP_REQ               = 0x0C04,    // CONFIRMED (LEGACY -- no-op ACK)
    NC_USER_XTRAP_ACK               = 0x0C05,    // CONFIRMED (LEGACY -- always 0x01)
    NC_USER_LOGIN_REQ               = 0x0C06,    // CONFIRMED -> credentials
    NC_USER_LOGIN_ACK               = 0x0C0A,    // CONFIRMED <- world list
    NC_USER_WORLDSELECT_REQ         = 0x0C0B,    // CONFIRMED -> select world
    NC_USER_WORLDSELECT_ACK         = 0x0C0C,    // CONFIRMED <- WM IP + token
    NC_USER_WORLD_STATUS_REQ        = 0x0C1B,    // CONFIRMED -> poll status
    NC_USER_WORLD_STATUS_ACK        = 0x0C1C,    // CONFIRMED <- world status
    NC_USER_LOGINFAIL_ACK           = NC_FAMILY_USER + 0x02,
    NC_USER_LOGOUT_REQ              = NC_FAMILY_USER + 0x0D,
    NC_USER_LOGOUT_ACK              = NC_FAMILY_USER + 0x0E,
    NC_USER_NORMALLOGOUT_CMD        = NC_FAMILY_USER + 0x0F,
    NC_USER_LOGIN_NETMARBLE_REQ     = NC_FAMILY_USER + 0x10,
    NC_USER_LOGIN_OUTSPARK_REQ      = NC_FAMILY_USER + 0x11,
    NC_USER_LOGIN_WITH_OTP_REQ      = NC_FAMILY_USER + 0x12,
    NC_USER_CHARSELECT_REQ          = NC_FAMILY_USER + 0x13,
    NC_USER_CHARDELETE_REQ          = NC_FAMILY_USER + 0x14,
    NC_USER_CHARCREATE_REQ          = NC_FAMILY_USER + 0x15,

    // ---- WorldManager (port 9110) --------------------------------------
    NC_WM_HANDSHAKE_REQ             = 0x200F,    // CONFIRMED -> 80-byte token
    NC_USER_LOGINWORLD_ACK          = 0x0C14,    // CONFIRMED <- char data block
    NC_MISC_GAMETIME_ACK            = 0x080E,    // CONFIRMED <- HH MM SS
    NC_CHAR_LOGIN_REQ               = NC_FAMILY_CHAR + 0x02,
    NC_CHAR_LOGIN_ACK               = 0x1003,    // CONFIRMED <- Zone IP+port
    NC_CHAR_OPTION_GET_CMD          = NC_FAMILY_CHAR + 0x04,
    NC_CHAR_OPTION_SHORTCUT_CMD     = 0x7032,    // CONFIRMED <- shortcut bindings
    NC_CHAR_OPTION_KEYMAP_CMD       = 0x7033,    // CONFIRMED <- key bindings
    NC_CHAR_OPTION_GAMEOPTION_CMD   = 0x7034,    // CONFIRMED <- game options
    NC_CHAR_LOGOUT_CMD              = NC_FAMILY_CHAR + 0x06,
    NC_CHAR_BRIEFINFO_LOGINCHARACTER_CMD = NC_FAMILY_CHAR + 0x07,
    NC_CHAR_STATUS_CHANGE_CMD       = NC_FAMILY_CHAR + 0x08,
    NC_CHAR_NEW_REQ                 = NC_FAMILY_CHAR + 0x09,
    NC_CHAR_NEW_ACK                 = NC_FAMILY_CHAR + 0x0A,
    NC_CHAR_DEL_REQ                 = NC_FAMILY_CHAR + 0x0B,
    NC_CHAR_DEL_ACK                 = NC_FAMILY_CHAR + 0x0C,
    NC_CHAR_CLIENT_BASE_CMD         = NC_FAMILY_CHAR + 0x0D,
    NC_CHAR_CLIENT_GAME_CMD         = NC_FAMILY_CHAR + 0x0E,
    NC_CHAR_CLIENT_SHAPE_CMD        = NC_FAMILY_CHAR + 0x0F,
    NC_CHAR_CLIENT_ITEM_CMD         = NC_FAMILY_CHAR + 0x10,
    NC_CHAR_CLIENT_PASSIVE_CMD      = NC_FAMILY_CHAR + 0x11,

    // ---- Zone server (port 9120) ---------------------------------------
    NC_MAP_LOGIN_REQ                = 0x1801,    // CONFIRMED -> char slot+name+SHA+SHN hash
    NC_MAP_LOGIN_ACK                = 0x1038,    // CONFIRMED <- char id+stats+map
    NC_MAP_CHARINFO_CMD             = 0x1039,    // CONFIRMED <- char info broadcast
    NC_MAP_INVENTORY_CMD            = 0x103A,    // CONFIRMED <- inventory state
    NC_MAP_EQUIPMENT_CMD            = 0x103B,    // CONFIRMED <- equipment state
    NC_MAP_SKILLBUFF_CMD            = 0x10CE,    // CONFIRMED <- skill/buff state
    NC_MAP_QUEST_CMD                = 0x10D7,    // CONFIRMED <- quest state
    NC_MAP_WORLDTICK_CMD            = 0x7C07,    // CONFIRMED <- zone world tick
    NC_MAP_WORLDPARAMS_CMD          = 0x700D,    // CONFIRMED <- 23 floats + 9 u32
    NC_MAP_ZONELIST_CMD             = 0x7005,    // CONFIRMED <- zone count + IDs
    NC_MAP_LINKSAME_CMD             = NC_FAMILY_MAP + 0x40,
    NC_MAP_LINKDIFF_CMD             = NC_FAMILY_MAP + 0x41,
    NC_MAP_LOGOUT_CMD               = NC_FAMILY_MAP + 0x42,
    NC_MAP_LOGOFF_REQ               = NC_FAMILY_MAP + 0x43,
    NC_MAP_HEARTBEAT_REQ            = NC_FAMILY_MAP + 0x44,
    NC_MAP_HEARTBEAT_ACK            = NC_FAMILY_MAP + 0x45,
    NC_MAP_TOWNPORTAL_REQ           = NC_FAMILY_MAP + 0x46,
    NC_MAP_TOWNPORTAL_ACK           = NC_FAMILY_MAP + 0x47,
    NC_MAP_CHANGETARGET_CMD         = NC_FAMILY_MAP + 0x48,
    NC_MAP_ENTER_CMD                = NC_FAMILY_MAP + 0x49,
    NC_MAP_EXIT_CMD                 = NC_FAMILY_MAP + 0x4A,
    NC_MAP_MOVE_REQ                 = NC_FAMILY_MAP + 0x4B,
    NC_MAP_MOVE_CMD                 = NC_FAMILY_MAP + 0x4C,
    NC_MAP_JUMP_REQ                 = NC_FAMILY_MAP + 0x4D,
    NC_MAP_JUMP_CMD                 = NC_FAMILY_MAP + 0x4E,
    NC_MAP_STOP_MOVE_REQ            = NC_FAMILY_MAP + 0x4F,
    NC_MAP_STOP_MOVE_CMD            = NC_FAMILY_MAP + 0x50,
    NC_MAP_SITDOWN_REQ              = NC_FAMILY_MAP + 0x51,
    NC_MAP_SITDOWN_CMD              = NC_FAMILY_MAP + 0x52,
    // PDB_NAME_ALIAS: NA2016 PDB symbol inventory uses NC_ACT_MOVE*_CMD
    // for the client->server move opcodes; NC_MAP_MOVE_REQ above is the
    // generic numeric slot. These aliases let code reference the
    // PDB-original names without changing the wire numbers. Verified
    // against ShineObjectClass::ShinePlayer::sp_NC_ACT_MOVE{RUN,WALK}_CMD
    // and CParserClient::fc_NC_ACT_MOVE{RUN,WALK}_CMD in the Zone PDB.
    NC_ACT_MOVERUN_CMD              = NC_MAP_MOVE_REQ,
    NC_ACT_MOVEWALK_CMD             = NC_MAP_MOVE_CMD,
    NC_ACT_STOP_REQ                 = NC_MAP_STOP_MOVE_REQ,
    NC_ACT_STOP_CMD                 = NC_MAP_STOP_MOVE_CMD,

    // ---- Action / NPC ---------------------------------------------------
    NC_ACT_CHAT_REQ                 = NC_FAMILY_ACT + 0x05,
    NC_ACT_CHAT_CMD                 = NC_FAMILY_ACT + 0x06,
    NC_ACT_SOMONEUSE_REQ            = NC_FAMILY_ACT + 0x10,
    NC_ACT_NPCCLICK_REQ             = NC_FAMILY_ACT + 0x11,
    NC_ACT_NPCMENU_REQ              = NC_FAMILY_ACT + 0x12,
    NC_ACT_REST_CMD                 = NC_FAMILY_ACT + 0x13,
    NC_ACT_EMOTICON_CMD             = NC_FAMILY_ACT + 0x14,

    // ---- Battle ---------------------------------------------------------
    NC_BAT_TARGETING_CMD            = NC_FAMILY_BAT + 0x01,
    NC_BAT_ATTACK_REQ               = NC_FAMILY_BAT + 0x02,
    NC_BAT_ATTACK_CMD               = NC_FAMILY_BAT + 0x03,
    NC_BAT_DAMAGE_CMD               = NC_FAMILY_BAT + 0x04,
    NC_BAT_DEAD_CMD                 = NC_FAMILY_BAT + 0x05,
    NC_BAT_RESURRECT_REQ            = NC_FAMILY_BAT + 0x06,
    NC_BAT_REGEN_CMD                = NC_FAMILY_BAT + 0x07,
    NC_BAT_LEVELUP_CMD              = NC_FAMILY_BAT + 0x08,
    NC_BAT_GETEXP_CMD               = NC_FAMILY_BAT + 0x09,
    NC_BAT_FREESTAT_DISTRIBUTE_REQ  = NC_FAMILY_BAT + 0x0A,
    NC_BAT_FREESTAT_DISTRIBUTE_ACK  = NC_FAMILY_BAT + 0x0B,
    NC_BAT_FREESTAT_RESET_REQ       = NC_FAMILY_BAT + 0x0C,
    NC_BAT_FREESTAT_RESET_ACK       = NC_FAMILY_BAT + 0x0D,
    // PROVISIONAL_ALIAS: client-side UI tables and earlier handler code
    // reference NC_BAT_NORMALATTACK_CMD. The NA2016 PDB symbol set uses
    // NC_BAT_HIT_REQ as the canonical "normal attack hit" request, but
    // both names are tolerated here so existing code links until a
    // packet capture pins which one the live server emits.
    NC_BAT_NORMALATTACK_CMD         = NC_BAT_ATTACK_REQ,
    NC_BAT_HIT_REQ                  = NC_BAT_ATTACK_REQ,

    // ---- Skill ----------------------------------------------------------
    NC_BAT_SKILL_USE_REQ            = NC_FAMILY_SKILL + 0x01,
    NC_BAT_SKILL_USE_ACK            = NC_FAMILY_SKILL + 0x02,
    NC_BAT_SKILL_HIT_CMD            = NC_FAMILY_SKILL + 0x03,
    NC_BAT_SKILL_LEARN_REQ          = NC_FAMILY_SKILL + 0x04,
    NC_BAT_SKILL_LEARN_ACK          = NC_FAMILY_SKILL + 0x05,
    NC_BAT_SKILL_TOGGLE_CMD         = NC_FAMILY_SKILL + 0x06,
    NC_BAT_SKILL_LIST_CMD           = NC_FAMILY_SKILL + 0x07,
    NC_BAT_SKILL_COOLDOWN_CMD       = NC_FAMILY_SKILL + 0x08,
    NC_SKILL_GET_SKILLUP_REQ        = NC_FAMILY_SKILL + 0x10,
    NC_SKILL_SKILLUP_ACK            = NC_FAMILY_SKILL + 0x11,
    NC_SKILL_ACTIVATE_REQ           = NC_FAMILY_SKILL + 0x12,
    NC_SKILL_DEACTIVATE_REQ         = NC_FAMILY_SKILL + 0x13,

    // ---- AbState --------------------------------------------------------
    NC_BAT_ABSTATESET_CMD           = NC_FAMILY_ABSTATE + 0x01,
    NC_BAT_ABSTATERESET_CMD         = NC_FAMILY_ABSTATE + 0x02,
    NC_BAT_ABSTATEDISPEL_CMD        = NC_FAMILY_ABSTATE + 0x03,

    // ---- Item -----------------------------------------------------------
    NC_ITEM_INVENINFO_CMD           = NC_FAMILY_ITEM + 0x01,
    NC_ITEM_PICKUP_REQ              = NC_FAMILY_ITEM + 0x02,
    NC_ITEM_PICK_ACK                = NC_FAMILY_ITEM + 0x03,
    NC_ITEM_DROP_REQ                = NC_FAMILY_ITEM + 0x04,
    NC_ITEM_RELOC_REQ               = NC_FAMILY_ITEM + 0x05,
    NC_ITEM_RELOC_ACK               = NC_FAMILY_ITEM + 0x06,
    NC_ITEM_USE_REQ                 = NC_FAMILY_ITEM + 0x07,
    NC_ITEM_USE_ACK                 = NC_FAMILY_ITEM + 0x08,
    NC_ITEM_EQUIP_REQ               = NC_FAMILY_ITEM + 0x09,
    NC_ITEM_EQUIP_ACK               = NC_FAMILY_ITEM + 0x0A,
    NC_ITEM_UNEQUIP_REQ             = NC_FAMILY_ITEM + 0x0B,
    NC_ITEM_UNEQUIP_ACK             = NC_FAMILY_ITEM + 0x0C,
    NC_ITEM_UPGRADE_REQ             = NC_FAMILY_ITEM + 0x0D,
    NC_ITEM_UPGRADE_ACK             = NC_FAMILY_ITEM + 0x0E,
    NC_ITEM_DESTROY_REQ             = NC_FAMILY_ITEM + 0x0F,
    NC_ITEM_DESTROY_ACK             = NC_FAMILY_ITEM + 0x10,
    NC_ITEM_DROPONFLOOR_CMD         = NC_FAMILY_ITEM + 0x11,
    NC_ITEM_PICKEDFROMFLOOR_CMD     = NC_FAMILY_ITEM + 0x12,
    NC_ITEM_UPGRADE_OPEN_CMD        = NC_FAMILY_ITEM + 0x13,    // Server -> client: open enchanter UI
    NC_STORE_BUY_REQ                = NC_FAMILY_ITEM + 0x20,
    NC_STORE_SELL_REQ               = NC_FAMILY_ITEM + 0x21,
    NC_STORE_CLOSE_REQ              = NC_FAMILY_ITEM + 0x22,

    // ---- NPC ------------------------------------------------------------
    NC_NPC_MENU_OPEN_CMD            = NC_FAMILY_NPC + 0x01,
    NC_NPC_MENU_PICK_REQ            = NC_FAMILY_NPC + 0x02,
    NC_NPC_MENU_PICK_ACK            = NC_FAMILY_NPC + 0x03,
    NC_NPC_SHOP_OPEN_CMD            = NC_FAMILY_NPC + 0x04,
    NC_NPC_SHOP_BUY_REQ             = NC_FAMILY_NPC + 0x05,
    NC_NPC_SHOP_BUY_ACK             = NC_FAMILY_NPC + 0x06,
    NC_NPC_SHOP_SELL_REQ            = NC_FAMILY_NPC + 0x07,
    NC_NPC_SHOP_SELL_ACK            = NC_FAMILY_NPC + 0x08,
    NC_NPC_SHOP_REPUR_REQ           = NC_FAMILY_NPC + 0x09,

    // ---- Quest ----------------------------------------------------------
    NC_QUEST_START_REQ              = NC_FAMILY_QUEST + 0x01,
    NC_QUEST_START_ACK              = NC_FAMILY_QUEST + 0x02,
    NC_QUEST_END_REQ                = NC_FAMILY_QUEST + 0x03,
    NC_QUEST_END_ACK                = NC_FAMILY_QUEST + 0x04,
    NC_QUEST_GIVEUP_REQ             = NC_FAMILY_QUEST + 0x05,
    NC_QUEST_CLIENT_ACTIVATE_CMD    = NC_FAMILY_QUEST + 0x06,
    NC_QUEST_DO_CMD                 = NC_FAMILY_QUEST + 0x07,
    NC_QUEST_DIARY_CMD              = NC_FAMILY_QUEST + 0x08,
    NC_QUEST_KILLCOUNT_CMD          = NC_FAMILY_QUEST + 0x09,

    // ---- Party ----------------------------------------------------------
    NC_PARTY_CREATE_REQ             = NC_FAMILY_PARTY + 0x01,
    NC_PARTY_CREATE_ACK             = NC_FAMILY_PARTY + 0x02,
    NC_PARTY_JOIN_REQ               = NC_FAMILY_PARTY + 0x03,
    NC_PARTY_JOIN_ACK               = NC_FAMILY_PARTY + 0x04,
    NC_PARTY_LEAVE_REQ              = NC_FAMILY_PARTY + 0x05,
    NC_PARTY_LEAVE_CMD              = NC_FAMILY_PARTY + 0x06,
    NC_PARTY_KICKOUT_REQ            = NC_FAMILY_PARTY + 0x07,
    NC_PARTY_KICKOUT_CMD            = NC_FAMILY_PARTY + 0x08,
    NC_PARTY_BREAK_REQ              = NC_FAMILY_PARTY + 0x09,
    NC_PARTY_BREAK_CMD              = NC_FAMILY_PARTY + 0x0A,
    NC_PARTY_INFO_CMD               = NC_FAMILY_PARTY + 0x0B,
    NC_PARTY_MEMBER_CMD             = NC_FAMILY_PARTY + 0x0C,
    NC_PARTY_BONUS_CMD              = NC_FAMILY_PARTY + 0x0D,

    // ---- Guild ----------------------------------------------------------
    NC_GUILD_CREATE_REQ             = NC_FAMILY_GUILD + 0x01,
    NC_GUILD_CREATE_ACK             = NC_FAMILY_GUILD + 0x02,
    NC_GUILD_JOIN_REQ               = NC_FAMILY_GUILD + 0x03,
    NC_GUILD_LEAVE_REQ              = NC_FAMILY_GUILD + 0x04,
    NC_GUILD_KICK_REQ               = NC_FAMILY_GUILD + 0x05,
    NC_GUILD_INFO_CMD               = NC_FAMILY_GUILD + 0x06,
    NC_GUILD_MEMBER_CMD             = NC_FAMILY_GUILD + 0x07,
    NC_GUILD_STORAGE_OPEN_CMD       = NC_FAMILY_GUILD + 0x08,
    NC_GUILD_STORAGE_PUT_REQ        = NC_FAMILY_GUILD + 0x09,
    NC_GUILD_STORAGE_TAKE_REQ       = NC_FAMILY_GUILD + 0x0A,
    NC_GUILD_TOURNAMENT_CMD         = NC_FAMILY_GUILD + 0x0B,
    NC_GUILD_WAR_DECLARE_REQ        = NC_FAMILY_GUILD + 0x0C,
    NC_GUILD_WAR_START_CMD          = NC_FAMILY_GUILD + 0x0D,

    // ---- Trade ----------------------------------------------------------
    NC_TRADE_REQ                    = NC_FAMILY_TRADE + 0x01,
    NC_TRADE_ACK                    = NC_FAMILY_TRADE + 0x02,
    NC_TRADE_CANCEL_CMD             = NC_FAMILY_TRADE + 0x03,
    NC_TRADE_PUT_CMD                = NC_FAMILY_TRADE + 0x04,
    NC_TRADE_REMOVE_CMD             = NC_FAMILY_TRADE + 0x05,
    NC_TRADE_DECIDE_CMD             = NC_FAMILY_TRADE + 0x06,

    // ---- Booth / Auction ------------------------------------------------
    NC_BOOTH_OPEN_REQ               = NC_FAMILY_BOOTH + 0x01,
    NC_BOOTH_CLOSE_CMD              = NC_FAMILY_BOOTH + 0x02,
    NC_BOOTH_BUY_REQ                = NC_FAMILY_BOOTH + 0x03,
    NC_BOOTH_LIST_CMD               = NC_FAMILY_BOOTH + 0x04,
    NC_AUCTION_LIST_REQ             = NC_FAMILY_AUCTION + 0x01,
    NC_AUCTION_REGIST_REQ           = NC_FAMILY_AUCTION + 0x02,
    NC_AUCTION_BID_REQ              = NC_FAMILY_AUCTION + 0x03,
    NC_AUCTION_BUY_REQ              = NC_FAMILY_AUCTION + 0x04,

    // ---- KQ / Instance --------------------------------------------------
    NC_KQ_QUEUE_REQ                 = NC_FAMILY_KQ + 0x01,
    NC_KQ_LEAVE_REQ                 = NC_FAMILY_KQ + 0x02,
    NC_KQ_VOTE_CMD                  = NC_FAMILY_KQ + 0x03,
    NC_KQ_STATE_CMD                 = NC_FAMILY_KQ + 0x04,
    NC_KQ_ENTER_CMD                 = NC_FAMILY_KQ + 0x05,
    NC_MID_QUEUE_REQ                = NC_FAMILY_MID + 0x01,
    NC_MID_LEAVE_REQ                = NC_FAMILY_MID + 0x02,
    NC_MID_REWARD_CMD               = NC_FAMILY_MID + 0x03,
    NC_MID_TIMER_CMD                = NC_FAMILY_MID + 0x04,

    // ---- Pet / Mount / Misc ---------------------------------------------
    NC_PET_SUMMON_REQ               = NC_FAMILY_PET + 0x01,
    NC_PET_DISMISS_CMD              = NC_FAMILY_PET + 0x02,
    NC_PET_FEED_REQ                 = NC_FAMILY_PET + 0x03,
    NC_MOVER_RIDE_REQ               = NC_FAMILY_PET + 0x04,
    NC_MOVER_UNRIDE_CMD             = NC_FAMILY_PET + 0x05,
    NC_GAMBLE_DICE_REQ              = NC_FAMILY_GAMBLE + 0x01,
    NC_GAMBLE_SLOT_REQ              = NC_FAMILY_GAMBLE + 0x02,
    NC_TITLE_GAIN_CMD               = NC_FAMILY_TITLE + 0x01,
    NC_TITLE_EQUIP_CMD              = NC_FAMILY_TITLE + 0x02,
    NC_PROD_BEGIN_REQ               = NC_FAMILY_PROD + 0x01,
    NC_PROD_RESULT_CMD              = NC_FAMILY_PROD + 0x02,
    NC_GATHER_BEGIN_REQ             = NC_FAMILY_PROD + 0x03,
    NC_DISMANTLE_REQ                = NC_FAMILY_PROD + 0x04,

    // ---- Chat / Friend / Holy / Prison / OpTool -------------------------
    NC_CHAT_NORMAL_REQ              = NC_FAMILY_CHAT + 0x01,
    NC_CHAT_NORMAL_CMD              = NC_FAMILY_CHAT + 0x02,
    NC_CHAT_PARTY_REQ               = NC_FAMILY_CHAT + 0x03,
    NC_CHAT_PARTY_CMD               = NC_FAMILY_CHAT + 0x04,
    NC_CHAT_GUILD_REQ               = NC_FAMILY_CHAT + 0x05,
    NC_CHAT_GUILD_CMD               = NC_FAMILY_CHAT + 0x06,
    NC_CHAT_WHISPER_REQ             = NC_FAMILY_CHAT + 0x07,
    NC_CHAT_WHISPER_CMD             = NC_FAMILY_CHAT + 0x08,
    NC_CHAT_SHOUT_REQ               = NC_FAMILY_CHAT + 0x09,
    NC_CHAT_SHOUT_CMD               = NC_FAMILY_CHAT + 0x0A,
    NC_CHAT_GM_REQ                  = NC_FAMILY_CHAT + 0x0B,
    NC_CHAT_ADMIN_CMD               = NC_FAMILY_CHAT + 0x0C,
    NC_FRIEND_ADD_REQ               = NC_FAMILY_FRIEND + 0x01,
    NC_FRIEND_ADD_ACK               = NC_FAMILY_FRIEND + 0x02,
    NC_FRIEND_DELETE_REQ            = NC_FAMILY_FRIEND + 0x03,
    NC_FRIEND_DEL_REQ               = NC_FAMILY_FRIEND + 0x03,   // alias
    NC_FRIEND_LIST_CMD              = NC_FAMILY_FRIEND + 0x04,
    NC_FRIEND_ONLINE_CMD            = NC_FAMILY_FRIEND + 0x05,
    NC_FRIEND_OFFLINE_CMD           = NC_FAMILY_FRIEND + 0x06,
    NC_HOLY_PROPOSE_REQ             = NC_FAMILY_HOLY + 0x01,
    NC_HOLY_CEREMONY_CMD            = NC_FAMILY_HOLY + 0x02,
    NC_HOLY_SUMMON_REQ              = NC_FAMILY_HOLY + 0x03,
    NC_PRISON_ENTER_CMD             = NC_FAMILY_PRISON + 0x01,
    NC_PRISON_LEAVE_CMD             = NC_FAMILY_PRISON + 0x02,
    NC_OPTOOL_AUTH_REQ              = NC_FAMILY_OPTOOL + 0x01,
    NC_OPTOOL_AUTH_ACK              = NC_FAMILY_OPTOOL + 0x02,
    NC_OPTOOL_CMD                   = NC_FAMILY_OPTOOL + 0x03,
    NC_OPTOOL_BAN_CMD               = NC_FAMILY_OPTOOL + 0x04,
    NC_OPTOOL_KICK_CMD              = NC_FAMILY_OPTOOL + 0x05,
    NC_OPTOOL_JAIL_CMD              = NC_FAMILY_OPTOOL + 0x06,    // -> tPrison row + relocate to jail map
    NC_OPTOOL_UNJAIL_CMD            = NC_FAMILY_OPTOOL + 0x07,
    NC_OPTOOL_SYSMSG_CMD            = NC_FAMILY_OPTOOL + 0x08,    // broadcast system chat to a world / all worlds
    NC_OPTOOL_GIVEITEM_CMD          = NC_FAMILY_OPTOOL + 0x09,    // p_Item_Create on target charNo
    NC_OPTOOL_TAKEITEM_CMD          = NC_FAMILY_OPTOOL + 0x0A,    // p_Item_Delete on item key
    NC_OPTOOL_QUERY_REQ             = NC_FAMILY_OPTOOL + 0x0B,    // generic SELECT against any DB (read-only)
    NC_OPTOOL_QUERY_ACK             = NC_FAMILY_OPTOOL + 0x0C,
    NC_OPTOOL_RESULT_ACK            = NC_FAMILY_OPTOOL + 0x0D,    // generic { uint8 ok, string msg } reply

    // ---- Inter-server (Login<->WM, WM<->Zone, WM<->DB) ------------------
    NC_INTER_HELLO_REQ              = NC_FAMILY_INTERSVR + 0x01,
    NC_INTER_HELLO_ACK              = NC_FAMILY_INTERSVR + 0x02,
    NC_INTER_AUTH_TOKEN_PUSH        = NC_FAMILY_INTERSVR + 0x03,
    NC_INTER_AUTH_TOKEN_RESULT      = NC_FAMILY_INTERSVR + 0x04,
    NC_INTER_ZONE_REGISTER_REQ      = NC_FAMILY_INTERSVR + 0x05,
    NC_INTER_ZONE_HEARTBEAT_CMD     = NC_FAMILY_INTERSVR + 0x06,
    NC_INTER_CHAR_DB_QUERY          = NC_FAMILY_INTERSVR + 0x07,
    NC_INTER_CHAR_DB_RESPONSE       = NC_FAMILY_INTERSVR + 0x08,
    NC_INTER_BROADCAST_CMD          = NC_FAMILY_INTERSVR + 0x09,
    // OPTool fanout: WM -> every zone (or targeted zone). Body shapes mirror
    // the OPTool client requests but with `uint32 charNo` already routed.
    NC_INTER_OPTOOL_BAN_PUSH        = NC_FAMILY_INTERSVR + 0x0A,
    NC_INTER_OPTOOL_KICK_PUSH       = NC_FAMILY_INTERSVR + 0x0B,
    NC_INTER_OPTOOL_JAIL_PUSH       = NC_FAMILY_INTERSVR + 0x0C,
    NC_INTER_OPTOOL_UNJAIL_PUSH     = NC_FAMILY_INTERSVR + 0x0D,
    NC_INTER_OPTOOL_SYSMSG_PUSH     = NC_FAMILY_INTERSVR + 0x0E,
    NC_INTER_OPTOOL_GIVEITEM_PUSH   = NC_FAMILY_INTERSVR + 0x0F,
    NC_INTER_OPTOOL_TAKEITEM_PUSH   = NC_FAMILY_INTERSVR + 0x10,
    // GameLog DB exe (World00_GameLog) -- Zone -> GameLog dispatcher.
    NC_INTER_GAMELOG_QUERY          = NC_FAMILY_INTERSVR + 0x11,
    NC_INTER_GAMELOG_RESPONSE       = NC_FAMILY_INTERSVR + 0x12,
    // AccountLog DB exe (World00_AccountLog) -- Zone/WM -> AccountLog.
    NC_INTER_ACCTLOG_QUERY          = NC_FAMILY_INTERSVR + 0x13,
    NC_INTER_ACCTLOG_RESPONSE       = NC_FAMILY_INTERSVR + 0x14,
    // GM event admin trigger: Zone -> WM. WM rebroadcasts kind=2 to all
    // zones via NC_INTER_BROADCAST_CMD. Body:
    //   uint32 eventNo, uint32 durationSec, uint8 action(1=start/0=stop)
    NC_INTER_GMEVENT_TRIGGER_REQ    = NC_FAMILY_INTERSVR + 0x15
};

} // namespace fiesta
#endif
