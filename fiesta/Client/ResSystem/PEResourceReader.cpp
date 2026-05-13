// Client/ResSystem/PEResourceReader.cpp
#include "PEResourceReader.h"
#include "../../Server/Shared/ShineLogSystem.h"
#include <vector>

namespace shine {

const void* PEResourceReader::Find(const char* szName, const char* szType, DWORD& rdwSize) {
    rdwSize = 0;

    HRSRC hRes = FindResourceA(NULL, szName, szType);
    if (!hRes) {
        SHINELOG_DEBUG("PEResource: not found name='%s' type='%s'", szName, szType);
        return NULL;
    }

    rdwSize = SizeofResource(NULL, hRes);
    if (rdwSize == 0) return NULL;

    HGLOBAL hMem = LoadResource(NULL, hRes);
    if (!hMem) return NULL;

    return LockResource(hMem);
    // Note: LockResource returns a pointer directly into the mapped PE image.
    // No FreeResource needed -- it's a no-op on Win32 since the OS manages it.
}

bool PEResourceReader::Exists(const char* szName, const char* szType) {
    return FindResourceA(NULL, szName, szType) != NULL;
}

bool PEResourceReader::ReadInto(const char* szName, const char* szType,
                                 std::vector<uint8>& rOut) {
    DWORD dwSize = 0;
    const void* p = Find(szName, szType, dwSize);
    if (!p || dwSize == 0) return false;
    rOut.assign((const uint8*)p, (const uint8*)p + dwSize);
    return true;
}

} // namespace shine
