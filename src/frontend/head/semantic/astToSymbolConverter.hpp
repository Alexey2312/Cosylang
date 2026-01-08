#pragma once

#include <optional>
#include <string>
#include <vector>

#include "cosylangValue.hpp"
#include "symbol.hpp"
#include "../node.hpp"


class AstToSymbolConverter
{
    bool isTypeWithThisNameWasAlreadyDeclarated(std::string inputName, std::vector<Symbol> alreadyDeclratedTypes);

    CosylangType defineVariableDataType(std::shared_ptr<Node> currentNode, std::vector<Symbol> declaratedTypes);
    std::string defineVariableName(std::shared_ptr<Node> currentNode);
    CosylangValue defineVariableValue(std::shared_ptr<Node> currentNode, CosylangType typeOfVariable);



    std::optional<Symbol> createVariableSymbolFromAST(Node& mainNode, std::vector<Symbol> declaratedTypes); //To refactoring!

    std::optional<Symbol> createFunctionSymbolFromAST(Node& mainNode, std::vector<Symbol> declaratedTypes);

public:
    Symbol createSymbolFromAST(Node& mainNode, std::vector<Symbol> declaratedTypes);
};
