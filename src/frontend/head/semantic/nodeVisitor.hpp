#pragma once

#include "scope.hpp"
#include "astToSymbolConverter.hpp"
#include "symbol.hpp"
#include "scopeRange.hpp"
#include <memory>
#include <vector>

class NodeVisitor
{
    AstToSymbolConverter symbolCreater;
    std::vector<Node> astLines;
    Scope globalScope = Scope("global", nullptr, {});
    std::vector<Scope> scopes;

public:

    void setAstLines(std::vector<Node> newAstLines)
    {
        astLines = newAstLines;
    }

    std::optional<Symbol> visitNode(Node visitingNode, std::vector<Symbol> declaratedSymbols);

    std::optional<ScopeRange> findScopeRange(std::vector<Node>::const_iterator startFrom, std::vector<Node>::const_iterator endOfAst);

    std::optional<Scope> visitScope(const Scope currentScope, std::vector<Node>::const_iterator beginOfScope, std::vector<Node>::const_iterator endOfScope);

    Scope visitGlobalScope(std::vector<Node> linesOfAst);
};
