#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "symbol.hpp"

class Scope
{
    std::string name;
    std::shared_ptr<Scope> parent;
    std::unordered_map<std::string, Symbol> symbols;
public:
    Scope();
};
