#pragma once

#include <vector>
#include <string>

enum TokenType
{
    TYPE_ID,
    INT16,
    INT32,
    INT64,
    STRING_TYPE,

    ANY_NUMBER,

    LET_ID,

    TYPE_NAME,
    VALUE,


    VAR,
    FUNC,

    ARGUMENTS,

    LEFT_BRCKET,
    RIGHT_BRCKET,

    PRINT,
    WRITE,
    EQUALS,
    QUOTES_TOKEN,
    COLON,
    SEMICOLON,

    END_OF_LINE,
};

struct token
{
    TokenType type;
    std::vector<char> value;
public:

    TokenType getType()
    {
        return type;
    };

    std::vector<char> getValue()
    {
        return value;
    };

    token(TokenType tokenType, std::vector<char> tokenValue);

};
