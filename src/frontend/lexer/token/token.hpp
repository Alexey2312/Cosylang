#pragma once

#include <string_view>

namespace Cosylang::Lexer::Token
{

enum class TokenType
{
    ID,
    NUMBER,
    INTEGER,
    FLOAT,
    STRING,

    TRUE, // true
    FALSE, // false

    DOT, // .
    COMMA,// ,
    COLON, // :
    SEMICOLON, // ;
    RANGE, // ..

    ASSIGN, // =
    PLUS_ASSIGN, // +=
    MINUS_ASSIGN, // -=
    MUL_ASSIGN, // *=
    DIV_ASSIGN, // /=

    MUL, // *
    DIV, // /
    PLUS, // +
    MINUS, // -

    EQUALS, // ==
    NOT_EQUALS, // !=
    GREATER_OR_EQUAL, // >=
    LESS_OR_EQUAL, // <=
    GREATER, // >
    LESS, // <

    LEFT_BRACKET, // (
    RIGHT_BRACKET, // )

    LEFT_CURLY_BRACKET, // {
    RIGHT_CURLY_BRACKET, // }

    LEFT_SQUARE_BRACKET, // [
    RIGHT_SQUARE_BRACKET, // ]

    AND, // && or 'and'
    OR, // || or 'or'
    NOT, // ! or 'not'

    BIT_AND, // &
    BIT_OR, // |
    BIT_XOR, // ^
    BIT_NOT, // ~

    IMPORT, // import
    NAMESPACE, // namespace

    VAR, // var
    CONST, // const

    IF, // if
    ELSE, // else
    ELIF, // elif

    MATCH, // match

    WHILE, // while
    FOR, // for
    CONTINUE, // continue
    BREAK, // break

    FUNC, // func
    RETURN, // return
    ARROW, // =>

    BODY, // keyword 'body'

    TYPE, // keyword 'type'

    END_OF_FILE
};

class Token
{
public:
    std::string_view name;
    TokenType type = TokenType::END_OF_FILE;

    size_t line = 0;
    size_t column = 0;
    size_t offset = 0;
    size_t length = name.length();

    Token(TokenType this_type, std::string_view this_name, size_t this_line, size_t this_column, size_t this_offset) :
        name(this_name), type(this_type), line(this_line), column(this_column), offset(this_offset), length(this_name.length()) {}
};

}; // namespace Cosylang::Lexer
