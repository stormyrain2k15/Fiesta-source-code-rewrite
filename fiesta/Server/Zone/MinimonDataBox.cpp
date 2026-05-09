// Server/Zone/MinimonDataBox.cpp
// WIRE-19 (Lyra, May 2026): lifted from anonymous namespace; table name
// corrected to MinimonInfo (actual NA2016 SHN file).
#include "MinimonDataBox.h"
#include "../DataReader/ShnRegistry.h"
namespace fiesta {
MinimonDataBox& MinimonDataBox::Get() { static MinimonDataBox s; return s; }
bool MinimonDataBox::Load() {
    return ShnRegistry::Get().GetTable("MinimonInfo") != NULL;
}
} // namespace fiesta
