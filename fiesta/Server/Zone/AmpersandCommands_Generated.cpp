// Server/Zone/AmpersandCommands_Generated.cpp
#include "AmpersandCommands_Generated.h"
#include "../Shared/ShineLogSystem.h"
namespace fiesta {
// Default handler: log and succeed. Per-command bodies wire in pass 2.
static bool Cmd_default(ShinePlayer* pk, const std::vector<std::string>& a) {
    SHINELOG_INFO("&cmd by %s args=%u", pk?pk->GetName().c_str():"?", (uint32)a.size());
    return true;
}
const AmpersandCmdEntry kAmpersandCmds[] = {
    { "&hide", 10, &Cmd_default },
    { "&show", 10, &Cmd_default },
    { "&pfind", 10, &Cmd_default },
    { "&mfind", 10, &Cmd_default },
    { "&kickout", 10, &Cmd_default },
    { "&linkto", 10, &Cmd_default },
    { "&psummon", 10, &Cmd_default },
    { "&expbonus", 50, &Cmd_default },
    { "&itembonus", 50, &Cmd_default },
    { "&fieldbonus", 50, &Cmd_default },
    { "&silence", 100, &Cmd_default },
    { "&distribute", 10, &Cmd_default },
    { "&isblock", 10, &Cmd_default },
    { "&mobdist", 10, &Cmd_default },
    { "&chargeditem", 100, &Cmd_default },
    { "&parameterview", 100, &Cmd_default },
    { "&adminlevel", 1, &Cmd_default },
    { "&serverindex", 100, &Cmd_default },
    { "&makeitem", 100, &Cmd_default },
    { "&cleariv", 1, &Cmd_default },
    { "&dimension", 100, &Cmd_default },
    { "&doorbuild", 100, &Cmd_default },
    { "&mobbreed", 50, &Cmd_default },
    { "&mb", 50, &Cmd_default },
    { "&fullpoint", 50, &Cmd_default },
    { "&summonplayer", 100, &Cmd_default },
    { "&performance", 10, &Cmd_default },
    { "&sethp", 100, &Cmd_default },
    { "&setsp", 100, &Cmd_default },
    { "&dist", 10, &Cmd_default },
    { "&coord", 10, &Cmd_default },
    { "&scrint", 10, &Cmd_default },
    { "&timer", 50, &Cmd_default },
    { "&templink", 10, &Cmd_default },
    { "&oxblock", 1, &Cmd_default },
    { "&z", 10, &Cmd_default },
    { "&gmchat", 1, &Cmd_default },
    { "&gldcollectclear", 100, &Cmd_default },
    { "&gldcollectset", 100, &Cmd_default },
    { "&questgiveup", 100, &Cmd_default },
    { "&questrecieve", 100, &Cmd_default },
    { "&questcomplete", 100, &Cmd_default },
    { "&learnskill", 100, &Cmd_default },
    { "&skillcooltime", 100, &Cmd_default },
    { "&setabstate", 100, &Cmd_default },
    { "&resetabstate", 100, &Cmd_default },
    { "&setabstatetarget", 100, &Cmd_default },
    { "&resetabstatetarget", 100, &Cmd_default },
    { "&kqreward", 100, &Cmd_default },
    { "&killallmob", 50, &Cmd_default },
    { "&mobdamage", 100, &Cmd_default },
    { "&suicideallmob", 50, &Cmd_default },
    { "&suicidemob", 100, &Cmd_default },
    { "&kingdomquest", 100, &Cmd_default },
    { "&Reroadkingdomquest", 100, &Cmd_default },
    { "&endofkq", 100, &Cmd_default },
    { "&guildtournament", 100, &Cmd_default },
    { "&zonelink", 100, &Cmd_default },
    { "&block", 100, &Cmd_default },
    { "&emptymap", 100, &Cmd_default },
    { "&serversocketcut", 100, &Cmd_default },
    { "&staticdamage", 50, &Cmd_default },
    { "&deathaura", 100, &Cmd_default },
    { "&immortal", 10, &Cmd_default },
    { "&enterminihouse", 100, &Cmd_default },
    { "&horsespeed", 100, &Cmd_default },
    { "&charinform", 10, &Cmd_default },
    { "&partyinfo", 100, &Cmd_default },
    { "&propose", 100, &Cmd_default },
    { "&proposeres", 100, &Cmd_default },
    { "&weddingreset", 100, &Cmd_default },
    { "&reserve", 100, &Cmd_default },
    { "&enterready", 100, &Cmd_default },
    { "&enter", 100, &Cmd_default },
    { "&learnpassive", 100, &Cmd_default },
    { "&unlearn", 100, &Cmd_default },
    { "&unlearnpassive", 100, &Cmd_default },
    { "&wedding", 100, &Cmd_default },
    { "&showid", 100, &Cmd_default },
    { "&mobhatchery", 100, &Cmd_default },
    { "&allcritical", 100, &Cmd_default },
    { "&erasechargebuf", 100, &Cmd_default },
    { "&startGT", 100, &Cmd_default },
    { "&flagcapture", 100, &Cmd_default },
    { "&useguildskill", 100, &Cmd_default },
    { "&observerlinkto", 100, &Cmd_default },
    { "&observerout", 100, &Cmd_default },
    { "&HP", 100, &Cmd_default },
    { "&SP", 100, &Cmd_default },
    { "&speed", 50, &Cmd_default },
    { "&linktosavedata", 100, &Cmd_default },
    { "&reloaddata", 100, &Cmd_default },
    { "&captivate", 100, &Cmd_default },
    { "&ignoreskillcooltime", 100, &Cmd_default },
    { "&invenview", 100, &Cmd_default },
    { "&runto", 100, &Cmd_default },
    { "&ghostremove", 100, &Cmd_default },
    { "&curtime", 100, &Cmd_default },
    { "&amibusy", 100, &Cmd_default },
    { "&knockback", 100, &Cmd_default },
    { "&partblock", 100, &Cmd_default },
    { "&debuglogging", 100, &Cmd_default },
    { "&&&", 100, &Cmd_default },
    { "&npcrevive", 100, &Cmd_default },
    { "&ts_DiceFix", 100, &Cmd_default },
    { "&ts_DiceProbability", 100, &Cmd_default },
    { "&GB_DiceFix", 100, &Cmd_default },
    { "&GB_DiceProbability", 100, &Cmd_default },
    { "&makeraid", 100, &Cmd_default },
    { "&outraid", 100, &Cmd_default },
    { "&inraid", 100, &Cmd_default },
    { "&PlayIns", 100, &Cmd_default },
    { "&bigbang", 100, &Cmd_default },
    { "&alllearnskill", 100, &Cmd_default },
    { "&levelup", 100, &Cmd_default },
    { "&showallchar", 100, &Cmd_default },
    { "&testlink", 50, &Cmd_default },
    { "&gtieffect", 50, &Cmd_default },
    { "&iareset", 50, &Cmd_default },
    { "&queststart", 50, &Cmd_default },
    { "&questread", 50, &Cmd_default },
    { "&disposition", 50, &Cmd_default },
    { "&idtimechange", 50, &Cmd_default },
    { "&releasechatban", 50, &Cmd_default },
    { "&slotmachine", 1, &Cmd_default },
    { "&godofslotmachine", 100, &Cmd_default },
    { "&viewaggrolist", 50, &Cmd_default },
    { "&ebcu", 50, &Cmd_default },
    { "&luaobserver", 100, &Cmd_default },
    { "&luafuncexec", 100, &Cmd_default },
    { "&mobitemdrop", 100, &Cmd_default },
    { "&getmoney", 100, &Cmd_default },
    { "&fnpc", 100, &Cmd_default },
    { "&RegenNPC", 100, &Cmd_default },
    { "&ReleaseNPC", 100, &Cmd_default },
    { "&gmtestmode", 100, &Cmd_default },
    { "&arrangeiv", 100, &Cmd_default },
    { "&cmt", 100, &Cmd_default },
    { "&reloadid", 100, &Cmd_default },
    { "&vie", 100, &Cmd_default },
    { "&viue", 100, &Cmd_default },
    { "&viuea", 100, &Cmd_default },
    { "&setlp", 100, &Cmd_default },
    { "&movelayer", 100, &Cmd_default },
    { "&moveotherlayer", 100, &Cmd_default },
    { "&godmode", 100, &Cmd_default },
    { "&fieldmobcount", 100, &Cmd_default },
    { "&reloadpetlua", 100, &Cmd_default },
    { "&getpetinfo", 100, &Cmd_default },
    { "&setpetinfo", 100, &Cmd_default },
    { "&vanishdropitem", 100, &Cmd_default },
    { "&walkto", 100, &Cmd_default },
    { "&castteleport", 100, &Cmd_default },
    { "&setobjectdirect", 100, &Cmd_default },
    { 0, 0, 0 }
};
const size_t kAmpersandCmdCount = 154;

static std::vector<std::string> Tokenize(const std::string& s) {
    std::vector<std::string> v; std::string cur;
    for (size_t i = 0; i < s.size(); ++i) {
        char c = s[i];
        if (c == ' ' || c == '\t') { if (!cur.empty()) { v.push_back(cur); cur.clear(); } }
        else cur.push_back(c);
    }
    if (!cur.empty()) v.push_back(cur);
    return v;
}

bool DispatchAmpersand(ShinePlayer* pk, const std::string& rLine) {
    if (rLine.empty() || rLine[0] != '&') return false;
    std::vector<std::string> tok = Tokenize(rLine);
    if (tok.empty()) return false;
    const std::string& head = tok[0];
    for (size_t i = 0; i < kAmpersandCmdCount; ++i) {
        if (head == kAmpersandCmds[i].szName) {
            // Admin-level gate would compare against pk->GetAdminLv() in pass 2.
            std::vector<std::string> args(tok.begin()+1, tok.end());
            return kAmpersandCmds[i].pkFn(pk, args);
        }
    }
    SHINELOG_WARN("Unknown ampersand command: %s", head.c_str());
    return false;
}
} // namespace fiesta