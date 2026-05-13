// Client/ResSystem/PEResourceReader.h
// Reads files embedded in the exe's PE resource section at runtime.
// Resources are compiled in via ShineResources.rc at build time.
//
// Resource layout in .rc:
//   SHINE_DATA  "SHN"  "ItemInfo.shine"
//   WEASEL      "DAT"  "Weasel.dat"
//   etc.
//
// At runtime:
//   PEResourceReader::Find("ItemInfo", "SHN", size) -> raw pointer into PE image
//   PEResourceReader::Find("Weasel",   "DAT", size) -> raw pointer into PE image
//
// Pointer lifetime: valid for the lifetime of the process. No copy, no alloc.
// The PE image is mapped read-only by the OS loader -- don't write to it.
#ifndef SHINE_CLIENT_RESSYSTEM_PERESOURCEREADER_H
#define SHINE_CLIENT_RESSYSTEM_PERESOURCEREADER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <string>
#include "../../Server/Shared/ShineTypes.h"

namespace shine {

class PEResourceReader {
public:
    // Find a named resource embedded in the exe.
    // szName = resource name (e.g. "ItemInfo", "Weasel")
    // szType = resource type (e.g. "SHN", "DAT")
    // rdwSize = filled with resource size in bytes on success
    // Returns pointer into the mapped PE image, or NULL if not found.
    static const void* Find(const char* szName, const char* szType, DWORD& rdwSize);

    // Convenience: check existence without caring about the pointer
    static bool Exists(const char* szName, const char* szType);

    // Fill a vector with the resource bytes (makes a copy -- use sparingly)
    static bool ReadInto(const char* szName, const char* szType,
                         std::vector<uint8>& rOut);
};

} // namespace shine
#endif // SHINE_CLIENT_RESSYSTEM_PERESOURCEREADER_H
