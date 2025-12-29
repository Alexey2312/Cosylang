#pragma once

#include <any>
#include <string>
#include <vector>
#include "kinds.hpp"
#include "../../code/semantic/functionFlags.cpp"
#include "types.hpp"

class ISymbol
{
private:
    std::string name;
    Kind kind;
    ISymbol* owner = nullptr;

public:
    ISymbol(std::string name, Kind kind, ISymbol* owner) : name(name), kind(kind), owner(owner) {}
    ~ISymbol() { delete owner; }

    std::string getName() const { return name; }
    Kind getKind() const { return kind; }
    ISymbol* getOwner() const { return owner; }
};

class VariableSymbol : ISymbol
{
private:
    std::any value;
    IType type;
public:
    VariableSymbol(std::string name, std::any newValue = std::any(), IType newType = AnyType(), ISymbol* owner = nullptr) : ISymbol(name, VariableKind(), owner), value(newValue), type(newType) {}
    std::any getValue() {return value;}
    void setValue(std::any newValue) {value = newValue;}
};

class FunctionSymbol : ISymbol
{
    std::string name;
    std::vector<ISymbol> arguments;
    std::vector<IFlag> flags;

public:
    std::vector<ISymbol> getArguments() const { return arguments; }
    FunctionSymbol(std::string name = "", std::vector<ISymbol> arguments = {}, std::vector<IFlag> flags = {}, ISymbol* owner = nullptr) : ISymbol(name, FunctionKind(), owner), arguments(arguments), flags(flags) {}
};
