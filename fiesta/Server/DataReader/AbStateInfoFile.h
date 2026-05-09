// Server/DataReader/AbStateInfoFile.h
// 02 -- readers for the AbState/ binary set:
//   * AbStateInfo.dat  -- master index of abnormal states (count + name table).
//   * Sta<XXX>.dat     -- per-state timeline blob, each StaXxx.dat is an array
//                          of records starting with a u32 record count and
//                          followed by 28-byte rows (5 * uint32 + dword) each.
//
// Format observed from the supplied AbState/ binaries:
//   AbStateInfo.dat:
//     [00..03] uint32 count          (e.g. 0x0000000A = 10 entries)
//     For each entry:
//       [00..00] uint8  mainStateId  (1..N)
//       [01..??] ascii  EnglishInxName (null-terminated, 32 bytes max)
//       [??..??] ascii  KoreanLabel    (32 bytes max -- shift-jis bytes)
//
//   StaXxx.dat:
//     [00..03] uint32 recordCount     (e.g. 0x97 = 151)
//     [04..  ] records, each 28 bytes laid out as:
//        uint32 timestepMs / level
//        uint32 actionId
//        uint32 strength
//        uint32 effectIndex  (low word = handle, high word = pos)
//        uint32 keepFrames
//        uint32 reserved
//        uint32 reserved
//
// Field semantics (timestep / actionId / strength) are tentative; we surface
// the raw fields so callers can reinterpret. Parsing is best-effort.
//
// EVIDENCE: HEX_PARTIAL  source: project-owner-supplied AbState/ binaries.
#ifndef FIESTA_DATAREADER_ABSTATEINFOFILE_H
#define FIESTA_DATAREADER_ABSTATEINFOFILE_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace fiesta {

struct AbStateMasterEntry {
    uint8       uiMainStateId;
    std::string kInxName;       // English Sta* name
    std::string kLabel;          // Localized label (raw bytes from file)
};
struct AbStateMaster {
    std::vector<AbStateMasterEntry> kEntries;
    std::vector<uint8>              kRest;       // unparsed trailer
};
bool LoadAbStateInfo(const std::string& rPath, AbStateMaster& rOut);

struct AbStateTimelineRow {
    uint32 aField[7];           // raw u32 view
};
struct AbStateTimeline {
    std::vector<AbStateTimelineRow> kRows;
    uint32 uiRecCount;
    uint32 uiRowBytes;          // detected stride
};
bool LoadAbStateTimeline(const std::string& rPath, AbStateTimeline& rOut);

} // namespace fiesta
#endif
