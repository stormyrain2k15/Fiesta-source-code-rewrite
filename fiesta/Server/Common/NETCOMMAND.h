// Server/Common/NETCOMMAND.h
// 07 Network -- canonical NC opcode enum. Names mirror the PDB symbol prefixes.
// EVIDENCE: PDB_CONFIRMED for names; EV_VERIFY on the numeric value.
//
// Numeric assignments are local provisional values. Per pack rule 04, no extra
// tuning/lookup file is introduced -- to retag a value, edit it in place.
//
#ifndef FIESTA_NETCOMMAND_H
#define FIESTA_NETCOMMAND_H
#include "../Shared/ShineTypes.h"

namespace fiesta {

// Family base addresses (provisional). Edit when capture proofing is done.
enum {
    NC_FAMILY_MISC      = 0x0000,
    NC_FAMILY_USER      = 0x0F00,
    NC_FAMILY_CHAR      = 0x0E00,
    NC_FAMILY_MAP       = 0x1000,
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
    NC_FAMILY_GAMBLE    = 0x2000,
    NC_FAMILY_ATTEND    = 0x2100,
    NC_FAMILY_TITLE     = 0x2200,
    NC_FAMILY_PROD      = 0x2300,
    NC_FAMILY_CHAT      = 0x2400,
    NC_FAMILY_FRIEND    = 0x2500,
    NC_FAMILY_HOLY      = 0x2600,
    NC_FAMILY_PRISON    = 0x2700,
    NC_FAMILY_OPTOOL    = 0x3000,
    NC_FAMILY_INTERSVR  = 0x4000  // server <-> server
};

// Per-family opcode enum. Values are EV_VERIFY -- pin via runtime capture.
enum NETCOMMAND {
    // 04 Login / Account
    NC_USER_LOGINFAIL_ACK         = NC_FAMILY_USER + 0x01,
    NC_USER_CLIENT_VERSION_CHECK_REQ = NC_FAMILY_USER + 0x02,
    NC_USER_XTRAP_REQ             = NC_FAMILY_USER + 0x03,
    NC_USER_XTRAP_ACK             = NC_FAMILY_USER + 0x04,
    NC_USER_LOGIN_REQ             = NC_FAMILY_USER + 0x05,
    NC_USER_LOGIN_ACK             = NC_FAMILY_USER + 0x06,
    NC_USER_WORLD_STATUS_REQ      = NC_FAMILY_USER + 0x07,
    NC_USER_WORLD_STATUS_ACK      = NC_FAMILY_USER + 0x08,
    NC_USER_WORLD_SELECT_REQ      = NC_FAMILY_USER + 0x09,
    NC_USER_WORLD_SELECT_ACK      = NC_FAMILY_USER + 0x0A,
    NC_USER_LOGOUT_REQ            = NC_FAMILY_USER + 0x0B,
    NC_USER_LOGOUT_ACK            = NC_FAMILY_USER + 0x0C,
    NC_USER_NORMALLOGOUT_CMD      = NC_FAMILY_USER + 0x0D,

    // 06/08 Char list / select / login on Zone
    NC_CHAR_CLIENT_BASE_CMD       = NC_FAMILY_CHAR + 0x01,
    NC_CHAR_LOGIN_REQ             = NC_FAMILY_CHAR + 0x02,
    NC_CHAR_LOGIN_ACK             = NC_FAMILY_CHAR + 0x03,
    NC_CHAR_OPTION_GET_CMD        = NC_FAMILY_CHAR + 0x04,
    NC_CHAR_OPTION_IMPROVE_CMD    = NC_FAMILY_CHAR + 0x05,
    NC_CHAR_LOGOUT_CMD            = NC_FAMILY_CHAR + 0x06,
    NC_CHAR_BRIEFINFO_LOGINCHARACTER_CMD = NC_FAMILY_CHAR + 0x07,
    NC_CHAR_STATUS_CHANGE_CMD     = NC_FAMILY_CHAR + 0x08,
    NC_CHAR_NEW_REQ               = NC_FAMILY_CHAR + 0x09,
    NC_CHAR_NEW_ACK               = NC_FAMILY_CHAR + 0x0A,
    NC_CHAR_DEL_REQ               = NC_FAMILY_CHAR + 0x0B,
    NC_CHAR_DEL_ACK               = NC_FAMILY_CHAR + 0x0C,

    // 10 Map
    NC_MAP_LINKSAME_CMD           = NC_FAMILY_MAP + 0x01,
    NC_MAP_LINKDIFF_CMD           = NC_FAMILY_MAP + 0x02,
    NC_MAP_LOGIN_REQ              = NC_FAMILY_MAP + 0x03,
    NC_MAP_LOGIN_ACK              = NC_FAMILY_MAP + 0x04,
    NC_MAP_LOGOUT_CMD             = NC_FAMILY_MAP + 0x05,
    NC_MAP_TOWNPORTAL_REQ         = NC_FAMILY_MAP + 0x06,
    NC_MAP_TOWNPORTAL_ACK         = NC_FAMILY_MAP + 0x07,

    // 11 Action
    NC_ACT_MOVERUN_CMD            = NC_FAMILY_ACT + 0x01,
    NC_ACT_MOVEWALK_CMD           = NC_FAMILY_ACT + 0x02,
    NC_ACT_STOP_CMD               = NC_FAMILY_ACT + 0x03,
    NC_ACT_NPCCLICK_CMD           = NC_FAMILY_ACT + 0x04,
    NC_ACT_CHAT_REQ               = NC_FAMILY_ACT + 0x05,
    NC_ACT_CHAT_CMD               = NC_FAMILY_ACT + 0x06,
    NC_ACT_JUMP_CMD               = NC_FAMILY_ACT + 0x07,
    NC_ACT_REST_CMD               = NC_FAMILY_ACT + 0x08,
    NC_ACT_EMOTICON_CMD           = NC_FAMILY_ACT + 0x09,

    // 12 Battle
    NC_BAT_TARGETING_CMD          = NC_FAMILY_BAT + 0x01,
    NC_BAT_NORMALATTACK_CMD       = NC_FAMILY_BAT + 0x02,
    NC_BAT_DAMAGE_CMD             = NC_FAMILY_BAT + 0x03,
    NC_BAT_DEAD_CMD               = NC_FAMILY_BAT + 0x04,
    NC_BAT_REGEN_CMD              = NC_FAMILY_BAT + 0x05,
    NC_BAT_LEVELUP_CMD            = NC_FAMILY_BAT + 0x06,
    NC_BAT_GETEXP_CMD             = NC_FAMILY_BAT + 0x07,

    // 13 Skill
    NC_BAT_SKILL_USE_REQ          = NC_FAMILY_SKILL + 0x01,
    NC_BAT_SKILL_USE_ACK          = NC_FAMILY_SKILL + 0x02,
    NC_BAT_SKILL_HIT_CMD          = NC_FAMILY_SKILL + 0x03,
    NC_BAT_SKILL_LEARN_REQ        = NC_FAMILY_SKILL + 0x04,
    NC_BAT_SKILL_LEARN_ACK        = NC_FAMILY_SKILL + 0x05,
    NC_BAT_SKILL_TOGGLE_CMD       = NC_FAMILY_SKILL + 0x06,
    NC_BAT_SKILL_LIST_CMD         = NC_FAMILY_SKILL + 0x07,
    NC_BAT_SKILL_COOLDOWN_CMD     = NC_FAMILY_SKILL + 0x08,

    // 14 AbState
    NC_BAT_ABSTATESET_CMD         = NC_FAMILY_ABSTATE + 0x01,
    NC_BAT_ABSTATERESET_CMD       = NC_FAMILY_ABSTATE + 0x02,
    NC_BAT_ABSTATEDISPEL_CMD      = NC_FAMILY_ABSTATE + 0x03,

    // 15 Item
    NC_ITEM_INVENINFO_CMD         = NC_FAMILY_ITEM + 0x01,
    NC_ITEM_PICK_REQ              = NC_FAMILY_ITEM + 0x02,
    NC_ITEM_PICK_ACK              = NC_FAMILY_ITEM + 0x03,
    NC_ITEM_DROP_REQ              = NC_FAMILY_ITEM + 0x04,
    NC_ITEM_RELOC_REQ             = NC_FAMILY_ITEM + 0x05,
    NC_ITEM_RELOC_ACK             = NC_FAMILY_ITEM + 0x06,
    NC_ITEM_USE_REQ               = NC_FAMILY_ITEM + 0x07,
    NC_ITEM_USE_ACK               = NC_FAMILY_ITEM + 0x08,
    NC_ITEM_EQUIP_REQ             = NC_FAMILY_ITEM + 0x09,
    NC_ITEM_EQUIP_ACK             = NC_FAMILY_ITEM + 0x0A,
    NC_ITEM_UNEQUIP_REQ           = NC_FAMILY_ITEM + 0x0B,
    NC_ITEM_UNEQUIP_ACK           = NC_FAMILY_ITEM + 0x0C,
    NC_ITEM_UPGRADE_REQ           = NC_FAMILY_ITEM + 0x0D,
    NC_ITEM_UPGRADE_ACK           = NC_FAMILY_ITEM + 0x0E,
    NC_ITEM_DESTROY_REQ           = NC_FAMILY_ITEM + 0x0F,
    NC_ITEM_DESTROY_ACK           = NC_FAMILY_ITEM + 0x10,
    NC_ITEM_DROPONFLOOR_CMD       = NC_FAMILY_ITEM + 0x11,
    NC_ITEM_PICKEDFROMFLOOR_CMD   = NC_FAMILY_ITEM + 0x12,

    // 19 NPC
    NC_NPC_MENU_OPEN_CMD          = NC_FAMILY_NPC + 0x01,
    NC_NPC_MENU_PICK_REQ          = NC_FAMILY_NPC + 0x02,
    NC_NPC_MENU_PICK_ACK          = NC_FAMILY_NPC + 0x03,
    NC_NPC_SHOP_OPEN_CMD          = NC_FAMILY_NPC + 0x04,
    NC_NPC_SHOP_BUY_REQ           = NC_FAMILY_NPC + 0x05,
    NC_NPC_SHOP_BUY_ACK           = NC_FAMILY_NPC + 0x06,
    NC_NPC_SHOP_SELL_REQ          = NC_FAMILY_NPC + 0x07,
    NC_NPC_SHOP_SELL_ACK          = NC_FAMILY_NPC + 0x08,
    NC_NPC_SHOP_REPUR_REQ         = NC_FAMILY_NPC + 0x09,

    // 20 Quest
    NC_QUEST_BEGIN_REQ            = NC_FAMILY_QUEST + 0x01,
    NC_QUEST_BEGIN_ACK            = NC_FAMILY_QUEST + 0x02,
    NC_QUEST_END_REQ              = NC_FAMILY_QUEST + 0x03,
    NC_QUEST_END_ACK              = NC_FAMILY_QUEST + 0x04,
    NC_QUEST_GIVEUP_REQ           = NC_FAMILY_QUEST + 0x05,
    NC_QUEST_DIARY_CMD            = NC_FAMILY_QUEST + 0x06,
    NC_QUEST_KILLCOUNT_CMD        = NC_FAMILY_QUEST + 0x07,

    // 24 Party
    NC_PARTY_REQ                  = NC_FAMILY_PARTY + 0x01,
    NC_PARTY_ACK                  = NC_FAMILY_PARTY + 0x02,
    NC_PARTY_KICKOUT_REQ          = NC_FAMILY_PARTY + 0x03,
    NC_PARTY_LEAVE_REQ            = NC_FAMILY_PARTY + 0x04,
    NC_PARTY_BREAK_CMD            = NC_FAMILY_PARTY + 0x05,
    NC_PARTY_BONUS_CMD            = NC_FAMILY_PARTY + 0x06,

    // 25 Guild
    NC_GUILD_CREATE_REQ           = NC_FAMILY_GUILD + 0x01,
    NC_GUILD_CREATE_ACK           = NC_FAMILY_GUILD + 0x02,
    NC_GUILD_JOIN_REQ             = NC_FAMILY_GUILD + 0x03,
    NC_GUILD_LEAVE_REQ            = NC_FAMILY_GUILD + 0x04,
    NC_GUILD_KICK_REQ             = NC_FAMILY_GUILD + 0x05,
    NC_GUILD_STORAGE_OPEN_CMD     = NC_FAMILY_GUILD + 0x06,
    NC_GUILD_STORAGE_PUT_REQ      = NC_FAMILY_GUILD + 0x07,
    NC_GUILD_STORAGE_TAKE_REQ     = NC_FAMILY_GUILD + 0x08,
    NC_GUILD_TOURNAMENT_CMD       = NC_FAMILY_GUILD + 0x09,
    NC_GUILD_WAR_DECLARE_REQ      = NC_FAMILY_GUILD + 0x0A,

    // 30 Trade
    NC_TRADE_REQ                  = NC_FAMILY_TRADE + 0x01,
    NC_TRADE_ACK                  = NC_FAMILY_TRADE + 0x02,
    NC_TRADE_CANCEL_CMD           = NC_FAMILY_TRADE + 0x03,
    NC_TRADE_PUT_CMD              = NC_FAMILY_TRADE + 0x04,
    NC_TRADE_REMOVE_CMD           = NC_FAMILY_TRADE + 0x05,
    NC_TRADE_DECIDE_CMD           = NC_FAMILY_TRADE + 0x06,

    // 30 Booth
    NC_BOOTH_OPEN_REQ             = NC_FAMILY_BOOTH + 0x01,
    NC_BOOTH_CLOSE_CMD            = NC_FAMILY_BOOTH + 0x02,
    NC_BOOTH_BUY_REQ              = NC_FAMILY_BOOTH + 0x03,
    NC_BOOTH_LIST_CMD             = NC_FAMILY_BOOTH + 0x04,

    // 29 Auction
    NC_AUCTION_LIST_REQ           = NC_FAMILY_AUCTION + 0x01,
    NC_AUCTION_REGIST_REQ         = NC_FAMILY_AUCTION + 0x02,
    NC_AUCTION_BID_REQ            = NC_FAMILY_AUCTION + 0x03,
    NC_AUCTION_BUY_REQ            = NC_FAMILY_AUCTION + 0x04,

    // 23 KQ
    NC_KQ_QUEUE_REQ               = NC_FAMILY_KQ + 0x01,
    NC_KQ_LEAVE_REQ               = NC_FAMILY_KQ + 0x02,
    NC_KQ_VOTE_CMD                = NC_FAMILY_KQ + 0x03,
    NC_KQ_STATE_CMD               = NC_FAMILY_KQ + 0x04,
    NC_KQ_ENTER_CMD               = NC_FAMILY_KQ + 0x05,

    // 22 Instance
    NC_MID_QUEUE_REQ              = NC_FAMILY_MID + 0x01,
    NC_MID_LEAVE_REQ              = NC_FAMILY_MID + 0x02,
    NC_MID_REWARD_CMD             = NC_FAMILY_MID + 0x03,
    NC_MID_TIMER_CMD              = NC_FAMILY_MID + 0x04,

    // 33/34 Pet / Mount
    NC_PET_SUMMON_REQ             = NC_FAMILY_PET + 0x01,
    NC_PET_DISMISS_CMD            = NC_FAMILY_PET + 0x02,
    NC_PET_FEED_REQ               = NC_FAMILY_PET + 0x03,
    NC_MOVER_RIDE_REQ             = NC_FAMILY_PET + 0x04,
    NC_MOVER_UNRIDE_CMD           = NC_FAMILY_PET + 0x05,

    // 35 Gamble
    NC_GAMBLE_DICE_REQ            = NC_FAMILY_GAMBLE + 0x01,
    NC_GAMBLE_SLOT_REQ            = NC_FAMILY_GAMBLE + 0x02,

    // 37 Attendance
    NC_ATTEND_GET_CMD             = NC_FAMILY_ATTEND + 0x01,
    NC_ATTEND_REWARD_CMD          = NC_FAMILY_ATTEND + 0x02,

    // 36 Title
    NC_TITLE_GAIN_CMD             = NC_FAMILY_TITLE + 0x01,
    NC_TITLE_EQUIP_CMD            = NC_FAMILY_TITLE + 0x02,

    // 32 Production
    NC_PROD_BEGIN_REQ             = NC_FAMILY_PROD + 0x01,
    NC_PROD_RESULT_CMD            = NC_FAMILY_PROD + 0x02,
    NC_GATHER_BEGIN_REQ           = NC_FAMILY_PROD + 0x03,
    NC_DISMANTLE_REQ              = NC_FAMILY_PROD + 0x04,

    // 28 Chat
    NC_CHAT_NORMAL_CMD            = NC_FAMILY_CHAT + 0x01,
    NC_CHAT_PARTY_CMD             = NC_FAMILY_CHAT + 0x02,
    NC_CHAT_GUILD_CMD             = NC_FAMILY_CHAT + 0x03,
    NC_CHAT_WHISPER_CMD           = NC_FAMILY_CHAT + 0x04,
    NC_CHAT_SHOUT_CMD             = NC_FAMILY_CHAT + 0x05,
    NC_CHAT_ADMIN_CMD             = NC_FAMILY_CHAT + 0x06,

    // 27 Friend
    NC_FRIEND_ADD_REQ             = NC_FAMILY_FRIEND + 0x01,
    NC_FRIEND_DEL_REQ             = NC_FAMILY_FRIEND + 0x02,
    NC_FRIEND_LIST_CMD            = NC_FAMILY_FRIEND + 0x03,

    // 26 HolyPromise / Wedding
    NC_HOLY_PROPOSE_REQ           = NC_FAMILY_HOLY + 0x01,
    NC_HOLY_CEREMONY_CMD          = NC_FAMILY_HOLY + 0x02,
    NC_HOLY_SUMMON_REQ            = NC_FAMILY_HOLY + 0x03,

    // 39 Prison
    NC_PRISON_ENTER_CMD           = NC_FAMILY_PRISON + 0x01,
    NC_PRISON_LEAVE_CMD           = NC_FAMILY_PRISON + 0x02,

    // 39 OpTool
    NC_OPTOOL_AUTH_REQ            = NC_FAMILY_OPTOOL + 0x01,
    NC_OPTOOL_AUTH_ACK            = NC_FAMILY_OPTOOL + 0x02,
    NC_OPTOOL_CMD                 = NC_FAMILY_OPTOOL + 0x03,
    NC_OPTOOL_BAN_CMD             = NC_FAMILY_OPTOOL + 0x04,
    NC_OPTOOL_KICK_CMD            = NC_FAMILY_OPTOOL + 0x05,

    // 05 Inter-server (Login<->WM, WM<->Zone, WM<->DB)
    NC_INTER_HELLO_REQ            = NC_FAMILY_INTERSVR + 0x01,
    NC_INTER_HELLO_ACK            = NC_FAMILY_INTERSVR + 0x02,
    NC_INTER_AUTH_TOKEN_PUSH      = NC_FAMILY_INTERSVR + 0x03,
    NC_INTER_AUTH_TOKEN_RESULT    = NC_FAMILY_INTERSVR + 0x04,
    NC_INTER_ZONE_REGISTER_REQ    = NC_FAMILY_INTERSVR + 0x05,
    NC_INTER_ZONE_HEARTBEAT_CMD   = NC_FAMILY_INTERSVR + 0x06,
    NC_INTER_CHAR_DB_QUERY        = NC_FAMILY_INTERSVR + 0x07,
    NC_INTER_CHAR_DB_RESPONSE     = NC_FAMILY_INTERSVR + 0x08,
    NC_INTER_BROADCAST_CMD        = NC_FAMILY_INTERSVR + 0x09
};

} // namespace fiesta
#endif
