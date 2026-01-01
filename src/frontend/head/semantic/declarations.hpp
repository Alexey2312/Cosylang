#pragma once
#include "../../head/node.hpp"

class Declaration // Base class for all declarations (Function declaration, variable declaration, etc)
{
    TokenType validType = TokenType::EOL;
public:
    Declaration() {}
    TokenType getValidType() const { return validType; }
    void setValidNode(TokenType type) { validType = type; }
    bool isValid(Node input) const { return input.getType() == validType; }
};

class VariableDeclaration : public Declaration
{
    TokenType thisType = TokenType::VAR;
public:
    VariableDeclaration()
    {
        setValidNode(thisType);
    }
};

class FunctionDeclaration : public Declaration
{
    TokenType thisType = TokenType::FUNC;
public:
    FunctionDeclaration()
    {
        setValidNode(thisType);
    }
};
