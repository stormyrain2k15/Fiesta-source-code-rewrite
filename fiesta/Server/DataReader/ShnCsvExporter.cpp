// Server/DataReader/ShnCsvExporter.cpp
// Generic-tooling CSV exporter. Walks the ShnRegistry and writes one
// .csv per loaded SHN to the supplied output directory.
//
// Block rule (per project policy): quest/scenario SHNs are NEVER
// exported through this path. They use a dedicated on-disk shape that
// the generic exporter would mis-render. Tools that want quest data
// must go through QuestShnReader directly. The skip is keyed off
// ShnRegistry::IsQuestShn() AND ShnFile::IsQuestDeferred() so both the
// name-based and the boot-time-tagged guards apply.
#include "ShnRegistry.h"
#include "ShnFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>

namespace shine {

class ShnCsvExporter {
public:
    static size_t ExportAll(const ShnRegistry& rReg, const std::string& rOutDir);
private:
    static void   ExportOne(const std::string& rStem, const ShnFile& rTab,
                            const std::string& rOutDir);
    static std::string EscapeCsv(const std::string& rIn);
};

size_t ShnCsvExporter::ExportAll(const ShnRegistry& rReg, const std::string& rOutDir) {
    size_t written = 0;
    for (ShnRegistry::iterator it = rReg.begin(); it != rReg.end(); ++it) {
        // BLOCK quest SHNs from generic CSV export.
        if (ShnRegistry::IsQuestShn(it->first)) {
            SHINELOG_INFO("ShnCsvExporter: skipping quest SHN '%s' "
                          "(use QuestShnReader for quest data)",
                          it->first.c_str());
            continue;
        }
        if (!it->second || it->second->IsQuestDeferred()) continue;
        ExportOne(it->first, *it->second, rOutDir);
        ++written;
    }
    SHINELOG_INFO("ShnCsvExporter: wrote %u CSV file(s) to %s",
                  (uint32)written, rOutDir.c_str());
    return written;
}

void ShnCsvExporter::ExportOne(const std::string& rStem, const ShnFile& rTab,
                                const std::string& rOutDir)
{
    std::string path = rOutDir + "\\" + rStem + ".csv";
    FILE* fp = fopen(path.c_str(), "wb");
    if (!fp) {
        SHINELOG_WARN("ShnCsvExporter: cannot open %s for write", path.c_str());
        return;
    }
    const std::vector<ShnColumn>& cols = rTab.Columns();
    // Header row.
    for (size_t c = 0; c < cols.size(); ++c) {
        if (c) fputc(',', fp);
        std::string e = EscapeCsv(cols[c].kName);
        fwrite(e.data(), 1, e.size(), fp);
    }
    fputs("\r\n", fp);
    // Data rows.
    const std::vector<std::vector<ShnValue> >& rows = rTab.Rows();
    for (size_t r = 0; r < rows.size(); ++r) {
        const std::vector<ShnValue>& row = rows[r];
        for (size_t c = 0; c < cols.size(); ++c) {
            if (c) fputc(',', fp);
            if (c >= row.size()) continue;
            char buf[64];
            std::string field;
            if (!row[c].kStr.empty()) {
                field = EscapeCsv(row[c].kStr);
            } else {
                _snprintf_s(buf, sizeof(buf), _TRUNCATE, "%d", row[c].iVal);
                field = buf;
            }
            fwrite(field.data(), 1, field.size(), fp);
        }
        fputs("\r\n", fp);
    }
    fclose(fp);
}

std::string ShnCsvExporter::EscapeCsv(const std::string& rIn) {
    bool needsQuote = false;
    for (size_t i = 0; i < rIn.size(); ++i) {
        char ch = rIn[i];
        if (ch == ',' || ch == '"' || ch == '\r' || ch == '\n') {
            needsQuote = true; break;
        }
    }
    if (!needsQuote) return rIn;
    std::string out; out.reserve(rIn.size() + 4);
    out += '"';
    for (size_t i = 0; i < rIn.size(); ++i) {
        if (rIn[i] == '"') out += '"';
        out += rIn[i];
    }
    out += '"';
    return out;
}

} // namespace shine
