// Server/DataReader/DataReader.h
// top-level SHN/TXT loader and table registry.
#ifndef FIESTA_DATAREADER_H
#define FIESTA_DATAREADER_H
#include "../Shared/ShineTypes.h"
#include "ShnDataFileCheckSum.h"
#include "ITableBase.h"
#include <string>
#include <vector>
#include <map>

namespace fiesta {

class DataReader {
public:
    DataReader();
    ~DataReader();

    // Sets the on-disk root for /Data ("Data\\..." mirrors the original layout).
    void SetRoot(const std::string& rRoot) { m_kRoot = rRoot; }
    const std::string& GetRoot() const     { return m_kRoot; }

    // Load <root>/<rLogicalName>.shn (or .txt). Returns rows of strings parsed
    // from a tab/comma-separated record file. SHN binary parsing belongs behind
    // the guarded loader; quest SHNs are refused.
    bool LoadAsRows(const std::string& rLogicalName,
                    std::vector<std::vector<std::string> >& rOut);

    // Read entire file as raw buffer (for binary table types not yet decoded).
    bool LoadRaw(const std::string& rLogicalName, std::vector<uint8>& rOut);

    // Cross-checks against ShineDataFileCheckSum.
    ShnDataFileCheckSum& Checksum() { return m_kChecksum; }
private:
    std::string         m_kRoot;
    ShnDataFileCheckSum m_kChecksum;
    bool ReadTextRows(const std::string& rPath,
                      std::vector<std::vector<std::string> >& rOut);
};

} // namespace fiesta
#endif
