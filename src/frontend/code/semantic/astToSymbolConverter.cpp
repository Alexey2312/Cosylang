#include "../../head/semantic/astToSymbolConverter.hpp"
#include "../../head/semantic/isTypeNode.hpp"
#include "../../head/semantic/defineTypeByName.hpp"
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>

bool AstToSymbolConverter::isTypeWithThisNameWasAlreadyDeclarated(std::string inputName, std::vector<Symbol> alreadyDeclratedTypes)
{
    for(Symbol& currentDeclaratedType : alreadyDeclratedTypes)
    {
        if (currentDeclaratedType.getVariableInfo() != std::nullopt)
        {
            return true;
        }
        else if (currentDeclaratedType.getFunctionInfo() != std::nullopt)
        {
            return true;
        }
    }
    return false;
}


// Define variable data
CosylangType AstToSymbolConverter::defineVariableDataType(std::shared_ptr<Node> currentNode, std::vector<Symbol> declaratedTypes)
{
    if (currentNode->getType() == TokenType::ASSIGN)
    {
        std::shared_ptr<Node> typeNode = currentNode->getChildren().at(0)->getChildren().at(1);
        if (IsTypeNodeChecker::isValidNameOfBaseType(typeNode->getValue()) || isTypeWithThisNameWasAlreadyDeclarated(typeNode->getValue(), declaratedTypes))
        {
            std::optional<CosylangType> outType = TypeDefiner::defineTypeByName(typeNode->getValue());
            if (outType.has_value())
            {
                return outType.value();
            }
            else
            {
                throw std::runtime_error("Unknow type: " + currentNode->getValue());
            }
        }
        else
        {
            throw std::runtime_error("Unknow type: " + currentNode->getValue());
        }
    }
    return VoidType();
}

std::string AstToSymbolConverter::defineVariableName(std::shared_ptr<Node> currentNode)
{
    if (currentNode->getType() == TokenType::ASSIGN)
    {
        return currentNode->getChildren().at(0)->getChildren().at(0)->getChildren().at(0)->getValue();
    }
    throw std::runtime_error("Unvalid currentNode in AstToSymbolConverter::defineVariableName!");
}

CosylangValue AstToSymbolConverter::defineVariableValue(std::shared_ptr<Node> currentNode, CosylangType typeOfVariable)
{
    if (currentNode->getChildren().at(0)->getType() == TokenType::ASSIGN)
    {
        return CosylangValue(currentNode->getChildren().at(0), typeOfVariable);
    }
    throw std::runtime_error("Unvalid currentNode in AstToSymbolConverter::defineVariableValue: " + currentNode->getValue());
}



std::optional<Symbol> AstToSymbolConverter::createVariableSymbolFromAST(Node& mainNode, std::vector<Symbol> declaratedTypes) //To refactoring!!!
{
    if (mainNode.getType() == TokenType::VAR)
    {
        std::shared_ptr<Node> currentNode = mainNode.getChildren().at(0);

        VariableInfo outVariable = VariableInfo(defineVariableName(currentNode), defineVariableDataType(currentNode, declaratedTypes), defineVariableValue(currentNode, defineVariableDataType(currentNode, declaratedTypes)));

        outVariable.setDataType(defineVariableDataType(currentNode, declaratedTypes));

    }
    return std::nullopt;
}

Symbol AstToSymbolConverter::createSymbolFromAST(Node& mainNode, std::vector<Symbol> declaratedTypes)
{

}
