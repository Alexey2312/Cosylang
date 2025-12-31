#include "../../head/checkOperationsTypes.hpp"
#include "../../head/token.hpp"
#include <vector>
#include <iostream>

std::vector<TokenType> binaryOperators = {
    TokenType::PLUS,
    TokenType::MINUS,
    TokenType::MUL,
    TokenType::DIV,
};

std::vector<TokenType> unaryOperators = {
    TokenType::MINUS,
};

std::vector<TokenType> notOperators = {
    TokenType::ID,
    TokenType::ANY_NUMBER,
};

std::vector<TokenType> keywords = {
    TokenType::IF,
    TokenType::PRINT,
};

bool isBinaryOperator(TokenType token)
{
    for (auto& binOperation : binaryOperators)
    {
        if (token == binOperation)
        {
            return true;
        }
    }
    return false;
}

bool isUnaryOperator(TokenType token)
{
    for (auto& unaryOperation : unaryOperators)
    {
        if (token == unaryOperation)
        {
            return true;
        }
    }
    return false;
}

bool isNotOperator(TokenType token)
{
    for (auto& notOperation : notOperators)
    {
        if (token == notOperation)
        {
            return true;
        }
    }
    return false;
}

/*        TESTS         */

void testIsBinaryOperator()
{
    std::string testingObject = "isBinaryOperator";
    for(auto& token : binaryOperators)
    {
        if(isBinaryOperator(token))
        {
            std::cout << testingObject << ": test passed" << std::endl;
        }
        else
        {
            std::runtime_error(testingObject + ": test failed");
        }
    }
}

void testIsUnaryOperator()
{
    std::string testingObject = "isUnaryOperator";
    for(auto& token : unaryOperators)
    {
        if(isUnaryOperator(token))
        {
            std::cout << testingObject << ": test passed" << std::endl;
        }
        else
        {
            std::runtime_error(testingObject + ": test failed");
        }
    }
}

void testIsNotOperator()
{
    std::string testingObject = "isNotOperator";
    for(auto& token : notOperators)
    {
        if(isNotOperator(token))
        {
            std::cout << testingObject << ": test passed" << std::endl;
        }
        else
        {
            std::runtime_error(testingObject + ": test failed");
        }
    }
}
