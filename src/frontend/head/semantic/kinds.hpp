#pragma once

#include <string>

class Kind
{
    std::string name;
public:
    Kind(std::string name) : name(name) {}
    std::string getName() const { return name; }
};

class VariableKind : public Kind
{
public:
    VariableKind() : Kind("variable") {}
};

class FunctionKind : public Kind
{
public:
    FunctionKind() : Kind("function") {}
};
