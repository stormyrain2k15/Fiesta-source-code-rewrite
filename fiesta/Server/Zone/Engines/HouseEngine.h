// Server/Zone/Engines/HouseEngine.h
// HouseEngine — aggregates 7 SHN loader(s).
#ifndef SHINE_ZONE_ENGINES_HOUSEENGINE_H
#define SHINE_ZONE_ENGINES_HOUSEENGINE_H
#include "../../DataReader/SHN/CharacterTitleStateServer.h"
#include "../../DataReader/SHN/MiniHouse.h"
#include "../../DataReader/SHN/MiniHouseDummy.h"
#include "../../DataReader/SHN/MiniHouseEndure.h"
#include "../../DataReader/SHN/MiniHouseFurniture.h"
#include "../../DataReader/SHN/MiniHouseFurnitureObjEffect.h"
#include "../../DataReader/SHN/MiniHouseObjAni.h"
#include <vector>
#include <string>

namespace shine {

class HouseEngine {
public:
    static HouseEngine& Get();
    void Bind();

    // Per-SHN accessors
    CharacterTitleStateServerShn& characterTitleStateServer() { return CharacterTitleStateServerShn::Get(); }
    MiniHouseShn& miniHouse() { return MiniHouseShn::Get(); }
    MiniHouseDummyShn& miniHouseDummy() { return MiniHouseDummyShn::Get(); }
    MiniHouseEndureShn& miniHouseEndure() { return MiniHouseEndureShn::Get(); }
    MiniHouseFurnitureShn& miniHouseFurniture() { return MiniHouseFurnitureShn::Get(); }
    MiniHouseFurnitureObjEffectShn& miniHouseFurnitureObjEffect() { return MiniHouseFurnitureObjEffectShn::Get(); }
    MiniHouseObjAniShn& miniHouseObjAni() { return MiniHouseObjAniShn::Get(); }

private:
    HouseEngine() {}
};

} // namespace shine
#endif // SHINE_ZONE_ENGINES_HOUSEENGINE_H
