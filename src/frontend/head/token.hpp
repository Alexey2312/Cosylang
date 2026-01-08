#pragma once
#include <string>

enum class TokenType
{
    NUMBER,
    ID,
    STRING,
    VALUE,

    VAR,
    CONST,

    LEFT_PAREN,
    RIGHT_PAREN,

    LEFT_BRACE,
    RIGHT_BRACE,

    PLUS,
    MINUS,
    MUL,
    DIV,

    COLON,
    QUOTES_TOKEN,

    PRINT,
    IF,
    ASSIGN,

    FUNC,
    OVERRIDE,
    PUBLIC,
    PRIVATE,
    ARGUMENTS,
    RETURNING_TYPE_OPERATOR,


    EXPRESSION,

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
