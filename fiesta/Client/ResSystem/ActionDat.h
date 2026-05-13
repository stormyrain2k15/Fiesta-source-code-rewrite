// Client/ResSystem/ActionDat.h
// Loader for ressystem/action/<InxName>.dat
// Links an actor InxName to its Gamebryo animation sequences.
//
// Binary layout (little-endian), verified against ActionReader v12 source:
//
//   [0x00] int32   entry_count
//   entry_count x ActionBlock:
//     int16    ActID          -- action index (0,1,2...)
//     char[33] SHNPath        -- NIF animation sequence name (e.g. "Weasel_stand")
//     char[33] FilePath       -- KF file reference
//     int32    loop           -- always -1
//     char[36] Text           -- action type: "stand","walk","run","attack","damage","die","skill"
//     int32    NifAnimID      -- structured anim ID (102000=stand, 201000=walk, 301000=attack...)
//     int32    zeros
//     WeirdBlock[33] s1       -- event trigger 1 (e.g. "MonAtk1")
//     WeirdBlock[33] s2       -- event trigger 2
//     WeirdBlock[34] s3       -- event trigger 3
//     byte[48] something      -- unknown block
//     WeirdBlock[33] Sound1   -- sub-animation 1 (e.g. "Weasel_walk01")
//     WeirdBlock[35] Sound2   -- sub-animation 2
//     int32    Signature      -- always 8
//     byte[16] somthingelse   -- unknown block
//   = 352 bytes per entry
//
// WeirdBlock[N]: if first 2 bytes == 0x2D 0x00 ('-'), field is empty;
//   consume remaining N-2 bytes as padding. Otherwise read N bytes as string.
#ifndef SHINE_CLIENT_RESSYSTEM_ACTIONDAT_H
#define SHINE_CLIENT_RESSYSTEM_ACTIONDAT_H

#include "../../Server/Shared/ShineTypes.h"
#include <string>
#include <vector>
#include <map>

namespace shine {

struct ActionBlock {
    int16       iActID;
    std::string kSHNPath;       // NIF animation sequence name
    std::string kFilePath;      // KF file reference
    std::string kText;          // action type: stand/walk/run/attack/damage/die/skill
    int32       iNifAnimID;     // structured anim ID
    std::string kS1;            // event trigger 1 (MonAtk1, DeathBase, etc.)
    std::string kS2;            // event trigger 2
    std::string kS3;            // event trigger 3
    std::string kSound1;        // sub-animation 1 (blend anim name)
    std::string kSound2;        // sub-animation 2
    int32       iSignature;     // always 8
    uint8       aSomething[48]; // unknown
    uint8       aSomethingElse[16]; // unknown

    bool HasEvent1()   const { return !kS1.empty(); }
    bool HasSubAnims() const { return !kSound1.empty(); }
};

class ActionDat {
public:
    ActionDat();

    bool Load(const std::string& rPath);
    bool IsLoaded() const { return m_bLoaded; }

    // Lookup by action type string ("stand","walk","run","attack","damage","die","skill")
    const ActionBlock* FindByType(const std::string& rType) const;

    // Lookup by NIF sequence name ("Weasel_stand" etc.)
    const ActionBlock* FindBySeq(const std::string& rSeq) const;

    // Lookup by NifAnimID
    const ActionBlock* FindByAnimID(int32 iAnimID) const;

    const std::vector<ActionBlock>& Blocks()  const { return m_kBlocks; }
    uint32                          Count()   const { return (uint32)m_kBlocks.size(); }
    const std::string&              InxName() const { return m_kInxName; }

private:
    std::vector<ActionBlock>            m_kBlocks;
    std::map<std::string, size_t>       m_kByType;
    std::map<std::string, size_t>       m_kBySeq;
    std::map<int32,       size_t>       m_kByAnimID;
    std::string                         m_kInxName;
    bool                                m_bLoaded;

    static std::string ReadString(const uint8* p, size_t& rPos, int iLen);
    static std::string ReadWeirdBlock(const uint8* p, size_t& rPos, int iLen);
};

class ActionDatCache {
public:
    static ActionDatCache& Get();
    const ActionDat* Acquire(const std::string& rInxName, const std::string& rActionDir);
    void Clear();
private:
    ActionDatCache() {}
    std::map<std::string, ActionDat> m_kCache;
};

} // namespace shine
#endif // SHINE_CLIENT_RESSYSTEM_ACTIONDAT_H
