// Server/DataReader/PsScriptFile.cpp
#include "PsScriptFile.h"
#include "../Shared/ShineLogSystem.h"
#include <stdio.h>
#include <ctype.h>
#include <string.h>

namespace fiesta {

// ---------------- Tokenizer ----------------
bool PsScriptFile::Tokenize(const std::string& rSrc) {
    m_kToks.clear();
    int line = 1;
    size_t i = 0;
    while (i < rSrc.size()) {
        char c = rSrc[i];
        if (c == '\n') { ++line; ++i; continue; }
        if (c == '\r' || c == ' ' || c == '\t') { ++i; continue; }
        if (c == ';') { // comment to end of line
            while (i < rSrc.size() && rSrc[i] != '\n') ++i;
            continue;
        }
        if (c == '"') {
            size_t j = i + 1; std::string s;
            while (j < rSrc.size() && rSrc[j] != '"') {
                if (rSrc[j] == '\n') ++line;
                s.push_back(rSrc[j]); ++j;
            }
            PsToken t; t.eKind = PSTK_STRING; t.kText = s; t.iLine = line;
            m_kToks.push_back(t);
            i = (j < rSrc.size()) ? j + 1 : j;
            continue;
        }
        if (c == '[') { PsToken t; t.eKind = PSTK_LBRACK; t.kText = "["; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        if (c == ']') { PsToken t; t.eKind = PSTK_RBRACK; t.kText = "]"; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        if (c == '.') { PsToken t; t.eKind = PSTK_DOT;    t.kText = "."; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        if (c == '%') { PsToken t; t.eKind = PSTK_PERCENT;t.kText = "%"; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        if (c == '@') { PsToken t; t.eKind = PSTK_AT;     t.kText = "@"; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        if (c == '(') { PsToken t; t.eKind = PSTK_LPAREN; t.kText = "("; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        if (c == ')') { PsToken t; t.eKind = PSTK_RPAREN; t.kText = ")"; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        if (c == ',') { PsToken t; t.eKind = PSTK_COMMA;  t.kText = ","; t.iLine = line; m_kToks.push_back(t); ++i; continue; }
        // Operator runs.
        if (c == '=' || c == '<' || c == '>' || c == '!' || c == '+' || c == '-' || c == '*' || c == '/') {
            // Numbers: -123 / +456 if next is a digit and previous token is *not* an operand.
            if ((c == '-' || c == '+') && i + 1 < rSrc.size() && isdigit((unsigned char)rSrc[i+1])) {
                bool isOperator = false;
                if (!m_kToks.empty()) {
                    PsTokenKind p = m_kToks.back().eKind;
                    isOperator = (p == PSTK_OP || p == PSTK_LBRACK || p == PSTK_LPAREN || p == PSTK_COMMA || p == PSTK_DOT);
                } else isOperator = true;
                if (isOperator) {
                    size_t j = i + 1; while (j < rSrc.size() && isdigit((unsigned char)rSrc[j])) ++j;
                    PsToken t; t.eKind = PSTK_NUMBER; t.kText = rSrc.substr(i, j - i); t.iLine = line;
                    m_kToks.push_back(t); i = j; continue;
                }
            }
            size_t j = i;
            while (j < rSrc.size() && (rSrc[j] == '=' || rSrc[j] == '<' || rSrc[j] == '>' || rSrc[j] == '!' ||
                                       rSrc[j] == '+' || rSrc[j] == '-' || rSrc[j] == '*' || rSrc[j] == '/')) ++j;
            PsToken t; t.eKind = PSTK_OP; t.kText = rSrc.substr(i, j - i); t.iLine = line;
            m_kToks.push_back(t); i = j; continue;
        }
        if (isdigit((unsigned char)c)) {
            size_t j = i; while (j < rSrc.size() && isdigit((unsigned char)rSrc[j])) ++j;
            PsToken t; t.eKind = PSTK_NUMBER; t.kText = rSrc.substr(i, j - i); t.iLine = line;
            m_kToks.push_back(t); i = j; continue;
        }
        if (isalpha((unsigned char)c) || c == '_') {
            size_t j = i;
            while (j < rSrc.size() && (isalnum((unsigned char)rSrc[j]) || rSrc[j] == '_' || rSrc[j] == '-'))
                ++j;
            PsToken t; t.eKind = PSTK_IDENT; t.kText = rSrc.substr(i, j - i); t.iLine = line;
            m_kToks.push_back(t); i = j; continue;
        }
        // Skip anything else (Korean comments etc.) to avoid wedging on stray bytes.
        ++i;
    }
    return true;
}

// ---------------- Parser ----------------
const PsToken& PsScriptFile::Peek(size_t off) const {
    static PsToken kEof = { PSTK_EOF, "", 0 };
    return (m_uiTok + off < m_kToks.size()) ? m_kToks[m_uiTok + off] : kEof;
}
const PsToken& PsScriptFile::Eat() {
    static PsToken kEof = { PSTK_EOF, "", 0 };
    if (m_uiTok < m_kToks.size()) return m_kToks[m_uiTok++];
    return kEof;
}
bool PsScriptFile::Match(PsTokenKind k) {
    if (Peek().eKind != k) return false;
    Eat(); return true;
}
bool PsScriptFile::MatchIdent(const char* sz) {
    if (Peek().eKind != PSTK_IDENT) return false;
    if (_stricmp(Peek().kText.c_str(), sz) != 0) return false;
    Eat(); return true;
}
bool PsScriptFile::ExpectIdent(const char* sz) {
    if (!MatchIdent(sz)) {
        SHINELOG_WARN("PsScriptFile %s: expected '%s' near line %d (got '%s')",
                      m_kPath.c_str(), sz, Peek().iLine, Peek().kText.c_str());
        return false;
    }
    return true;
}
bool PsScriptFile::Expect(PsTokenKind k) {
    if (!Match(k)) { SHINELOG_WARN("PsScriptFile %s: expected token kind %d at line %d", m_kPath.c_str(), (int)k, Peek().iLine); return false; }
    return true;
}

bool PsScriptFile::ParseExpressionUntil(PsExpr& rOut, PsTokenKind eStop1, PsTokenKind eStop2) {
    rOut.kTokens.clear();
    int parenDepth = 0;
    while (true) {
        const PsToken& tk = Peek();
        if (tk.eKind == PSTK_EOF) return false;
        if (parenDepth == 0) {
            if (tk.eKind == eStop1 || tk.eKind == eStop2) return true;
            if (tk.eKind == PSTK_IDENT && _stricmp(tk.kText.c_str(), "then") == 0) return true;
        }
        if (tk.eKind == PSTK_LPAREN) ++parenDepth;
        else if (tk.eKind == PSTK_RPAREN) --parenDepth;
        rOut.kTokens.push_back(tk); Eat();
    }
}

bool PsScriptFile::ParseStmt(PsBlock& rOwner) {
    const PsToken& tk = Peek();
    if (tk.eKind == PSTK_EOF) return false;

    // Recognize 'close' / 'else' as terminators handled by caller -- not statements.
    if (tk.eKind == PSTK_IDENT) {
        if (_stricmp(tk.kText.c_str(), "close") == 0) return false;
        if (_stricmp(tk.kText.c_str(), "else")  == 0) return false;
        if (_stricmp(tk.kText.c_str(), "open")  == 0) {
            // Inline anonymous block (rare at statement level): treat as call-into.
            Eat();
            std::string anon; int ln = tk.iLine;
            int idx = ParseBlock(anon, ln);
            (void)idx;
            return true;
        }

        // 'var' decl.
        if (_stricmp(tk.kText.c_str(), "var") == 0) {
            Eat();
            PsStmt s; s.eKind = PsStmt::K_VAR_DECL; s.iLine = tk.iLine;
            s.iThenBlock = s.iElseBlock = s.iBodyBlock = -1;
            // Consume idents and optional defaults until '.'
            while (Peek().eKind != PSTK_DOT && Peek().eKind != PSTK_EOF) {
                PsExpr e; e.kTokens.push_back(Eat());
                // Default token (string or number) is optional.
                if (Peek().eKind == PSTK_STRING || Peek().eKind == PSTK_NUMBER) e.kTokens.push_back(Eat());
                s.kArgs.push_back(e);
            }
            Match(PSTK_DOT);
            rOwner.kStmts.push_back(s);
            return true;
        }
        // 'if' .. 'then' open ... close [else open ... close].
        if (_stricmp(tk.kText.c_str(), "if") == 0) {
            Eat();
            PsStmt s; s.eKind = PsStmt::K_IF; s.iLine = tk.iLine;
            s.iThenBlock = s.iElseBlock = s.iBodyBlock = -1;
            ParseExpressionUntil(s.kCond, PSTK_DOT, PSTK_EOF);
            ExpectIdent("then");
            ExpectIdent("open");
            s.iThenBlock = ParseBlock(std::string(), tk.iLine);
            if (MatchIdent("else")) {
                ExpectIdent("open");
                s.iElseBlock = ParseBlock(std::string(), tk.iLine);
            }
            rOwner.kStmts.push_back(s);
            return true;
        }
        // 'infinite' open ... close
        if (_stricmp(tk.kText.c_str(), "infinite") == 0) {
            Eat();
            PsStmt s; s.eKind = PsStmt::K_INFINITE; s.iLine = tk.iLine;
            s.iThenBlock = s.iElseBlock = -1;
            ExpectIdent("open");
            s.iBodyBlock = ParseBlock(std::string(), tk.iLine);
            rOwner.kStmts.push_back(s);
            return true;
        }
        // 'break "name".'
        if (_stricmp(tk.kText.c_str(), "break") == 0) {
            Eat();
            PsStmt s; s.eKind = PsStmt::K_BREAK; s.iLine = tk.iLine;
            s.iThenBlock = s.iElseBlock = s.iBodyBlock = -1;
            if (Peek().eKind == PSTK_STRING) { s.kVerb = Eat().kText; }
            Match(PSTK_DOT);
            rOwner.kStmts.push_back(s);
            return true;
        }
        // 'call "name".'
        if (_stricmp(tk.kText.c_str(), "call") == 0) {
            Eat();
            PsStmt s; s.eKind = PsStmt::K_CALL; s.iLine = tk.iLine;
            s.iThenBlock = s.iElseBlock = s.iBodyBlock = -1;
            if (Peek().eKind == PSTK_STRING) { s.kVerb = Eat().kText; }
            Match(PSTK_DOT);
            rOwner.kStmts.push_back(s);
            return true;
        }

        // Generic verb statement: <ident> <args...>.
        // Distinguish from assignment (X = expr.) by looking ahead.
        // Save the leading ident.
        const PsToken& head = Eat();
        // Assignment: ident '=' expr '.'
        if (Peek().eKind == PSTK_OP && Peek().kText == "=") {
            Eat();
            PsStmt s; s.eKind = PsStmt::K_ASSIGN; s.iLine = head.iLine;
            s.iThenBlock = s.iElseBlock = s.iBodyBlock = -1;
            s.kVerb = head.kText;
            ParseExpressionUntil(s.kCond, PSTK_DOT, PSTK_EOF);
            Match(PSTK_DOT);
            rOwner.kStmts.push_back(s);
            return true;
        }
        // Builtin / verb form.
        PsStmt s; s.eKind = PsStmt::K_BUILTIN; s.iLine = head.iLine;
        s.iThenBlock = s.iElseBlock = s.iBodyBlock = -1;
        s.kVerb = head.kText;
        // Each argument runs until the next whitespace-delimited "atom"; we
        // treat tokens up to the '.' as a list of single-token args (strings,
        // numbers, idents, %, @).
        while (Peek().eKind != PSTK_DOT && Peek().eKind != PSTK_EOF) {
            // Group %-concat / @func(...) chains into one PsExpr.
            PsExpr e;
            int parenDepth = 0;
            while (Peek().eKind != PSTK_DOT && Peek().eKind != PSTK_EOF) {
                const PsToken& t2 = Peek();
                if (parenDepth == 0) {
                    // Continuation tokens that "stick" to the current expression.
                    if (t2.eKind == PSTK_PERCENT || t2.eKind == PSTK_OP) {
                        e.kTokens.push_back(Eat()); continue;
                    }
                    // If we already have an operand and the next is also an
                    // operand without a connecting % / op, this is the start
                    // of the *next* argument.
                    if (!e.kTokens.empty() &&
                        (t2.eKind == PSTK_IDENT || t2.eKind == PSTK_STRING ||
                         t2.eKind == PSTK_NUMBER || t2.eKind == PSTK_AT)) {
                        // Special case: '@CharClass' '(' ... ')' is one expr.
                        if (e.kTokens.back().eKind == PSTK_AT) { e.kTokens.push_back(Eat()); continue; }
                        if (e.kTokens.back().eKind == PSTK_PERCENT || e.kTokens.back().eKind == PSTK_OP) {
                            e.kTokens.push_back(Eat()); continue;
                        }
                        break;
                    }
                }
                if (t2.eKind == PSTK_LPAREN) ++parenDepth;
                else if (t2.eKind == PSTK_RPAREN) --parenDepth;
                e.kTokens.push_back(Eat());
            }
            if (e.kTokens.empty()) break;
            s.kArgs.push_back(e);
        }
        Match(PSTK_DOT);
        rOwner.kStmts.push_back(s);
        return true;
    }
    // Skip unexpected tokens at statement boundary (resilient parse).
    Eat();
    return true;
}

int PsScriptFile::ParseBlock(const std::string& rName, int iLine) {
    int idx = (int)m_kBlocks.size();
    PsBlock blk; blk.kName = rName; blk.iLine = iLine;
    m_kBlocks.push_back(blk);
    // Optional [name] after 'open' (caller didn't consume it).
    if (m_kBlocks[idx].kName.empty() && Match(PSTK_LBRACK)) {
        if (Peek().eKind == PSTK_IDENT || Peek().eKind == PSTK_STRING) {
            m_kBlocks[idx].kName = Eat().kText;
        }
        Match(PSTK_RBRACK);
    }
    while (Peek().eKind != PSTK_EOF) {
        if (Peek().eKind == PSTK_IDENT && _stricmp(Peek().kText.c_str(), "close") == 0) {
            Eat(); break;
        }
        if (!ParseStmt(m_kBlocks[idx])) {
            // Statement parser said "stop"; consume close if present.
            if (Peek().eKind == PSTK_IDENT && _stricmp(Peek().kText.c_str(), "close") == 0) Eat();
            break;
        }
    }
    return idx;
}

bool PsScriptFile::Parse() {
    m_uiTok = 0;
    m_kBlocks.clear();
    m_kTopLevel.clear();
    while (Peek().eKind != PSTK_EOF) {
        if (Peek().eKind == PSTK_IDENT && _stricmp(Peek().kText.c_str(), "open") == 0) {
            int ln = Peek().iLine;
            Eat();
            int idx = ParseBlock(std::string(), ln);
            m_kTopLevel.push_back(idx);
        } else {
            // Tolerate stray tokens at file scope.
            Eat();
        }
    }
    return true;
}

bool PsScriptFile::Load(const std::string& rPath) {
    m_kPath = rPath;
    FILE* fp = NULL; fopen_s(&fp, rPath.c_str(), "rb");
    if (!fp) { SHINELOG_ERROR("PsScriptFile: cannot open %s", rPath.c_str()); return false; }
    fseek(fp, 0, SEEK_END); long n = ftell(fp); fseek(fp, 0, SEEK_SET);
    std::string src; if (n > 0) { src.resize((size_t)n); fread(&src[0], 1, (size_t)n, fp); }
    fclose(fp);
    if (!Tokenize(src)) return false;
    if (!Parse())       return false;
    SHINELOG_INFO("PsScriptFile '%s': blocks=%u top=%u", rPath.c_str(),
                  (uint32)m_kBlocks.size(), (uint32)m_kTopLevel.size());
    return true;
}

const PsBlock* PsScriptFile::FindBlock(const std::string& rName) const {
    for (size_t i = 0; i < m_kBlocks.size(); ++i)
        if (m_kBlocks[i].kName == rName) return &m_kBlocks[i];
    return NULL;
}

} // namespace fiesta
