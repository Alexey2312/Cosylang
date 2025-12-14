
#include "symbol.hpp"
#include <iostream>
#include <map>
#include <string>
#include <optional>

class SymbolTable
{
private:
    std::map<std::string, ISymbol> table;

public:
    void addSymbol(const std::string name, ISymbol symbol)
    {
        table.insert(std::make_pair(name, symbol));
    }
    void removeSymbol(const std::string& name)
    {
        try
        {
            table.erase(name);
        }
        catch (...)
        {
            std::cout << "Error removing symbol: " << name << std::endl;
        }
    }
    std::optional<ISymbol> getSymbol(const std::string& name)
    {
        try
        {
            return table.at(name);
        }
        catch (...)
        {
            return std::nullopt;
        }
    }
};
