# Pass 1.16 -- OperatorTool exe drop, loopback-only admin entry (2026-02)

User clarification: the OperatorTool was the admin / dev control panel, NOT a
game function. Nothing in the engine depends on it. Existing services merely
**accepted** its connection (and only over loopback). So:

- **Don't ship a new OperatorTool exe.**
- **Keep the connectors / SQL surface** so any external admin panel can
  attach and drive everything (jail, sysmsg, give/remove item, read every DB).

## Removed

- `Server/OperatorTool/` -- the entire dir + `Main.cpp`. Build target,
  README listing, and `BUILD.md` run-order line all stripped.

## Kept (as referenced)

- `WMOPToolSession`            (`Server/WorldManager/WorldManagerServer.h`)
- `SQLP_Operator`              (`Server/DataServer/Common/SQLP.h/.cpp`)
- `DB::tOperator`, `DB::tMenuAuth` (`Server/DataServer/Common/DBSchemaConstants.h`)
- All `NC_OPTOOL_*` opcodes    (`Server/Common/NETCOMMAND.h`)

## Code changes

### `Server/Shared/Socket_Acceptor.{h,cpp}`
- Added an optional `bool bLoopbackOnly = false` parameter to
  `Socket_Acceptor::Start`.
- When set, the listen socket binds to `INADDR_LOOPBACK` (`127.0.0.1`)
  instead of `INADDR_ANY`, **and** the AcceptLoop double-checks the peer's
  address is on `127.0.0.0/8` before instantiating a session. Defence in
  depth -- the kernel already rejects external peers, but the second
  check covers a future caller that forgets the flag.

### `Server/WorldManager/Main.cpp`
- The OPTool acceptor (`WM.OPToolPort`, default 28004) now starts with
  `bLoopbackOnly=true`. The other four acceptors (Client, Zone, Login,
  CharDB) remain network-bindable as before.

### `Server/WorldManager/WorldManagerServer.{h,cpp}`
- `WMOPToolSession` now tracks `m_iOperLevel` + `m_bAuthed`.
- Auth flow uses a lazy, WM-local `SQLP_Operator` instance that calls
  `p_Operator_Logon` against the OperatorTool DB. Once authed, every
  subsequent command on the same session inherits the operator level.
- Real dispatch table for the admin command surface. Each command is
  documented with its wire layout in the .cpp:
  - `NC_OPTOOL_AUTH_REQ` / `_ACK`
  - `NC_OPTOOL_BAN_CMD`, `NC_OPTOOL_KICK_CMD`
  - `NC_OPTOOL_JAIL_CMD`, `NC_OPTOOL_UNJAIL_CMD`
  - `NC_OPTOOL_SYSMSG_CMD` (world / all-worlds broadcast)
  - `NC_OPTOOL_GIVEITEM_CMD`, `NC_OPTOOL_TAKEITEM_CMD`
  - `NC_OPTOOL_QUERY_REQ` / `_ACK`  (read-only SELECT against any DB; a
    runtime safety net rejects any non-`SELECT` verb even if the panel
    sends one).
  - `NC_OPTOOL_RESULT_ACK`           (generic success / error reply)
- Pre-auth packets are rejected with a `RESULT_ACK { ok=0, "not authed" }`.

### `Server/Common/NETCOMMAND.h`
- Added `NC_OPTOOL_JAIL_CMD`, `_UNJAIL_CMD`, `_SYSMSG_CMD`,
  `_GIVEITEM_CMD`, `_TAKEITEM_CMD`, `_QUERY_REQ`, `_QUERY_ACK`,
  `_RESULT_ACK` (`NC_FAMILY_OPTOOL + 0x06..0x0D`).

## Net result

The engine ships without an OperatorTool exe, but anyone running a Win32
admin panel on the same host can:

1. `connect("127.0.0.1", 28004)`
2. Send `NC_OPTOOL_AUTH_REQ` with their tOperator credentials.
3. On `NC_OPTOOL_AUTH_ACK { ok=1 }`, drive jail / sysmsg / item-give /
   item-take / read-only queries.

If they don't connect, gameplay is unaffected.

## Total source files
**~190** (one Main.cpp deleted; this doc + Socket_Acceptor + WMOPToolSession
+ NETCOMMAND.h + WM/Main.cpp updated).
