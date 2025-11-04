#include "../../head/parser.hpp"
#include "../../head/node.hpp"
#include "../../head/token.hpp"
#include "../../head/ast/nodesList.hpp"
#include <stdexcept>
#include <vector>
#include <iostream>

std::vector<token> tokens;
unsigned int position = 0;

token nextToken()
{
    if (position >= tokens.size())
    {
        return token(TokenType::EOL, "");
    }
    return tokens[position++];
}

token lastToken()
{
    if (position <= tokens.size())
    {
        return token(TokenType::EOL, "");
    }
    return tokens[position--];
}

token peekToken()
{
    return tokens[position];
}

void advance()
{
    position++;
}

void eat(TokenType expectedToken)
{
    if (peekToken().getType() == expectedToken)
    {
        advance();
    }
    else
    {
        throw std::runtime_error("Unexpected token: " + peekToken().getValue());
    }
}

bool match(TokenType expectedToken)
{
    if (peekToken().getType() == expectedToken)
    {
        advance();
        return true;
    }
    return false;
}


Node parseFactor(token inputToken)
{
    switch (inputToken.getType())
    {
        case TokenType::ANY_NUMBER:
        {
            eat(TokenType::ANY_NUMBER);
            return createNode(ANY_NUMBER);
        }

        case TokenType::ID:
        {
            eat(TokenType::ID);
            return createNode(ID);
        }

        case TokenType::LEFT_PAREN:
        {
            eat(TokenType::LEFT_PAREN);
            return createNode(LEFT_PAREN);
        }

        case TokenType::EOL:
        {
            eat(TokenType::EOL);
            return createNode(EOL);
        }

        default:
        {
            throw std::runtime_error("Unexpected token: " + inputToken.getValue());
        }

    }
}

Node parseTerm(token inputToken)
{
    int precedence = 3;

    Node left = parseFactor(inputToken);
    Node outNode = createNode(EOL);
    while (match(TokenType::MUL) || match(TokenType::DIV))
    {
        token operation = lastToken();
        Node right = parseFactor(peekToken());
        outNode = Node(operation.getType(), operation.getValue(), precedence, {&left, &right}, nullptr, OperationType::BINARY);
    }
    return outNode;
}

Node parseExpression(token inputToken)
{
    Node outNode = createNode(EOL);
    if (inputToken.getType() == TokenType::STRING)
    {
        return Node(TokenType::STRING, inputToken.getValue(), 0, {nullptr}, nullptr, OperationType::NOT_AN_OPERATION);
    }



    Node left = parseTerm(inputToken);
    while (match(TokenType::PLUS) || match(TokenType::MINUS))
    {
        token operation = lastToken();
        Node right = parseTerm(peekToken());
        outNode = Node(operation.getType(), operation.getValue(), 2, {&left, &right}, nullptr, OperationType::BINARY);
    }

    return outNode;
}

std::vector<Node*> Parser::parse(std::vector<token> inputTokens)
{
    tokens = inputTokens;
    std::cout << "Getting tokens..." << std::endl;
    std::vector<Node*> ast;

    return ast;
}
