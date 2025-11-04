#pragma once
#include <string>

enum TokenType
{
    TYPE_ID,
    INT32,
    STRING,

    ANY_NUMBER,
    ID,

    TYPE_NAME,
    VALUE,

    VAR,

    LEFT_PAREN,
    RIGHT_PAREN,

    PLUS,
    MINUS,
    MUL,
    DIV,

    COLON,
    QUOTES_TOKEN,

    PRINT,
    IF,
    EQUALS,

    EOL,
};

struct token
{
private:
    TokenType type;
    std::string value;
public:

    TokenType getType() {return type;};

    std::string getValue() {return value;};

    token(TokenType tokenType, std::string tokenValue) : type(tokenType), value(tokenValue) {};
};
