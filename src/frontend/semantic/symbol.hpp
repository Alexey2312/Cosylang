#pragma once

#include "../parser/node/node.hpp"

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

struct FunctionKind
{
    SymbolKind type_name = SymbolKind::FUNCTION;
    std::vector<Kind> params = {};
    std::vector<Symbol*> overloads = {};
};


struct Symbol
{
    std::string name;
    std::variant<Kind, FunctionKind> kind;
    Visibility visibility = Visibility::PRIVATE;
    Parser::Node* node = nullptr;

    Symbol() = default;

    Symbol(Symbol&& other) noexcept
        : name(std::move(other.name)),
          kind(std::move(other.kind)),
          visibility(other.visibility),
          node(other.node)
    {
        other.node = nullptr;
    }

    Symbol& operator=(Symbol&& other) noexcept
    {
        if (this != &other)
        {
            name = std::move(other.name);
            kind = std::move(other.kind);
            visibility = other.visibility;
            node = other.node;
            other.node = nullptr;
        }
        return *this;
    }

    Symbol(const Symbol& other)
        : name(other.name),
          kind(other.kind),
          visibility(other.visibility),
          node(other.node) {}

    Symbol& operator=(const Symbol& other)
    {
        if (this != &other)
        {
            name = other.name;
            kind = other.kind;
            visibility = other.visibility;
            node = other.node;
        }
        return *this;
    }
};

} //namespace Cosylang::Semantic
