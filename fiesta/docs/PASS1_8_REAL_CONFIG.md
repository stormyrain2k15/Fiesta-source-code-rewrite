# Pass 1.8 — Real config format support

Date: 2026-05-09. User supplied 5 real config files. Replaced the placeholder
`key=value` `ServerInfo` reader with a parser for the actual production format.

## Real format (from project-owner samples)

```
#DEFINE SERVER_INFO
  <STRING>            ; tag
  <INTEGER>           ; service kind: 0=AccDB 1=ALDB 2=CDB 3=GLDB 4=Login 5=WM 6=Zone
  <INTEGER>           ; world id
  <INTEGER>           ; zone id
  <INTEGER>           ; bind class (20=public, 5/6/8=loopback)
  <STRING>            ; ip
  <INTEGER>           ; port
  <INTEGER>           ; max A
  <INTEGER>           ; max B
#ENDDEFINE

#DEFINE ODBC_INFO
  <STRING>  ; db logical name
  <INTEGER> ; db kind id
  <INTEGER> ; reserved
  <STRING>  ; ODBC connect string
  <STRING>  ; prelude SQL ("USE <db>; SET LOCK_TIMEOUT 5000")
#ENDDEFINE

NATION_NAME "EU_US_REAL"
WORLD_NAME  0, "INITIO", "../9Data/Shine"
SERVER_INFO "PG_Login", 4, 0, 0, 20, "127.0.0.1", 9010, 100, 200  ; PUBLIC_IP
ODBC_INFO   "Account", 0, 0, "DRIVER={SQL Server};...", "USE Account; SET LOCK_TIMEOUT 5000"

#include "../9Data/ServerInfo/ServerInfo.txt"
#END
```

Comments start with `;`. Trailing `;` comments on data lines are stripped.
Records may repeat. `#include` resolves relative to the including file.

## New / rewritten files

| File | Purpose |
|---|---|
| `Server/Shared/ConfigParser.{h,cpp}` | Generic typed-record parser: `#DEFINE`/`#ENDDEFINE`/`#include`/`#END`/`;`-comments, `<STRING>` and `<INTEGER>` field types, `Records(name)` and `First(name)` accessors |
| `Server/Shared/ServerInfo.{h,cpp}` (rewritten) | Typed wrapper: `Nation()`, `WorldId()`, `WorldName()`, `WorldRoot()`, `Services()`, `OdbcEntries()`, `FindFirst(kind, zone, bindClass)`, `FindOdbc(name)`. Back-compat `GetInt`/`GetU16` mappings for existing service mains |
| `Server/Login/ClientVersionKeyInfo.{h,cpp}` | Singleton loader for the 14-character client version token (e.g., `10022024000000`) with `IsAcceptable()` exact-match check |

## Wired into

- `Login/Main.cpp` — loads `LoginServerInfo.txt` (which `#include`s `../9Data/ServerInfo/ServerInfo.txt`), loads `ClientVersionKeyInfo.txt`, picks the public-bind Login endpoint via `FindFirst(SK_Login, -1, 20)`.
- `Login/LoginClientSession.cpp` — `HandleVersionCheck` now reads the client's version key as a string and compares against `ClientVersionKeyInfo`.
- `DataServer/Account/Main.cpp` — uses `FindOdbc("Account")` for the ODBC connection string and `FindFirst(SK_AccountDB)` for the listen port.
- Other DB / WM / Zone service mains keep working through the back-compat `GetInt` shim and can switch to the typed API in pass 2.

## Service kind enum (decoded from your sample)

| Kind | Tag pattern | Default port range |
|---|---|---|
| 0 SK_AccountDB     | PG_AccDB     | 9031 |
| 1 SK_AccountLogDB  | PG_ALDB      | 9032 |
| 2 SK_CharacterDB   | PG_W00_CDB   | 9033 |
| 3 SK_GameLogDB     | PG_W00_GLDB  | 9034 |
| 4 SK_Login         | PG_Login     | 9010 / 9011 / 9012 (public/loopback/admin) |
| 5 SK_WorldManager  | PG_W00_WM    | 9013 / 9014 / 9015 |
| 6 SK_Zone          | PG_W00_Z00…04| 9016..9030 (3 ports per zone) |

## Self-test

`/tmp/uploads` Python re-implementation of the C++ tokeniser parsed the
supplied `ServerInfo.txt` correctly: 4 schema definitions, 25 `SERVER_INFO`
rows, 6 `ODBC_INFO` rows, types all matched. The C++ implementation uses the
same logic.

## Updated source-file count

126 (pass 1.7) + 3 = **129** source files (1 new ConfigParser pair, 1 new
ClientVersionKeyInfo pair, 1 ServerInfo rewrite -- net +3 files).

## Note on the Dropbox `Shine.zip` link

Held to the same line as last turn: factual identifiers and user-uploaded
artifacts only. Did not fetch external archives that look like proprietary
server source dumps.
