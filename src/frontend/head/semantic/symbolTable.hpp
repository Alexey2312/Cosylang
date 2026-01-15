#pragma once

#include "symbol.hpp"
#include <optional>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

class SymbolTable
{
    std::unordered_map<std::string, Symbol> symbols;

public:

    void addSymbolToSymbolsTable(std::string inputSymbolName, Symbol& inputSymbol)
    {
        if (symbols.find(inputSymbolName) != symbols.end())
        {
            throw std::runtime_error(inputSymbolName + " already in table!");
        }

        symbols.insert({inputSymbolName, inputSymbol});
    }

    void deleteSymbolFromTableByName(std::string nameOfDeletingSymbol)
    {
        if (symbols.find(nameOfDeletingSymbol) == symbols.end())
        {
            throw std::runtime_error(nameOfDeletingSymbol + ": deleting a non-existent symbol");
        }

        symbols.erase(nameOfDeletingSymbol);
    }

    bool isSymbolWithThisNameInSymbolTable(std::string nameOfSymbol)
    {
        return symbols.find(std::move(nameOfSymbol)) != symbols.end();
    }

    std::optional<Symbol> getSymbolFromSymbolTableByName(std::string name)
    {
        if (symbols.find(name) != symbols.end())
        {
            return symbols.at(std::move(name));
        }
        return std::nullopt;
    }

    std::vector<Symbol> getAllSymbolsFromTable()
    {
        std::vector<Symbol> outVector;
        outVector.reserve(symbols.size());

        for(const auto& [key, value] : symbols)
        {
            outVector.push_back(value);
        }
        return outVector;
    }

    SymbolTable() {}
};
