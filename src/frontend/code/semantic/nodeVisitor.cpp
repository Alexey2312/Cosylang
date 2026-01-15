#include "../../head/semantic/nodeVisitor.hpp"
#include <memory>
#include <optional>
#include <stdexcept>
#include <vector>

std::optional<Symbol> NodeVisitor::visitNode(Node visitingNode, std::vector<Symbol> declaratedSymbols)
{
    AstToSymbolConverter symbolCrator = AstToSymbolConverter();
    return symbolCrator.createSymbolFromAST(visitingNode, declaratedSymbols);
}

std::optional<ScopeRange> NodeVisitor::findScopeRange(std::vector<Node>::const_iterator startFrom, std::vector<Node>::const_iterator endOfAst)
{
    unsigned int levelOfNesting = 0;
    std::vector<Node>::const_iterator rangeBegin = startFrom;

    for(std::vector<Node>::const_iterator& iterator = startFrom; iterator != endOfAst; iterator++)
    {
        if (iterator->getType() == TokenType::LEFT_BRACE)
        {
            levelOfNesting++;
            if (levelOfNesting == 0)
            {
                rangeBegin = iterator++;
                continue;
            }
        }
        else if (iterator->getType() == TokenType::LEFT_BRACE)
        {
            levelOfNesting--;
            if (levelOfNesting == 0)
            {
                return ScopeRange(rangeBegin, iterator--);
            }
        }
    }

    return std::nullopt;
}


std::optional<Scope> NodeVisitor::visitScope(Scope currentScope, std::vector<Node>::const_iterator beginOfScope, std::vector<Node>::const_iterator endOfScope)
{
    if (beginOfScope->getType() != TokenType::LEFT_BRACE)
    {
        return std::nullopt;
    }
    beginOfScope++;

    for(std::vector<Node>::const_iterator iteratorOfCurrentAstLine = beginOfScope; iteratorOfCurrentAstLine != endOfScope; iteratorOfCurrentAstLine++)
    {
        if (auto declaration = visitNode(*iteratorOfCurrentAstLine, currentScope.getDeclaratedTypes()))
        {
            currentScope.addDeclaration(declaration);
        }
        else if (iteratorOfCurrentAstLine->getType() == TokenType::LEFT_BRACE || iteratorOfCurrentAstLine->getChildren().at((iteratorOfCurrentAstLine->getChildren().size() - 1))->getType() == TokenType::LEFT_BRACE)
        {
            if (std::optional<Scope> nestingScope = visitScope(Scope("", std::make_shared<Scope>(currentScope), {}), iteratorOfCurrentAstLine, endOfScope))
            {
                currentScope.addChild(std::make_shared<Scope>(nestingScope.value()));
            }
        }
    }

    if (endOfScope->getType() != TokenType::RIGHT_BRACE)
    {
        throw std::runtime_error("Expected '}' in scope, last line of AST: " + endOfScope->getValue());
    }

    return currentScope;
}

Scope NodeVisitor::visitGlobalScope(std::vector<Node> linesOfAst)
{
    Scope globalScope = Scope("global", nullptr, {});

    std::vector<Node>::const_iterator currentIterator = linesOfAst.begin();
    while (currentIterator != linesOfAst.end())
    {
        if (currentIterator->getType() == TokenType::LEFT_BRACE || currentIterator->getChildren().at((currentIterator->getChildren().size() - 1))->getType() == TokenType::LEFT_BRACE)
        {
            const std::optional<Scope> globalChildScope = visitScope(Scope("", nullptr, {}), currentIterator, linesOfAst.end());
            if (globalChildScope.has_value())
            {
                globalScope.addChild(std::make_shared<Scope>(globalChildScope.value()));
                currentIterator = (globalChildScope.value().getRange().value().getEnd())++;
            }
        }
    }

    return globalScope;
}
