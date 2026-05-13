// Server/DataReader/SHN/Produce.cpp
// Auto-generated: one-file-per-SHN split for Produce.shn
#include "Produce.h"
#include "../../Shared/ShineLogSystem.h"

namespace shine {

ProduceShn& ProduceShn::Get() { static ProduceShn s; return s; }

void ProduceShn::Load() {
    const ShnFile* t = ShnRegistry::Get().GetTable("Produce");
    if (!t) { SHINELOG_WARN("Produce.shn missing"); return; }
    m_kRows.reserve(t->Rows().size());
    for (size_t _r = 0; _r < t->Rows().size(); ++_r) {
        ProduceRow rec;
        rec.uiProductID = (uint16)ShnGetU32(*t, _r, "ProductID");
        rec.kProduceIndex = ShnGetStr(*t, _r, "ProduceIndex");
        rec.kName = ShnGetStr(*t, _r, "Name");
        rec.kProduct = ShnGetStr(*t, _r, "Product");
        rec.uiLot = ShnGetU32(*t, _r, "Lot");
        rec.kRaw0 = ShnGetStr(*t, _r, "Raw0");
        rec.uiQuantity0 = ShnGetU32(*t, _r, "Quantity0");
        rec.kRaw1 = ShnGetStr(*t, _r, "Raw1");
        rec.uiQuantity1 = ShnGetU32(*t, _r, "Quantity1");
        rec.kRaw2 = ShnGetStr(*t, _r, "Raw2");
        rec.uiQuantity2 = ShnGetU32(*t, _r, "Quantity2");
        rec.kRaw3 = ShnGetStr(*t, _r, "Raw3");
        rec.uiQuantity3 = ShnGetU32(*t, _r, "Quantity3");
        rec.kRaw4 = ShnGetStr(*t, _r, "Raw4");
        rec.uiQuantity4 = ShnGetU32(*t, _r, "Quantity4");
        rec.kRaw5 = ShnGetStr(*t, _r, "Raw5");
        rec.uiQuantity5 = ShnGetU32(*t, _r, "Quantity5");
        rec.kRaw6 = ShnGetStr(*t, _r, "Raw6");
        rec.uiQuantity6 = ShnGetU32(*t, _r, "Quantity6");
        rec.kRaw7 = ShnGetStr(*t, _r, "Raw7");
        rec.uiQuantity7 = ShnGetU32(*t, _r, "Quantity7");
        rec.uiMasteryType = ShnGetU32(*t, _r, "MasteryType");
        rec.uiMasteryGain = ShnGetU32(*t, _r, "MasteryGain");
        rec.uiNeededMasteryType = ShnGetU32(*t, _r, "NeededMasteryType");
        rec.uiNeededMasteryGain = ShnGetU32(*t, _r, "NeededMasteryGain");
        m_kRows.push_back(rec);
    }
    SHINELOG_INFO("Produce.shn: %u rows", (uint32)m_kRows.size());
}

} // namespace shine
