#include "../../head/semantic/checkers.hpp"
#include "../../head/semantic/symbol.hpp"
#include <string>
#include <vector>

class ValidFirstNode
{
    Node selfNode;
public:
    ValidFirstNode(Node node) : selfNode(node) {}
    Node getSelfNode()
    {
        return selfNode;
    }
    void setSelfNode(Node node)
    {
        selfNode = node;
    }
};

class ValidVarKeyword : ValidFirstNode
{
    std::vector<Node> children;
public:
    ValidVarKeyword(Node node) : ValidFirstNode(node)
    {
        if(node.getType() != TokenType::VAR)
        {
            throw std::runtime_error("Semantic error: ValidVarKeyword: Invalid variable declaration");
        }
        setSelfNode(node);
        children = node.getChildren();
    }

    IType defineVariableType(Node mainNode)
    {
        Node currentNode = mainNode;
        std::vector<Node> currentNodeChildren = currentNode.getChildren();
        IType type = AnyType();
        if (mainNode.getType() == TokenType::COLON)
        {
            if (currentNodeChildren[1].getType() == TokenType::TYPE_NAME)
            {
                std::string typeName = currentNodeChildren[1].getValue();
                return TypeFactory::createType(typeName);
            }
            else
            {
                throw std::runtime_error("Semantic error: ValidVarKeyword: Invalid type declaration");
            }
        }
        else if (mainNode.getType() == TokenType::TYPE_NAME)
        {
            std::string typeName = mainNode.getValue();
            return TypeFactory::createType(typeName);
        }
        else
        {
            throw std::runtime_error("Semantic error: ValidVarKeyword: Invalid type declaration");
        }
    }

    VariableSymbol processVarDeclaration(Node mainNode)
    {
        Node currentNode = mainNode;
        std::string variableName;
        IType variableType = TypeFactory::createType("any");
        std::vector<Node> currentNodeChildren = currentNode.getChildren();
        if(currentNodeChildren.empty() || currentNodeChildren.at(0).getType() != TokenType::EQUALS || currentNodeChildren.at(0).getType() != TokenType::ID || currentNodeChildren.at(0).getType() != TokenType::COLON)
        {
            throw std::runtime_error("Semantic error: processVarDeclaration: Invalid variable declaration");
        }
        if (currentNodeChildren.at(0).getType() == TokenType::EQUALS)
        {
            return processVarDeclaration(currentNode);
        }
        if(currentNodeChildren.at(0).getType() == TokenType::ID)
        {
            variableName = currentNodeChildren.at(0).getValue();
        }
        else
        {
            variableType = defineVariableType(currentNode);
        }

        return VariableSymbol(variableName, variableType);
    }

    VariableSymbol workWithNode()
    {
        Node currentNode = getSelfNode();
        std::vector<Node> currentNodeChildren = currentNode.getChildren();
        std::string variableName;
        if (currentNodeChildren[0].getType() == TokenType::VAR)
        {
            return processVarDeclaration(currentNode);
        }
        else
        {
            throw std::runtime_error("Semantic error: ValidVarKeyword: Invalid variable declaration");
        }
    }
};

class VariableDeclarationChecker : public Checker
{
    std::vector<TokenType> firstNodeTypes = {TokenType::EQUALS, TokenType::ID, TokenType::VAR, TokenType::CONST};
public:
    void isValid(Node node, std::string errorMessage = "Semantic error: Invalid variable declaration")
    {
        bool isValid = false;
        for(TokenType validFirst : firstNodeTypes)
        {
            if(node.getType() == validFirst)
            {

                isValid = true;
                return;
            }
        }
        if(!isValid)
        {
            throw std::runtime_error(errorMessage);
        }
    }

    void check(const Node node)
    {
        isValid(node);


    }
};
