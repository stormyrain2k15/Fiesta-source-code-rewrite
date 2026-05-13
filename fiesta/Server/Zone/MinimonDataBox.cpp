// Server/Zone/MinimonDataBox.cpp
// table name
// corrected to MinimonInfo (actual NA2016 SHN file).
#include "MinimonDataBox.h"
#include "../DataReader/ShnRegistry.h"
namespace shine {
MinimonDataBox& MinimonDataBox::Get() { static MinimonDataBox s; return s; }
bool MinimonDataBox::Load() {
    return ShnRegistry::Get().GetTable("MinimonInfo") != NULL;
}
} // namespace shine
