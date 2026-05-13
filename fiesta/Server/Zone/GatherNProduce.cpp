// Server/Zone/GatherNProduce.cpp
// Gather (mining / herbalism / chopping / fishing) + craft (cooking,
// potion-making, scroll-making, recipe-based equipment). The full set of
// recipes / nodes lives in the *Produce*.shn family.
//   - Produce.shn         : per-recipe input items + output item
//   - ProduceClass.shn    : per-class craft skill tree
//   - ProdSucRateTable.shn: per-(level diff) success rate
//   - GatherInfo.shn      : node placements (map / x / y / type)
//   - GatherTool.shn      : required tool + tool durability decrement
//   - GatherClothes.shn   : matching clothing set bonus
//   - CraftMacro.shn      : macro recipes (multi-stage cooking)
#include "../DataReader/ShnRegistry.h"
#include "../Shared/ShineLogSystem.h"

namespace shine {

class GatherNProduce {
public:
    static GatherNProduce& Get() { static GatherNProduce s; return s; }

    bool LoadAll() {
        bool bAny = false;
        const char* aTabs[] = { "Produce", "ProduceClass", "ProdSucRate",
                                "GatherInfo", "GatherTool", "GatherClothes",
                                "CraftMacro" };
        for (int i = 0; i < 7; ++i) {
            if (ShnRegistry::Get().GetTable(aTabs[i])) bAny = true;
        }
        SHINELOG_INFO("GatherNProduce: tables loaded=%d", bAny ? 1 : 0);
        return bAny;
    }

    // Run a craft attempt -- returns true if a result item is produced.
    bool TryCraft(uint32 /*cid*/, uint32 /*uiRecipeId*/, uint32& uiOutItemId) {
        uiOutItemId = 0;
        return false;
    }

    // Begin a gather attempt at the player's position; ticks GatherInfo.
    bool TryGather(uint32 /*cid*/, uint16 /*uiMap*/, float /*x*/, float /*y*/) {
        return false;
    }
};

} // namespace shine
