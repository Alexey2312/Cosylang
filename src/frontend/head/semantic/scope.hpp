#pragma once

#include <memory>
#include <string>

#include "symbolTable.hpp"


class Scope
{
    std::string name;
    SymbolTable symbolTable;
    std::shared_ptr<Scope> parent;

public:
    Scope(std::string thisName, std::shared_ptr<Scope> thisParent, SymbolTable thisSymbolTable) : name(thisName), parent(thisParent), symbolTable(thisSymbolTable) {}

};
