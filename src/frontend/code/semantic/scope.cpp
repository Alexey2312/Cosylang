#include "../../head/semantic/scope.hpp"
#include <vector>

std::vector<Symbol> Scope::getDeclaratedTypes()
{
    std::vector<Symbol> outVector = symbolTable.getAllSymbolsFromTable();

    while (parent.lock() != nullptr)
    {
        std::vector<Symbol> parentDeclaratedTypes = parent.lock()->getDeclaratedTypes();
        outVector.insert(outVector.begin(), std::make_move_iterator(parentDeclaratedTypes.begin()), std::make_move_iterator(parentDeclaratedTypes.end()));
    }

    return outVector;
}

void Scope::addDeclaration(Symbol newSymbol)
{
    symbolTable.addSymbolToSymbolsTable(newSymbol.getName(), newSymbol);
}

void Scope::addDeclaration(std::optional<Symbol> newSymbol)
{
    if (newSymbol.has_value())
    {
        symbolTable.addSymbolToSymbolsTable(newSymbol.value().getName(), newSymbol.value());
    }
}
