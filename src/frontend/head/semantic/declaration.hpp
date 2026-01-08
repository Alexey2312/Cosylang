#pragma once

#include <string>
#include "scope.hpp"

class Declaration
{
    std::string name;
    Scope location;
public:
    Declaration(const std::string& name, Scope& scope) : name(name), location(scope) {}
};
