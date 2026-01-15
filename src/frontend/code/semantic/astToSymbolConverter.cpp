#include "../../head/semantic/astToSymbolConverter.hpp"
#include "../../head/semantic/isTypeNode.hpp"
#include "../../head/semantic/defineTypeByName.hpp"
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <vector>

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

        std::string variableName = defineVariableName(currentNode);

        CosylangType variableDataType = defineVariableDataType(currentNode, declaratedTypes);

        CosylangValue variableValue = defineVariableValue(currentNode, variableDataType);

        VariableInfo outVariable = VariableInfo(variableName, variableDataType, variableValue);

        return Symbol(variableName, outVariable);

    }
    return std::nullopt;
}

std::optional<Symbol> AstToSymbolConverter::createFunctiomParameterFromAST(std::shared_ptr<Node> mainNode, std::vector<Symbol> declaratedTypes)
{
    if (mainNode->getType() == TokenType::ASSIGN || mainNode->getType() == TokenType::COLON)
    {


        std::string parameterName = defineVariableName(mainNode);
        CosylangType parameterType = defineVariableDataType(mainNode, declaratedTypes);
        std::optional<CosylangValue> parameterValue = defineVariableValue(mainNode, parameterType);

        return Symbol(parameterName, FunctionParameterInfo(parameterName, parameterType, parameterValue));
    }

    return std::nullopt;
}

CosylangType AstToSymbolConverter::defineFunctionReturningDataType(std::shared_ptr<Node> currentNode, std::vector<Symbol> declaratedTypes)
{
    if (currentNode->getType() == TokenType::COLON)
    {
        std::shared_ptr<Node> typeNode = currentNode->getChildren().at(1);

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
            throw std::runtime_error("Unknow type!");
        }
    }

    throw std::runtime_error("Unknow type!");
}

std::string defineFunctionName(std::shared_ptr<Node> currentNode)
{
    return currentNode->getChildren().at(0)->getValue();
}

std::optional<std::vector<Symbol>> AstToSymbolConverter::defineFunctionParameters(std::shared_ptr<Node> currentNode, std::vector<Symbol> declaratedTypes)
{
    if (currentNode->getType() == TokenType::ARGUMENTS)
    {
        std::vector<std::optional<Symbol>> parametersVector;
        for(auto& parameter : currentNode->getChildren())
        {
            parametersVector.push_back(createFunctiomParameterFromAST(parameter, declaratedTypes));
        }
    }
    throw std::runtime_error("Unvalid currentNode in AstToSymbolConverter::defineFunctionParameters");
}

std::optional<Symbol> AstToSymbolConverter::createFunctionSymbolFromAST(Node& mainNode, std::vector<Symbol> declaratedTypes)
{
    if (mainNode.getType() == TokenType::FUNC)
    {
        std::shared_ptr<Node> currentNode = mainNode.getChildren().at(0);

        CosylangType defineParameterDataType(std::shared_ptr<Node> currentNode, std::vector<Symbol> declaratedTypes);
        std::string defineParameterName(std::shared_ptr<Node> currentNode);
        std::optional<CosylangValue> defineParameterDefaultValue(std::shared_ptr<Node> currentNode, CosylangType typeOfParameter);
    }

    return std::nullopt;
}

std::optional<Symbol> AstToSymbolConverter::createSymbolFromAST(Node& mainNode, std::vector<Symbol> declaratedTypes)
{
    return createVariableSymbolFromAST(mainNode, declaratedTypes);
}
