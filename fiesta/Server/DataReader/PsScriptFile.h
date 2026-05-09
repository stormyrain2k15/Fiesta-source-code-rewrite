// Server/DataReader/PsScriptFile.h
// parser for the ".ps" DSL used by:
//   * MobBehaviorDescript/*.ps        (mob AI behavior trees)
//   * MobBehaviorDescript/KQ/*.ps     (per-KQ AI overrides)
//   * ScenarioBookShelf/<Cat>/*.ps    (scenario / dungeon scripts)
// Grammar (verbatim from the supplied files):
//   open  [<BlockName>]            ; declare block (no-name = anonymous)
//     var <Name1>. <Name2>. ...    ; declare local variables
//     <Statement>.                 ; statement; statements end with '.'
//     if <Expr> then open ... close [else open ... close]
//     infinite                     ; infinite loop wrapper around the block
//     break "<BlockName>".         ; break to named outer block
//     call "<BlockName>".          ; call another block
//     mobregen <Aggro> "<Map>" "<Area>" "<Mob>" "<Manager>".
//     chat <Handle> "<Text>".
//     pause sec <N>.
//     <Builtin> <Args...>.         ; doorbuild, dooropen, doorclose, broadcast,
//                                    linkto, npcchat, scriptfile, effectobj, ...
//   close
// Comments start with ';' (single-line). Strings are double-quoted. Numbers
// are signed integers. Identifiers can include '_' '%' '@' (rare).
// This module produces a syntax tree (PsBlock / PsStmt / PsExpr) suitable for
// downstream interpretation. We do NOT execute the tree here; that lives with
// the AI behavior runtime (MobBehaviorScript) and scenario runtime
// (ScenarioScript).
//   MobBehaviorDescript/DefaultBehavior.ps,
//   MobBehaviorDescript/KQ/KingSlime.ps,
//   ScenarioBookShelf/Promote/JobChange1.ps.
#ifndef FIESTA_DATAREADER_PSSCRIPTFILE_H
#define FIESTA_DATAREADER_PSSCRIPTFILE_H
#include "../Shared/ShineTypes.h"
#include <string>
#include <vector>

namespace fiesta {

enum PsTokenKind {
    PSTK_EOF = 0,
    PSTK_IDENT,        // alpha (_a-zA-Z) followed by [_a-zA-Z0-9-]
    PSTK_NUMBER,       // [-+]?[0-9]+ (we keep the raw text)
    PSTK_STRING,       // double-quoted literal (escapes preserved as-is)
    PSTK_LBRACK,       // [
    PSTK_RBRACK,       // ]
    PSTK_DOT,          // . (statement terminator)
    PSTK_PERCENT,      // % (string-concat operator)
    PSTK_AT,           // @ (function call: @CharClass(...))
    PSTK_LPAREN,       // (
    PSTK_RPAREN,       // )
    PSTK_COMMA,        // ,
    PSTK_OP            // operators: == === =!= != < > <= >= + - * / =
};

struct PsToken {
    PsTokenKind eKind;
    std::string kText;      // exact source text of the token (without quotes for strings)
    int         iLine;
};

struct PsExpr {
    // Flattened expression -- we keep the raw token sequence for the
    // condition. Interpretation is deferred to the runtime (which compares
    // the tokens against its variable / built-in state).
    std::vector<PsToken> kTokens;
};

struct PsStmt {
    enum Kind {
        K_VAR_DECL,         // var X "default" Y "default".
        K_ASSIGN,           // X = expr.
        K_BUILTIN,          // verb arg arg arg.
        K_IF,               // if expr then open(...) [else open(...)]
        K_INFINITE,         // infinite ... (followed by an open block)
        K_BREAK,            // break "name".
        K_CALL              // call "name".
    } eKind;

    std::string              kVerb;            // for K_BUILTIN / K_CALL / K_BREAK / K_ASSIGN
    std::vector<PsExpr>      kArgs;            // arg expressions
    PsExpr                   kCond;            // for K_IF / K_ASSIGN-rhs
    int                      iLine;
    int                      iThenBlock;       // index into PsScriptFile::m_kBlocks (-1 if none)
    int                      iElseBlock;
    int                      iBodyBlock;       // for K_INFINITE
};

struct PsBlock {
    std::string           kName;       // empty = anonymous
    std::vector<PsStmt>   kStmts;
    int                   iLine;
};

class PsScriptFile {
public:
    bool Load(const std::string& rPath);

    size_t            BlockCount() const { return m_kBlocks.size(); }
    const PsBlock&    Block(size_t i) const { return m_kBlocks[i]; }
    const PsBlock*    FindBlock(const std::string& rName) const;
    const std::vector<int>& TopLevelBlocks() const { return m_kTopLevel; }

private:
    std::vector<PsBlock>  m_kBlocks;
    std::vector<int>      m_kTopLevel; // indices into m_kBlocks (declared at file scope)

    // Tokenizer state.
    std::vector<PsToken>  m_kToks;
    size_t                m_uiTok;
    std::string           m_kPath;

    bool   Tokenize(const std::string& rSrc);
    bool   Parse();
    int    ParseBlock(const std::string& rName, int iLine); // returns block index
    bool   ParseStmt(PsBlock& rOwner);
    bool   ParseExpressionUntil(PsExpr& rOut, PsTokenKind eStop1, PsTokenKind eStop2);

    const PsToken& Peek(size_t off = 0) const;
    const PsToken& Eat();
    bool Match(PsTokenKind k);
    bool MatchIdent(const char* sz);
    bool ExpectIdent(const char* sz);
    bool Expect(PsTokenKind k);
};

} // namespace fiesta
#endif
