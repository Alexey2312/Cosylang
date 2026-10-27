#pragma once

#include "../parser/node/node.hpp"
#include "type.hpp"

#include <string>
#include <variant>
#include <vector>

namespace Cosylang::Semantic
{

enum class SymbolKind
{
    VARIABLE,
    PARAM,
    FUNCTION,
    STRUCT,
    FIELD,
    NAMESPACE,
    MODULE,
    TYPE_ALIAS
};

enum class Visibility
{
    PRIVATE,
    PUBLIC
};

struct Kind
{
    SymbolKind type_name;
};

struct Symbol;
struct Scope;

struct VariableKind
{
    SymbolKind type_name = SymbolKind::VARIABLE;
    Type type = Type("void", TypeKind::VOID);
};

struct ParameterKind
{
    SymbolKind type_name = SymbolKind::PARAM;
    Type type = Type("void", TypeKind::VOID);
};

struct FunctionKind
{
    SymbolKind type_name = SymbolKind::FUNCTION;
    Scope* scope = nullptr;
    Type type = Type("function", TypeKind::FUNCTION);
    Type returning_type = Type("void", TypeKind::VOID);
    std::vector<Symbol*> overloads = {};
};

struct StructKind
{
    SymbolKind type_name = SymbolKind::PARAM;
    Type type = Type("struct", TypeKind::STRUCT);
};

struct Symbol
{
    std::string name;
    std::variant<Kind, VariableKind, ParameterKind, FunctionKind, StructKind> kind;
    Visibility visibility = Visibility::PRIVATE;
    Parser::Node* node = nullptr;
    bool is_export = false;

    Symbol() = default;

    Symbol(Symbol&& other) noexcept
        : name(std::move(other.name)),
        kind(std::move(other.kind)),
        visibility(other.visibility),
        node(other.node),
        is_export(other.is_export) {other.node = nullptr;}

    Symbol& operator=(Symbol&& other) noexcept
    {
        if (this != &other)
        {
            name = std::move(other.name);
            kind = std::move(other.kind);
            visibility = other.visibility;
            is_export = other.is_export;
            node = other.node;
            other.node = nullptr;
        }
        return *this;
    }

    Symbol(const Symbol& other)
        : name(other.name),
        kind(other.kind),
        visibility(other.visibility),
        node(other.node),
        is_export(other.is_export) {}

    Symbol& operator=(const Symbol& other)
    {
        if (this != &other)
        {
            name = other.name;
            kind = other.kind;
            visibility = other.visibility;
            node = other.node;
            is_export = other.is_export;
        }
        return *this;
    }
};

} //namespace Cosylang::Semantic
