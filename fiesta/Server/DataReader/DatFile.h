// Server/DataReader/DatFile.h
// Action/AbState .dat reader. These files bridge SHN data rows to NIF
// animation clips: a MobInfo / NPCInfo row references an action set name; the
// matching <Name>.dat lists clip-name entries the client looks up in the
// associated Gamebryo NIF when an action opcode arrives.
// Format (derived empirically from the samples in misc/Action/*.dat and
// misc/AbState/*.dat -- all sizes match exactly):
//   [uint32 entry_count][entry x entry_count]
//   Action  entry size = 352 bytes
//   AbState entry size = 104 bytes
// Entry layout (Action; the prefix on AbState looks similar):
//   [uint16  uiKind]        // index/type tag
//   [char    szClip[?]]     // zero-padded clip name; trailing 0xCD heap fill
//   [bytes   abPayload[]]   // animation parameters (blend frames, loop flags,
//                              sound triggers, hit-frame markers)
//                              once we know which sub-fields the client reads.
#ifndef SHINE_DATAREADER_DATFILE_H
#define SHINE_DATAREADER_DATFILE_H
#include "../Shared/ShineTypes.h"
#include <vector>
#include <string>

namespace shine {

enum DatKind { DAT_KIND_ACTION = 0, DAT_KIND_ABSTATE = 1 };

struct DatEntry {
    uint16 uiKind;
    std::string kClipName;           // up to first NUL or 0xCD fill byte
    std::vector<uint8> kPayload;     // raw remainder, decoded in pass 2
};

class DatFile {
public:
    DatFile();

    bool LoadFromFile(const std::string& rPath, DatKind eKind);

    DatKind                    Kind()    const { return m_eKind; }
    uint32                     Count()   const { return (uint32)m_kEntries.size(); }
    const std::vector<DatEntry>& Entries() const { return m_kEntries; }
    static size_t              EntrySizeFor(DatKind e) { return e == DAT_KIND_ABSTATE ? 104 : 352; }

private:
    DatKind                m_eKind;
    std::vector<DatEntry>  m_kEntries;
};

// Per-Action lookup table: maps logical Mob/NPC name -> DatFile.
class ActionDatBox {
public:
    static ActionDatBox& Get();
    const DatFile* Load(const std::string& rRoot, const std::string& rLogicalName);
    const DatFile* Find(const std::string& rLogicalName) const;
    void   Clear();
private:
    std::map<std::string, DatFile*> m_kAll;
};

} // namespace shine
#endif
