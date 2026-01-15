#pragma once

#include <memory>
#include <optional>
#include <string>
#include <vector>

#include "symbol.hpp"
#include "symbolTable.hpp"
#include "scopeRange.hpp"


class Scope
{
    std::string name;
    SymbolTable symbolTable;
    std::weak_ptr<Scope> parent;
    std::vector<std::shared_ptr<Scope>> children;
    std::optional<ScopeRange> range;

public:
    Scope(std::string thisName, std::shared_ptr<Scope> thisParent, std::vector<std::shared_ptr<Scope>> thisChildren, std::optional<ScopeRange> thisRange = std::nullopt, SymbolTable thisSymbolTable = SymbolTable()) : name(thisName), parent(thisParent), symbolTable(thisSymbolTable), children(thisChildren), range(thisRange) {}

    Scope(const Scope&) = default;
    Scope(Scope&&) = default;
    Scope& operator=(const Scope&) = default;
    Scope& operator=(Scope&&) = default;

    std::vector<Symbol> getDeclaratedTypes();

    void addDeclaration(Symbol newSymbol);
    void addDeclaration(std::optional<Symbol> newSymbol);

    std::string getName() { return name; }
    SymbolTable getSymbolTable() { return symbolTable; }

    std::weak_ptr<Scope> getParent()
    {
        return parent;
    }
    void setParent(std::weak_ptr<Scope> newParent)
    {
        parent = newParent;
    }

    std::vector<std::shared_ptr<Scope>> getChildren()
    {
        return children;
    }
    void setChildren(std::vector<std::shared_ptr<Scope>> newChildren)
    {
        children = newChildren;
    }
    void addChild(std::shared_ptr<Scope> newChild)
    {
        children.push_back(newChild);
    }

    std::optional<ScopeRange> getRange() const
    {
        return range;
    }
    void setRange(std::optional<ScopeRange> newRange)
    {
        range = newRange;
    }

};
