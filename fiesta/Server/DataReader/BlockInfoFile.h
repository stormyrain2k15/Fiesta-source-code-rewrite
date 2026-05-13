// Server/DataReader/BlockInfoFile.h
// readers for the three binary terrain / blocking formats shipped under
// BlockInfo/<MapID>.{shbd,aid,sbi}.
// Layout observations from the supplied Shine.zip:
//   *.shbd  (SHine BlockInformation Data):
//     [00..03] uint32 width   (e.g. 0x000003B6)
//     [04..07] uint32 height  (e.g. 0x00001DB0 -- bytes per row * h or total cells)
//     [08..  ] packed cells   (1 byte per cell, 0xFF = blocked, 0x00 = open).
//   The size of file - 8 always equals width * height (verified for Adl.shbd,
//   AdlF.shbd). Cells are streamed in row-major order.
//   *.aid   (Area ID map):
//     [00..03] uint32 entryCount (e.g. 1)
//     [04..04+32*N-1] N entries of 32-byte ASCII names ("MH_Zone1"...).
//     <after names> uint32 cellW, uint32 cellH (some files), then a packed
//     uint8 grid that indexes into the name table. We expose names + raw
//     payload; full rendering is left to the renderer.
//   *.sbi   (Sub-Block Information / door blocks):
//     [00..03] uint32 entryCount
//     [04..]   N entries; each entry: 32-byte ASCII name + 5*uint32 fields
//              (x0,y0,x1,y1,flags). Trailing null byte then the next name.
// We don't have authoritative format docs; the readers below extract whatever
// is parsable and surface unknown bytes as a raw blob so the rest of the code
// can wire to them without losing information.
//                                Adl.shbd, Adl.aid, AdlF.sbi.
#ifndef SHINE_DATAREADER_BLOCKINFOFILE_H
#define SHINE_DATAREADER_BLOCKINFOFILE_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace shine {

// .shbd -- packed walk grid.
struct BlockGrid {
    uint32             uiWidth, uiHeight;
    std::vector<uint8> kCells;          // size = uiWidth * uiHeight (one byte per cell).
};
bool LoadShbd(const std::string& rPath, BlockGrid& rOut);

// .aid -- area-id name table + raw payload.
struct AidEntry { std::string kName; };
struct AidFile {
    std::vector<AidEntry> kEntries;
    std::vector<uint8>    kPayload;     // bytes after name table (raw; layout TBD)
};
bool LoadAid(const std::string& rPath, AidFile& rOut);

// .sbi -- sub-block information (door blockers, region rects).
struct SbiEntry {
    std::string kName;
    uint32      aField[5];      // typically (x0,y0,x1,y1,flags) -- size verified
};
struct SbiFile {
    std::vector<SbiEntry> kEntries;
    std::vector<uint8>    kRest;
};
bool LoadSbi(const std::string& rPath, SbiFile& rOut);

} // namespace shine
#endif
