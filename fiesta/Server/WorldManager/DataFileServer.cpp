// Server/WorldManager/DataFileServer.cpp
// SHN ingest authority on the WM. Mirrors the boot path the Zone uses:
// load every .shn under <root>/Shine/, run the column auditor, log a count.
// Cross-zone services (KQServer, NpcScheduleServer, GMEventManager, ...)
// pull table contents directly from ShnRegistry::Get() once this finishes.
#include "WMServices.h"
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

DataFileServer& DataFileServer::Get() { static DataFileServer s; return s; }

bool DataFileServer::LoadAll(const std::string& rDataRoot) {
    size_t uiLoaded = ShnRegistry::Get().LoadAll(rDataRoot);
    SHINELOG_INFO("DataFileServer: %u SHN tables loaded from '%s'",
                  (uint32)uiLoaded, rDataRoot.c_str());
    if (uiLoaded == 0) return false;
    ShnAudit_EmitReport(ShnRegistry::Get());
    return true;
}

} // namespace shine
