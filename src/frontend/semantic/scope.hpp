#pragma once

#include "symbol.hpp"
#include <string>
#include <unordered_map>

namespace Cosylang::Semantic
{

struct Scope
{
    std::unordered_map<std::string, Symbol*> table;
    Scope* parent;

    void loadDeclaration(const std::string& name, Symbol* symbol);
    Symbol* lookup(const std::string& name);

    Scope(Scope* parent = nullptr) : parent(parent) {}
};

} //namespace Cosylang::Semantic
