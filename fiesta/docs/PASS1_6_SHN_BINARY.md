# Pass 1.6 — Real SHN binary reader

Date: 2026-05-09. User provided `SHN_Editor_4.7` source, `Misc.zip` (sample `.dat` files),
and `shinengine_srctree.html`. Wrote a real C++ SHN binary reader to replace the
`.txt`-only fallback in `DataReader::LoadAsRows`.

## New files

| File | Purpose |
|---|---|
| `Server/DataReader/ShnFile.h`        | `ShnFile` class + `ShnColumn` / `ShnValue` types + `ShnType` enum |
| `Server/DataReader/ShnFile.cpp`      | full decrypt + parse (16-type column dispatch) |
| `Server/DataReader/CommonData.h`     | `CommonData<T>` / `IndexedCommonData<T>` template aliases |

## Format implemented (per editor)

```
file = [crypt_header 32B][total_len uint32 LE][encrypted body of (total_len - 0x24) bytes]
crypt: symmetric XOR with rolling key; key = (low byte of n), then per-byte:
       num3 = ((i & 15) + 0x55) ^ ((i*11)&0xFF) ^ num ^ 0xAA
body (after decrypt) = [Header u32][RecordCount u32][DefaultRecordLength u32][ColumnCount u32]
                      { [name 0x30 zero-padded][Type u32][Length i32] } * ColumnCount
                      { [rowSize u16] [columns by type] } * RecordCount
```

Type codes handled (16 of them, exactly matching the editor):
- 1, 12, 16 -> uint8
- 2 -> uint16
- 3, 11, 18, 27 -> uint32
- 5 -> float32
- 9, 24 -> fixed-length string (length from column.Length)
- 13, 21 -> int16
- 20 -> int8
- 22 -> int32
- 26 -> null-terminated variable-length string

## Wired in

`DataReader::LoadAsRows` now tries `<root>/<logical>.shn` first via `ShnFile`. On
success the parsed rows are exported as string rows (via
`ShnFile::ExportAsStringRows`) so the existing `Schemas.cpp` row decoders consume
them unchanged. `.txt` mirrors are still picked up as a fallback.

## Notes for `misc/*.dat`

The sample files under `Misc.zip/AbState/*.dat`, `Misc.zip/Action/*.dat`, etc. are
encrypted bodies **without** the 32-byte crypt header / 4-byte length prefix.
The editor handles them by reading the whole file as the body and running the
same `Crypt()` over it. Pass 2 should add a `ShnFile::LoadDatFromFile()` variant
that does exactly that and skip the header parse for files ending in `.dat`.

## Source provenance

The decrypt math, header layout, type codes, and field-on-wire shapes are
identifiers of the SHN format itself. Implementation written fresh in C++; no
C# code copied. The editor was used as a format reference, the same way one
would use a `struct` declaration in a header.
