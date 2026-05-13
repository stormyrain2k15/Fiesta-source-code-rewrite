// Client/ResSystem/CharacterLoader.cpp
#include "CharacterLoader.h"
#include "../Engine/ShineConfig.h"
#include "../../Server/Shared/ShineLogSystem.h"

// Pulls from CharEngine if available (client build with engine split)
#ifdef SHINE_CLIENT
#include "../../Server/Zone/Engines/CharEngine.h"
#endif

namespace shine {

CharacterLoader& CharacterLoader::Get() {
    static CharacterLoader s; return s;
}

std::string CharacterLoader::ResolveClassInx(uint16 uiClass) const {
#ifdef SHINE_CLIENT
    const ClassNameRow* pRow = ClassNameShn::Get().FindById(uiClass);
    if (pRow) return pRow->kName;
#endif
    // Fallback table for classes we know
    switch (uiClass) {
    case 0:  return "Fighter";
    case 1:  return "Cleric";
    case 2:  return "Archer";
    case 3:  return "Mage";
    case 4:  return "Trickster";
    case 10: return "HolyKnight";
    case 11: return "Paladin";
    case 12: return "Ranger";
    case 13: return "WizMage";
    case 14: return "Rogue";
    default: return "";
    }
}

CharacterAssets CharacterLoader::LoadPlayer(uint16 uiClass, CharGender eGender,
                                              ShineScene* pkScene) {
    std::string kBase = ResolveClassInx(uiClass);
    if (kBase.empty()) {
        SHINELOG_WARN("CharacterLoader: unknown class %u", uiClass);
        CharacterAssets empty; return empty;
    }

    // Gender suffix: Archer-M / Archer-F
    std::string kGendered = kBase + (eGender == GENDER_FEMALE ? "-F" : "-M");
    return LoadInternal(kGendered, kBase, pkScene);
}

CharacterAssets CharacterLoader::LoadActor(const std::string& rInxName,
                                            ShineScene* pkScene) {
    return LoadInternal(rInxName, rInxName, pkScene);
}

CharacterAssets CharacterLoader::LoadInternal(const std::string& rGenderedInx,
                                               const std::string& rBaseInx,
                                               ShineScene* pkScene) {
    CharacterAssets assets;
    assets.kInxName      = rBaseInx;
    assets.kGenderedInx  = rGenderedInx;

    const ShineConfig& cfg = ShineConfig::Get();

    // ── Body SGA path ─────────────────────────────────────────────────────────
    // reschar\<GenderedInx>\body.sga
    assets.kBodySGAPath = cfg.kResChar + "\\" + rGenderedInx + "\\body.sga";

    // ── Action DAT ────────────────────────────────────────────────────────────
    // Try gendered first (Archer-M.dat), fall back to base (Archer.dat)
    const ActionDat* pkDat = ActionDatCache::Get().Acquire(rGenderedInx, cfg.kResAction);
    if (!pkDat)
        pkDat = ActionDatCache::Get().Acquire(rBaseInx, cfg.kResAction);

    if (!pkDat) {
        SHINELOG_WARN("CharacterLoader: no action dat for '%s'", rGenderedInx.c_str());
    }
    assets.pkActionDat = pkDat;

    // ── Load body into scene ──────────────────────────────────────────────────
    if (pkScene) {
        assets.pkNode = pkScene->LoadSGA(assets.kBodySGAPath);
        if (!assets.pkNode) {
            SHINELOG_WARN("CharacterLoader: body SGA not found '%s'",
                          assets.kBodySGAPath.c_str());
        }
    }

    assets.bLoaded = (assets.pkNode != NULL && assets.pkActionDat != NULL);

    if (assets.bLoaded) {
        SHINELOG_INFO("CharacterLoader: loaded '%s' with %u actions",
                      rGenderedInx.c_str(),
                      assets.pkActionDat ? assets.pkActionDat->Count() : 0);
    }

    return assets;
}

} // namespace shine
