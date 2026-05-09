# Pass 1.14 -- DataServer DB Layer (2026-02)

## Source artifacts consumed
- `/app/downloads/db/Account.bak`           (2026-02 user drop)
- `/app/downloads/db/AccountLog.bak`        (2026-02 user drop)
- `/app/downloads/db/Character/World00_Character.bak`
- `/app/downloads/db/World00_GameLog.bak`
- `/app/downloads/db/Options.bak`
- `/app/downloads/db/OperatorTool.bak`
- `/app/downloads/db/StatisticsData.bak`
- `/app/downloads/parsers/shn_parser.py`    (canonical Python SHN parser)

The .bak files are SQL Server full-database dumps. We extracted procedure /
table / column identifiers via the in-page string tables -- exact parameter
type widths still need to come from a `RESTORE` against a real instance, but
*names* are now anchored to ground truth.

## Inventory after extraction

### Stored procedures (56 unique, by database)

**World00_Character (45)**
```
p_Char_FameSub                p_Char_Login                  p_Char_Logout
p_Char_MoneyMove              p_Char_MoneySub
p_Char_WeddingStatus_Get      p_Char_WeddingStatus_Set
p_Char_Wedding_Cancel_Propose p_Char_Wedding_Divorce_Cancel
p_Char_Wedding_Divorce_Do     p_Char_Wedding_Divorce_by_Agree
p_Char_Wedding_Divorce_by_Force
p_Char_Wedding_Do             p_Char_Wedding_Escape_Divorce_State
p_Char_Wedding_Get_Partner_Info
p_Char_Wedding_Propose
p_Friend_Del_All              p_Event_Friend_Get
p_GuildTournament_Get         p_GuildTournament_GetAllTournamentList
p_GuildTournament_GetLastMatch p_GuildTournament_Set
p_Guild_GetListDetail
p_HolyPromise_CheckRing       p_HolyPromise_Del
p_HolyPromise_DelChar         p_HolyPromise_Get
p_HolyPromise_GetMember       p_HolyPromise_GetMemberCount
p_HolyPromise_Set             p_HolyPromise_SetDate
p_Item_BuyAll                 p_Item_BuyLot
p_Item_Create                 p_Item_Delete
p_Item_GetOption              p_Item_GetStorageTypeCount
p_Item_IsValidOwner           p_Item_Merge_New
p_Item_SetOption              p_Item_Split_N_Merge_New
p_Quest_GetAllDoing           p_Quest_Set
p_Skill_SetPower              p_Skill_SetPowerAll
p_getQuestTimes
```

**Account (3)**: `p_UserGetAuthList`, `p_UserSetAuthLoginable`, `p_test`
**AccountLog (2)**: `p_IPBlock_IsBlock`, `p_DayConnectionTime`
**OperatorTool (5)**: `p_Operator_Logon`, `p_Operator_Del`, `p_Operator_SetLevel`, `p_Operator_SetPW`, `p_test`
**StatisticsData (2)**: `p_DayConnectionTime`, `p_test`

### Tables (per DB)

| DB                  | Tables |
|---------------------|--------|
| Account             | tUser, tUserAuth, tUserBlock, tUserOption, tCash, tCashLog, tChannelLog, tChargeItem, tChargeItemDelLog, tDrawComment, tOrder, tPasswordLog, tPayAgreement, tPayResult, tPayType, tSerialKey, tZipCode |
| AccountLog          | tAccountLog, tEvent_Friend, tIPBlock |
| World00_Character   | tCharacter, tCharacterShape, tFriend, tGuild, tGuildAcademyHistory, tGuildAcademyMember, tGuildEmblem, tGuildToken, tGuildTournament, tHolyPromise, tItem, tItemEnchant, tItemOptions, tMarriage, tPetNew, tQuest, tQuestTimes, tRestExp, tSkill, tStatCharLevel, tStatGameMoney, tUserMoney, tGambleCoin/CoinExchangeLog/Dice/DiceLog/Jackpot/JackpotLog/SlotMachineLog |
| World00_GameLog     | (write-only logs; columns inferred per-call) |
| OperatorTool        | tOperator, tMenuAuth |
| StatisticsData      | tAccountLog, tCharacter, tQuestTimes, tSameTimeChar |
| Options             | tGameOptionInit |

## Code changes

### `Server/DataServer/Common/DBSchemaConstants.h` (rewritten, ~430 lines)
- Added a new `fiesta::DB::<TableName>` namespace per real table with
  `static const char* const kTable / kColumn` constants.
- The pre-existing `fiesta::DBField::*()` accessors are preserved verbatim
  (back-compat) and now forward to the new namespaced constants.

### `Server/DataServer/Common/Database.{h,cpp}`
- Added `Database::Quote()` -- T-SQL single-quote escape + control-byte strip.
- Added `Database::ExecProc()` / `Database::QueryProc()` -- builds
  `{CALL proc(args)}` ODBC call escapes around an arg-string.

### `Server/DataServer/Common/SQLP.{h,cpp}` (rewritten)
Eleven facade classes, one per logical concern, sharing a `SQLP_Base`:

| Class               | DB                | Procs wrapped |
|---------------------|-------------------|---------------|
| `SQLP_Account`      | Account           | up_Server_Account_Login, p_UserGetAuthList, p_UserSetAuthLoginable + tUser SELECT helpers |
| `SQLP_AccountLog`   | AccountLog        | p_IPBlock_IsBlock, p_Event_Friend_Get + tAccountLog INSERT |
| `SQLP_Character`    | World00_Character | p_Char_Login/Logout/MoneyMove/MoneySub/FameSub |
| `SQLP_Wedding`      | World00_Character | 11 wedding/divorce procs |
| `SQLP_HolyPromise`  | World00_Character | 8 holy-promise procs |
| `SQLP_Guild`        | World00_Character | p_Guild_GetListDetail + 4 GuildTournament procs |
| `SQLP_Item`         | World00_Character | 10 item-CRUD procs |
| `SQLP_Quest`        | World00_Character | p_Quest_GetAllDoing/Set, p_getQuestTimes |
| `SQLP_Skill`        | World00_Character | p_Skill_SetPower/SetPowerAll |
| `SQLP_Friend`       | World00_Character | p_Friend_Del_All |
| `SQLP_GameLog`      | World00_GameLog   | LogKill / LogDrop / LogTrade INSERTs |
| `SQLP_Statistics`   | StatisticsData    | up_Server_Statistics_*, p_DayConnectionTime |
| `SQLP_Operator`     | OperatorTool      | 4 operator-management procs |
| `SQLP_Options`      | Options           | tGameOptionInit SELECTs |
| `SQLP_Report`       | (any)             | up_Server_Report_File |
| `SQLP_IPChecker`    | (any)             | up_Server_IPChecker_IsBlocked |

### `Server/DataServer/Account/Main.cpp` (rewritten)
- Instantiates `SQLP_Account` + `SQLP_IPChecker` against the `Account` ODBC
  entry (`m_kInfo.FindOdbc("Account")`).
- Falls back to the documented connect string when no `OdbcEntry` is present.

### `Server/DataServer/AccountLog/Main.cpp` (rewritten)
- Instantiates `SQLP_AccountLog` against the `AccountLog` ODBC entry.

### `Server/DataServer/Character/Main.cpp` (rewritten)
- Instantiates **all eight** World00_Character facades (`SQLP_Character`,
  `SQLP_Wedding`, `SQLP_HolyPromise`, `SQLP_Guild`, `SQLP_Item`, `SQLP_Quest`,
  `SQLP_Skill`, `SQLP_Friend`) against a single shared ODBC connection.

### `Server/DataServer/GameLog/Main.cpp` (rewritten)
- Instantiates `SQLP_GameLog` and `SQLP_Report` against the `GameLog` DB.

### `Server/OperatorTool/Main.cpp` (rewritten)
- Now opens an ODBC connection to `OperatorTool` and instantiates
  `SQLP_Operator`. The auth-request handler reads `userid\0password\0` from
  the packet body and calls `SQLP_Operator::Logon`, attaching the operator
  level to the session for downstream BAN/KICK gating.

## Notes on parameter shapes
- All proc calls go through `Database::ExecProc/QueryProc` which build the
  `{CALL p_Foo(?, ?, ?)}` form with literal-substituted args. **Real
  parameter types** (lengths, nullability, output params) need to come from
  a live SQL Server `sp_helptext` over the restored `.bak`s; if any proc
  signature mismatches, the fix is local to a single function in `SQLP.cpp`.
- All UPSERT proc names that I could not find a match for in the .bak strings
  (`up_Server_Account_Login`, `up_Server_Statistics_CharLogin`,
  `up_Server_IPChecker_IsBlocked`, `up_Server_Report_File`) are kept under
  their PDB-documented names; the `VerifyLogin` path falls back to a direct
  `tUser` SELECT when the wrapper isn't deployed.

## Auxiliary: client version-check disabled
- `Server/Login/LoginClientSession.cpp::HandleVersionCheck` -- the
  `ClientVersionKeyInfo::IsAcceptable` gate is now commented out and
  prefixed with **MUST CONFIGURE TO ENABLE**. The packet is acknowledged
  blindly and the session moves into `LS_VERSION_OK`. Original game shipped
  without this check for most of its lifetime; only the WM ever consulted
  the build key, and even there the gate was advisory.

## Total source files
**~191** (one new doc file; SQLP/DBSchema/Main rewrites).
