// Server/DataReader/DataReader.cpp
// top-level data loader. Tries .shn (real binary, decrypt + parse) first,
// falls back to .txt for tabular mirrors. Quest SHNs are refused via
// ShnDataFileCheckSum::IsAllowed (spec rule 02).
#include "DataReader.h"
#include "ShnFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <string.h>

namespace shine {

DataReader::DataReader() : m_kRoot("9Data") {}
DataReader::~DataReader() {}

static std::string Join(const std::string& a, const std::string& b) {
    if (a.empty()) return b;
    char back = a[a.size() - 1];
    if (back == '\\' || back == '/') return a + b;
    return a + "\\" + b;
}

bool DataReader::LoadRaw(const std::string& rLogical, std::vector<uint8>& rOut) {
    if (!m_kChecksum.IsAllowed(rLogical)) {
        SHINELOG_WARN("DataReader: refused (quest-protected) %s", rLogical.c_str());
        return false;
    }
    rOut.clear();
    const char* aExt[3] = { ".shn", ".txt", "" };
    for (int i = 0; i < 3; ++i) {
        std::string p = Join(m_kRoot, rLogical) + aExt[i];
        FILE* fp = NULL; fopen_s(&fp, p.c_str(), "rb");
        if (!fp) continue;
        fseek(fp, 0, SEEK_END); long sz = ftell(fp); fseek(fp, 0, SEEK_SET);
        if (sz < 0) { fclose(fp); continue; }
        rOut.resize((size_t)sz);
        if (sz > 0) fread(&rOut[0], 1, (size_t)sz, fp);
        fclose(fp);
        if (!m_kChecksum.Verify(rLogical, sz ? &rOut[0] : NULL, (size_t)sz)) return false;
        return true;
    }
    SHINELOG_WARN("DataReader: not found %s under %s", rLogical.c_str(), m_kRoot.c_str());
    return false;
}

static bool ReadTextRows(const std::string& rPath,
                         std::vector<std::vector<std::string> >& rOut) {
    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) return false;
    char line[8192];
    while (fgets(line, sizeof(line), fp)) {
        std::vector<std::string> row;
        std::string cur;
        for (size_t i = 0; line[i] && line[i] != '\r' && line[i] != '\n'; ++i) {
            char c = line[i];
            if (c == '\t' || c == ',') { row.push_back(cur); cur.clear(); }
            else cur.push_back(c);
        }
        row.push_back(cur);
        if (!row.empty() && !(row.size() == 1 && row[0].empty()))
            rOut.push_back(row);
    }
    fclose(fp);
    return true;
}

bool DataReader::LoadAsRows(const std::string& rLogical,
                            std::vector<std::vector<std::string> >& rOut) {
    if (!m_kChecksum.IsAllowed(rLogical)) {
        SHINELOG_WARN("DataReader: refused (quest-protected) %s", rLogical.c_str());
        return false;
    }
    // 1) Try .shn (real binary) and convert to string rows.
    {
        std::string p = Join(m_kRoot, rLogical) + ".shn";
        ShnFile shn;
        if (shn.LoadFromFile(p)) {
            shn.ExportAsStringRows(rOut);
            return true;
        }
    }
    // 2) Fall back to plain .txt mirror.
    {
        std::string p = Join(m_kRoot, rLogical) + ".txt";
        std::vector<std::vector<std::string> > kRows;
        if (ReadTextRows(p, kRows)) { rOut.swap(kRows); return true; }
    }
    return false;
}

bool DataReader::ReadTextRows(const std::string& rPath,
                              std::vector<std::vector<std::string> >& rOut) {
    return shine::ReadTextRows(rPath, rOut);
}

} // namespace shine
