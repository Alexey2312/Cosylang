#include "scope.hpp"
#include <iostream>
#include <variant>

namespace Cosylang::Semantic
{

void Scope::loadDeclaration(const std::string& name, Symbol* symbol)
{
    auto it = table.find(name);

    if (it == table.end())
    {
        table.emplace(std::move(name), symbol);
        return;
    }

    std::visit(
        [&](auto& kind)
        {
            using T = std::decay_t<decltype(kind)>;

            if constexpr (std::is_same_v<T, FunctionKind>)
            {
                if (std::holds_alternative<FunctionKind>(symbol->kind))
                {
                    kind.overloads.push_back(symbol);
                }
                else
                {
                    std::cerr << "Redefinition of '" << name << "'\n";
                }
            }
            else
            {
                std::cerr << "Redefinition of '" << name << "'\n";
            }
        },
        it->second->kind
    );
}

Symbol* Scope::lookup(const std::string& name)
{
    auto it = table.find(name);
    if (it != table.end())
    {
        return it->second;
    }

    if (parent)
    {
        return parent->lookup(name);
    }

    return nullptr;
}


} //namespace Cosylang::Semantic
