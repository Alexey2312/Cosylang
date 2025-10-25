#include "../head/token.hpp"



TokenType type;
std::vector<char> value;
TokenType token::getType()
{
    return type;
};

const std::vector<char> token::getValue() const
{
    return value;
};

token::token(TokenType tokenType, std::vector<char> tokenValue) : type(tokenType), value(std::move(tokenValue)) {}
