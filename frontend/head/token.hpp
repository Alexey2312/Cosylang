#pragma once
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

    LEFT_PAREN,
    RIGHT_PAREN,

    PLUS,
    MINUS,
    MUL,
    DIV,


    PRINT,
    WRITE,
    EQUAL,
    QUOTES_TOKEN,
    COLON,
    SEMICOLON,

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
