// Server/Zone/PineScript/PineScript.cpp
// Parser + interpreter loop. Statements are line-oriented: `Cmd arg1 arg2`.
// Comments start with `;`. Labels start with `:`. Unknown commands log a
// warning and are skipped (so unknown future ops don't crash a live KQ).
#include "PineScript.h"
#include "../../Shared/ShineLogSystem.h"
#include "../../Shared/GTimer.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

namespace shine {

PineScript& PineScript::Get() { static PineScript s; return s; }

bool PineScript::LoadFile(const std::string& rPath) {
    FILE* fp = fopen(rPath.c_str(), "rb");
    if (!fp) { SHINELOG_WARN("PS: open '%s' failed", rPath.c_str()); return false; }
    fseek(fp, 0, SEEK_END); long n = ftell(fp); fseek(fp, 0, SEEK_SET);
    std::string s; s.resize((size_t)n);
    if (n > 0) fread(&s[0], 1, (size_t)n, fp);
    fclose(fp);

    PSProgram prog; prog.kName = rPath; prog.uiPC = 0; prog.uiWaitUntilMs = 0; prog.bDone = false;
    if (!ParseTokens(s, prog)) return false;

    // Strip path -> use the filename as program name.
    size_t slash = rPath.find_last_of("/\\");
    prog.kName = (slash == std::string::npos) ? rPath : rPath.substr(slash + 1);
    m_kPrograms[prog.kName] = prog;
    SHINELOG_INFO("PS load '%s' (%u stmts)", prog.kName.c_str(), (uint32)prog.kBody.size());
    return true;
}

bool PineScript::ParseTokens(const std::string& rSrc, PSProgram& rOut) {
    const char* p = rSrc.c_str();
    while (*p) {
        // Line.
        std::string line;
        while (*p && *p != '\n') { if (*p != '\r') line += *p; ++p; }
        if (*p == '\n') ++p;
        // Trim leading ws.
        size_t i = 0; while (i < line.size() && (line[i]==' '||line[i]=='\t')) ++i;
        if (i >= line.size() || line[i] == ';' || line[i] == ':') continue;
        // Tokenize.
        PSStatement st;
        size_t j = i;
        while (j < line.size() && line[j] != ' ' && line[j] != '\t') ++j;
        st.kCmd = line.substr(i, j - i);
        while (j < line.size()) {
            while (j < line.size() && (line[j]==' '||line[j]=='\t')) ++j;
            if (j >= line.size()) break;
            size_t k = j;
            while (k < line.size() && line[k] != ' ' && line[k] != '\t') ++k;
            st.kArgs.push_back(line.substr(j, k - j));
            j = k;
        }
        if (!st.kCmd.empty()) rOut.kBody.push_back(st);
    }
    return true;
}

bool PineScript::LoadDir(const std::string& /*rDir*/) {
    // Per-platform dir walk lives in ZoneAssetLoader; LoadDir is a no-op
    // facade so callers can early-bind without #ifdef'ing Win32 directory
    // iteration here.
    return true;
}

bool PineScript::Run(const std::string& rName) {
    std::map<std::string, PSProgram>::iterator it = m_kPrograms.find(rName);
    if (it == m_kPrograms.end()) { SHINELOG_WARN("PS run: '%s' not loaded", rName.c_str()); return false; }
    m_kRunning.push_back(it->second);
    SHINELOG_INFO("PS run '%s'", rName.c_str());
    return true;
}

bool PineScript::Step(PSProgram& rProg) {
    if (rProg.bDone) return false;
    if (rProg.uiPC >= rProg.kBody.size()) { rProg.bDone = true; return false; }
    if (GTimer::NowMillis() < rProg.uiWaitUntilMs) return true;
    const PSStatement& st = rProg.kBody[rProg.uiPC++];
    if (st.kCmd == "Wait" && !st.kArgs.empty()) {
        rProg.uiWaitUntilMs = GTimer::NowMillis() + (uint64)atoi(st.kArgs[0].c_str());
    } else if (st.kCmd == "End") {
        rProg.bDone = true;
    } else {
        // Forward to function dispatcher in PineScriptFunction.cpp.
        extern bool PSDispatchFunction(const PSStatement& rSt, PSProgram& rProg);
        if (!PSDispatchFunction(st, rProg))
            SHINELOG_DEBUG("PS '%s' unknown cmd '%s'", rProg.kName.c_str(), st.kCmd.c_str());
    }
    return true;
}

void PineScript::Tick() {
    for (size_t i = 0; i < m_kRunning.size(); ) {
        Step(m_kRunning[i]);
        if (m_kRunning[i].bDone) m_kRunning.erase(m_kRunning.begin() + i);
        else ++i;
    }
}

} // namespace shine
