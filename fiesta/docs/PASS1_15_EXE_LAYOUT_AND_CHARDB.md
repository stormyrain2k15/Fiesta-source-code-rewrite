# Pass 1.15 -- Production exe naming, GamigoZR removal, CharDB plumbing (2026-02)

User-context drops:
- 3 file-listing screenshots showing the real shipping exe names.
- Confirmation that GamigoZR (`ZR`) was a security-check workaround, not a real
  game function.

## Removed

- `Server/GamigoZR/` (directory + Main.cpp + every reference in
  README.md / BUILD.md). The "ZR" launcher only existed to bypass an old
  client-version check inside the Zone server. With that gate now disabled
  (Pass 1.14 banner: "MUST CONFIGURE TO ENABLE"), the workaround is dead
  weight.

## Production exe naming (recorded in `docs/BUILD.md`)

The shipping NA2016 / EU drop renames each Win32 binary at link time.
Numeric prefix = boot order. Spaces / underscores are preserved verbatim
so symbol-server lookups against shipped PDBs continue to match.

| Source dir                       | Output exe (Release/x86)        |
|----------------------------------|---------------------------------|
| `Server/Login/`                  | `3LoginServer2.exe`             |
| `Server/WorldManager/`           | `4WorldManagerServer2.exe`      |
| `Server/Zone/`                   | `5ZoneServer2.exe`              |
| `Server/DataServer/Account/`     | `Account Release.exe`           |
| `Server/DataServer/AccountLog/`  | `AccountLog Release.exe`        |
| `Server/DataServer/Character/`   | `Character Release.exe`         |
| `Server/DataServer/GameLog/`     | `GameLog_Release.exe`           |
| `Server/OperatorTool/`           | `OperatorTool Release.exe`      |

The standalone `Account.exe` (309 KB, Mar 2 build) seen in the screenshots
is the operator-side admin tool; its in-game flows live in
`Server/OperatorTool/`.

## Code changes

### `Server/Zone/ShineObject.{h,cpp}`
- `ShinePlayer` now owns the full identity / progression set:
  `m_uiAcctID`, `m_uiCharID`, `m_kName`, `m_uiLevel`, `m_uiClass`,
  `m_iAdminLevel`, `m_uiExp`, `m_uiFame`, `m_uiMoney`,
  `m_uiSTR/END/INT/DEX/MEN`, `m_uiFreeStat`, `m_uiSkillPoint`.
- Per-player **`CharacterSkill m_kSkills`** and
  **`AbnormalState m_kAbState`** owned by the player; expose via
  `Skills()` / `AbState()` accessors.
- New `LoadFromCharDBRow(const DBRecord&)` maps a `p_Char_Login` projection
  directly into the player. Column ordering documented inline.
- Helper accessors: `AddExp`, `AddMoney` (sat-sub on negative),
  `AddFame` (sat-sub at 0).
- Recomputes `MaxHP` / `MaxSP` from `END * 14` + `INT * 12` per
  `BattleTunables.h`; ParamN.txt ladder takes over once that table is
  authoritatively loaded.

### `Server/Zone/SkillSystem.h` + `Server/Zone/AbState.h`
- Stop including `ShineObject.h` (would cause a circular include now that
  `ShineObject.h` includes both). Forward-declare `class ShineObject;` /
  `class ShinePlayer;` instead and pull in `ShineTypes.h` for primitives.

### `Server/DataServer/Character/Main.cpp`
- `CharDBSession::OnPacket` now actually serves
  `NC_INTER_CHAR_DB_QUERY`. Body shape:
  ```
  uint8 op (1=Login, 2=Logout)   uint32 charId
  ```
- For `op=1` it calls `SQLP_Character::Login(cid, rows)` and replies with
  `NC_INTER_CHAR_DB_RESPONSE`:
  ```
  uint8 op   uint8 ok   uint16 nCols   for each col: <ASCII NUL-terminated string>
  ```
- For `op=2` it calls `SQLP_Character::Logout(cid)` and replies with
  `op + ok` only.

The Zone-side outbound client connector (a non-acceptor IOCP socket) is
the next concrete step: it should connect to `Character.Port` (27602),
issue the query in the existing `CharLogin` handler, and on
`NC_INTER_CHAR_DB_RESPONSE` invoke `ShinePlayer::LoadFromCharDBRow`. With
those three lines the round trip is closed.

### Login version-check gate (carried over from Pass 1.14)
- `LoginClientSession::HandleVersionCheck` keeps the `IsAcceptable` block
  commented out behind a `MUST CONFIGURE TO ENABLE` banner. Default
  posture matches original-game shipping behaviour.

## Total source files
**~191** (one new doc; GamigoZR/Main.cpp gone; ShineObject{.h,.cpp},
SkillSystem.h, AbState.h, Character/Main.cpp rewritten).
