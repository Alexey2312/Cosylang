#pragma once

#include "symbol.hpp"
#include <string>
#include <unordered_map>

namespace Cosylang::Semantic
{

struct Module;

struct Scope
{
    std::unordered_map<std::string, Symbol*> table;
    Scope* parent;
    Cosylang::Semantic::Module* owner = nullptr;

    void loadDeclaration(const std::string& name, Symbol* symbol);
    Symbol* lookup(const std::string& name);

    Scope(Scope* this_parent = nullptr, Cosylang::Semantic::Module* this_owner = nullptr) : parent(this_parent), owner(this_owner) {}
};

} //namespace Cosylang::Semantic
