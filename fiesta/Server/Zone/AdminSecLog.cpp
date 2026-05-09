#include "AdminSecLog.h"
#include "../Shared/ShineLogSystem.h"
#include "MapField.h"
#include <stdarg.h>
#include <ctype.h>
namespace fiesta {

// 39
void ShinePrison::Send (ShinePlayer* pk, uint32) {
    if (pk) TownPortal(pk, /*MapID*/ 100, Vec3(0,0,0)); // Prison map id placeholder (EV_VERIFY)
}
void ShinePrison::Release(ShinePlayer* pk) {
    if (pk) TownPortal(pk, /*MapID*/ 1, Vec3(0,0,0));
}
void PrisonServer::Tick() {}
// Forward decl from generated AmpersandCommands_Generated.cpp.
bool DispatchAmpersand(ShinePlayer* pk, const std::string& rLine);

bool AmpersandCommand::Execute(ShinePlayer* pk, const std::string& rLine) {
    return DispatchAmpersand(pk, rLine);
}
uint8 AdminLvSet::GetRequiredLv(const std::string& rCmd) {
    if (rCmd == "ban" || rCmd == "kick") return 4;
    if (rCmd == "tp"  || rCmd == "warp") return 3;
    return 1;
}

// 40
bool XTrap4Server::Handshake(ShinePlayer*, const uint8*, size_t) { return true; }
bool XTrap4Server::Validate (ShinePlayer*, const uint8*, size_t) { return true; }

static std::map<std::string, bool> s_kBadNames;
bool BadNameFilterData::IsBlocked(const std::string& s) {
    std::string l = s; for (size_t i = 0; i < l.size(); ++i) l[i] = (char)tolower((unsigned char)l[i]);
    return s_kBadNames.find(l) != s_kBadNames.end();
}
void BadNameFilterData::AddBlocked(const std::string& s) {
    std::string l = s; for (size_t i = 0; i < l.size(); ++i) l[i] = (char)tolower((unsigned char)l[i]);
    s_kBadNames[l] = true;
}

// 41
void EasyLogSys::Write(const char* topic, const char* fmt, ...) {
    char buf[1024]; va_list ap; va_start(ap, fmt); _vsnprintf_s(buf, sizeof(buf), _TRUNCATE, fmt, ap); va_end(ap);
    SHINELOG_INFO("[%s] %s", topic ? topic : "log", buf);
}
void MobHuntLog::Record (CharID c, MobID s) { EasyLogSys::Write("MobHunt"  , "cid=%u species=%u", c, s); }
void ItemDropLog::Record(CharID c, ItemID i, uint16 q) { EasyLogSys::Write("ItemDrop", "cid=%u inx=%u qty=%u", c, i, q); }

} // namespace fiesta
