// Server/DataReader/QuestShnReader.h
// Dedicated quest/scenario SHN reader.
//
// The generic ShnFile parser refuses to touch quest/PineScript SHNs
// because they ship with a different on-disk shape (column descriptors
// reference scripted record types and variable-length encrypted bodies
// the generic decoder is not authorized to interpret). ShnRegistry
// tags those files as "quest deferred" placeholders at boot; this
// reader is the single authorized consumer.
//
// Construction model:
//   1. Boot: ShnRegistry::EnumerateShn -> ShnFile::MarkAsQuestDeferred
//      (tags the placeholder with the on-disk path)
//   2. Quest subsystem: QuestShnReader::Get().LoadAllDeferred() walks
//      the registry, picks every quest-deferred placeholder, and runs
//      a per-file specialized parser keyed off the logical name
//      (QuestData / QuestDialog / QuestSpecies / QuestEvent /
//      QuestFramework / PineScript).
//   3. Quest runtime: QuestSystem queries this reader, never the
//      generic ShnRegistry.
//
// CSV exporters / audit tooling: call ShnRegistry::IsQuestShn() to
// SKIP these files entirely. They must not invoke this reader.
#ifndef SHINE_DATAREADER_QUESTSHNREADER_H
#define SHINE_DATAREADER_QUESTSHNREADER_H
#include "../Shared/ShineTypes.h"
#include <map>
#include <string>
#include <vector>

namespace shine {

// Per-quest record produced by the dedicated reader. Field set is
// deliberately minimal -- the canonical NA2016 quest record shape is
// not yet RE'd in this tree, and inventing one would lock us into a
// parse-time decision before the binary format is verified. Concrete
// columns are filled in by per-file specializations as the format is
// confirmed.
struct QuestRow {
    uint32      uiQuestId;
    uint8       uiKind;            // 0=normal, 1=daily, 2=event, 3=scenario
    std::string kInxName;          // string id used by PineScript
    std::vector<uint8> kRawBody;   // unparsed payload (kept until format is RE'd)
};

class QuestShnReader {
public:
    static QuestShnReader& Get();

    // Walks the ShnRegistry, picks every entry where IsQuestDeferred()
    // is true, and runs the dedicated reader against the on-disk path
    // recorded in ShnFile::QuestDeferredPath(). Returns the count of
    // files actually read. Files that fail to parse are logged and
    // skipped; the runtime stays usable for the rest.
    size_t LoadAllDeferred();

    // Direct lookup for the quest runtime. Returns NULL if no row with
    // that QuestId has been loaded (or if the file the row would come
    // from never deferred -- e.g. a filesystem missing the SHN).
    const QuestRow* FindQuest(uint32 uiQuestId) const;

    // Iteration helper (used by QuestSystem::CharQuest startup checks).
    typedef std::map<uint32, QuestRow>::const_iterator iterator;
    iterator begin() const { return m_kQuests.begin(); }
    iterator end()   const { return m_kQuests.end();   }
    size_t   size()  const { return m_kQuests.size();  }

private:
    QuestShnReader() {}

    // Per-file-name dispatcher. The logical stem (e.g. "QuestData"
    // vs "QuestDialog") drives which specialization runs.
    bool LoadOne(const std::string& rStem, const std::string& rPath);

    // Specialization stubs. Bodies fill in as each on-disk format is
    // RE'd. Until then they read the file into kRawBody and tag the
    // QuestRow with QuestId / Kind extracted from the file name.
    bool LoadQuestData      (const std::string& rPath);
    bool LoadQuestDialog    (const std::string& rPath);
    bool LoadQuestSpecies   (const std::string& rPath);
    bool LoadQuestEvent     (const std::string& rPath);
    bool LoadQuestFramework (const std::string& rPath);
    bool LoadPineScript     (const std::string& rPath);

    std::map<uint32, QuestRow> m_kQuests;
};

} // namespace shine
#endif
