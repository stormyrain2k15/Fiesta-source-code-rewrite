// Server/DataReader/SHN/Riding.cpp
// Auto-generated: one-file-per-SHN split for Riding.shn
#include "Riding.h"
#include "../../Shared/ShineLogSystem.h"

namespace fiesta {

RidingShn& RidingShn::Get() { static RidingShn s; return s; }

void RidingShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("Riding");
    if (!t) { SHINELOG_WARN("Riding.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        RidingRow rec;
        rec.uiHandle = (uint16)ShnGetU32(*t, _r, "Handle");
        rec.kItemID = ShnGetStr(*t, _r, "ItemID");
        rec.kName = ShnGetStr(*t, _r, "Name");
        rec.kBodyType = ShnGetStr(*t, _r, "BodyType");
        rec.kShape = ShnGetStr(*t, _r, "Shape");
        rec.uiUseTime = (uint16)ShnGetU32(*t, _r, "UseTime");
        rec.kFeedType = ShnGetStr(*t, _r, "FeedType");
        rec.kTexture = ShnGetStr(*t, _r, "Texture");
        rec.uiFeedGauge = (uint16)ShnGetU32(*t, _r, "FeedGauge");
        rec.uiHGauge = (uint16)ShnGetU32(*t, _r, "HGauge");
        rec.uiInitHgauge = (uint16)ShnGetU32(*t, _r, "InitHgauge");
        rec.uiTick = (uint16)ShnGetU32(*t, _r, "Tick");
        rec.uiUGauge = (uint16)ShnGetU32(*t, _r, "UGauge");
        rec.uiRunSpeed = (uint16)ShnGetU32(*t, _r, "RunSpeed");
        rec.uiFootSpeed = (uint16)ShnGetU32(*t, _r, "FootSpeed");
        rec.uiCastingTime = (uint16)ShnGetU32(*t, _r, "CastingTime");
        rec.uiCoolTime = ShnGetU32(*t, _r, "CoolTime");
        rec.kIconFileN = ShnGetStr(*t, _r, "IconFileN");
        rec.uiIconIndex = (uint16)ShnGetU32(*t, _r, "IconIndex");
        rec.kImageN = ShnGetStr(*t, _r, "ImageN");
        rec.kImageH = ShnGetStr(*t, _r, "ImageH");
        rec.kImageE = ShnGetStr(*t, _r, "ImageE");
        rec.kDummyA = ShnGetStr(*t, _r, "DummyA");
        rec.kDummyB = ShnGetStr(*t, _r, "DummyB");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("Riding.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace fiesta
