#pragma once

#include <string>
#include "scope.hpp"

class Declaration
{
    std::string name;
    Scope scope;
public:
    Declaration(const std::string& name, Scope scope);
};
