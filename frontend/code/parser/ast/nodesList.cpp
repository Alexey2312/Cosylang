#include "../../../head/node.hpp"
#include "../../../head/ast/nodesList.hpp"
#include <map>

std::map<TokenType, int> precedence = {
    {TokenType::PLUS, 1},
    {TokenType::MINUS, 1},
    {TokenType::MUL, 2},
    {TokenType::DIV, 2},
    {TokenType::PRINT, 4},
    {TokenType::IF, 4},
};

std::map<TokenType, Node> nodes
{
    {TokenType::PLUS, Node(TokenType::PLUS, "+", precedence[TokenType::PLUS], {nullptr}, nullptr, OperationType::BINARY)},
    {TokenType::MINUS, Node(TokenType::MINUS, "-", precedence[TokenType::MINUS], {nullptr}, nullptr, OperationType::BINARY)},
    {TokenType::MUL, Node(TokenType::MUL, "*", precedence[TokenType::MUL], {nullptr}, nullptr, OperationType::BINARY)},
    {TokenType::DIV, Node(TokenType::DIV, "/", precedence[TokenType::DIV], {nullptr}, nullptr, OperationType::BINARY)},
    {TokenType::PRINT, Node(TokenType::PRINT, "print", precedence[TokenType::PRINT], {nullptr}, nullptr, OperationType::KEYWORD)},
    {TokenType::IF, Node(TokenType::IF, "if", precedence[TokenType::IF], {nullptr}, nullptr, OperationType::KEYWORD)},
};

Node createIrregularNode(token inputToken)
{
    switch (inputToken.getType())
    {

    case TokenType::ID:
    {
        return Node(TokenType::ID, inputToken.getValue(), 0, {nullptr}, nullptr, OperationType::NOT_AN_OPERATION);
    }

    case TokenType::ANY_NUMBER:
    {
        return Node(TokenType::ANY_NUMBER, inputToken.getValue(), 0, {nullptr}, nullptr, OperationType::NOT_AN_OPERATION);
    }
    case TokenType::STRING:
    {
        return Node(TokenType::STRING, inputToken.getValue(), 0, {nullptr}, nullptr, OperationType::NOT_AN_OPERATION);
    }

    default:
    {
        throw std::runtime_error("createIrregularToken: Invalid token type");
    }

    }
}

Node createNode(token inputToken)
{
    if (nodes.count(inputToken.getType()) == 1)
    {
        return createNode(inputToken.getType());
    }
    else
    {
        return createIrregularNode(inputToken);
    }

}
