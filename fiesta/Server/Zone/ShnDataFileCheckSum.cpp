// Server/Zone/ShnDataFileCheckSum.cpp
// Zone-side facade. The canonical implementation lives in
// Server/DataReader/ShnDataFileCheckSum.{h,cpp} (which uses the
// MD5Checksum::Compute / ToHex API and includes the protected-quest
// guard). Zone code that needs to verify an SHN file should construct
// a ShnDataFileCheckSum instance via the DataReader header. This TU
// is kept as a project-file slot so the NA2016-style file inventory
// remains intact, but emits no symbols of its own.
#include "../DataReader/ShnDataFileCheckSum.h"
namespace shine { /* see DataReader/ShnDataFileCheckSum.cpp */ }
