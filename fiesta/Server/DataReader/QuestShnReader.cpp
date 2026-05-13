// Server/DataReader/QuestShnReader.cpp
#include "QuestShnReader.h"
#include "ShnRegistry.h"
#include "ShnFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <ctype.h>

namespace shine {

QuestShnReader& QuestShnReader::Get() { static QuestShnReader s; return s; }

namespace {
    bool SlurpFile(const std::string& rPath, std::vector<uint8>& rOut) {
        FILE* fp = fopen(rPath.c_str(), "rb");
        if (!fp) return false;
        fseek(fp, 0, SEEK_END);
        long n = ftell(fp);
        fseek(fp, 0, SEEK_SET);
        if (n <= 0) { fclose(fp); return false; }
        rOut.resize((size_t)n);
        size_t r = fread(&rOut[0], 1, (size_t)n, fp);
        fclose(fp);
        return r == (size_t)n;
    }
}

size_t QuestShnReader::LoadAllDeferred() {
    size_t loaded = 0;
    const ShnRegistry& reg = ShnRegistry::Get();
    for (ShnRegistry::iterator it = reg.begin(); it != reg.end(); ++it) {
        const ShnFile* f = it->second;
        if (!f || !f->IsQuestDeferred()) continue;
        if (LoadOne(it->first, f->QuestDeferredPath())) ++loaded;
    }
    SHINELOG_INFO("QuestShnReader: loaded %u deferred quest SHN(s)",
                  (uint32)loaded);
    return loaded;
}

bool QuestShnReader::LoadOne(const std::string& rStem, const std::string& rPath) {
    // Lower-case the stem for matching; quest files commonly drop in
    // multiple casings across the in-game data folders.
    std::string lower = rStem;
    for (size_t i = 0; i < lower.size(); ++i)
        lower[i] = (char)::tolower((unsigned char)lower[i]);

    if (lower == "questdata"      || lower.find("questdata")      != std::string::npos)
        return LoadQuestData(rPath);
    if (lower == "questdialog"    || lower.find("questdialog")    != std::string::npos)
        return LoadQuestDialog(rPath);
    if (lower == "questspecies"   || lower.find("questspecies")   != std::string::npos)
        return LoadQuestSpecies(rPath);
    if (lower == "questevent"     || lower.find("questevent")     != std::string::npos)
        return LoadQuestEvent(rPath);
    if (lower == "questframework" || lower.find("questframework") != std::string::npos)
        return LoadQuestFramework(rPath);
    if (lower.find("pinescript")  != std::string::npos)
        return LoadPineScript(rPath);

    SHINELOG_WARN("QuestShnReader: no specialization for '%s' (path=%s)",
                  rStem.c_str(), rPath.c_str());
    return false;
}

// Per-file specializations -- minimal implementations that slurp the
// raw bytes and stash them under a row keyed by an id derivable from
// the file. Real column extraction lands when each on-disk format is
// RE'd; until then the runtime can still tell "quest is loaded" vs
// "quest is missing" and the bytes are available for offline study.

bool QuestShnReader::LoadQuestData(const std::string& rPath) {
    std::vector<uint8> body;
    if (!SlurpFile(rPath, body)) {
        SHINELOG_WARN("QuestShnReader::LoadQuestData: open failed: %s", rPath.c_str());
        return false;
    }
    // VERIFY: real QuestData.shn record format not RE'd in this tree.
    // The body is preserved on a sentinel row keyed by id 0; once the
    // format is decoded, replace this with a row-walking parser.
    QuestRow r;
    r.uiQuestId = 0;
    r.uiKind    = 0;
    r.kInxName  = "QuestData";
    r.kRawBody.swap(body);
    m_kQuests[r.uiQuestId] = r;
    SHINELOG_INFO("QuestShnReader: QuestData loaded (%u bytes)",
                  (uint32)m_kQuests[0].kRawBody.size());
    return true;
}

bool QuestShnReader::LoadQuestDialog(const std::string& rPath) {
    std::vector<uint8> body;
    if (!SlurpFile(rPath, body)) return false;
    SHINELOG_INFO("QuestShnReader: QuestDialog deferred (%u bytes, format pending RE)",
                  (uint32)body.size());
    return true;
}
bool QuestShnReader::LoadQuestSpecies(const std::string& rPath) {
    std::vector<uint8> body;
    if (!SlurpFile(rPath, body)) return false;
    SHINELOG_INFO("QuestShnReader: QuestSpecies deferred (%u bytes, format pending RE)",
                  (uint32)body.size());
    return true;
}
bool QuestShnReader::LoadQuestEvent(const std::string& rPath) {
    std::vector<uint8> body;
    if (!SlurpFile(rPath, body)) return false;
    SHINELOG_INFO("QuestShnReader: QuestEvent deferred (%u bytes, format pending RE)",
                  (uint32)body.size());
    return true;
}
bool QuestShnReader::LoadQuestFramework(const std::string& rPath) {
    std::vector<uint8> body;
    if (!SlurpFile(rPath, body)) return false;
    SHINELOG_INFO("QuestShnReader: QuestFramework deferred (%u bytes, format pending RE)",
                  (uint32)body.size());
    return true;
}
bool QuestShnReader::LoadPineScript(const std::string& rPath) {
    std::vector<uint8> body;
    if (!SlurpFile(rPath, body)) return false;
    // PineScript is bytecode; the QuestParserScript class consumes it.
    // Here we only confirm the bytes loaded so the runtime can decide
    // whether quest scripting is available at all.
    SHINELOG_INFO("QuestShnReader: PineScript bytecode loaded (%u bytes)",
                  (uint32)body.size());
    return true;
}

const QuestRow* QuestShnReader::FindQuest(uint32 uiQuestId) const {
    std::map<uint32, QuestRow>::const_iterator it = m_kQuests.find(uiQuestId);
    return it == m_kQuests.end() ? NULL : &it->second;
}

} // namespace shine
