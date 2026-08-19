#pragma once
#include "../../lexer/token/token.hpp"

namespace Cosylang::Parser
{

enum class NodeType
{
    ID,
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

    CODE_BLOCK, // { }
    ARRAY_INDEX, // [ ]
    PARENTHESIS, // ( )

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
};

struct Node
{
    NodeType type;
    const Lexer::Token::Token* token;

    Node* first_child = nullptr;
    Node* next_sibling = nullptr;

    Node(NodeType this_type, const Lexer::Token::Token* this_token, Node* this_first_child = nullptr, Node* this_next_sibling = nullptr)
        : type(this_type), token(this_token), first_child(this_first_child), next_sibling(this_next_sibling) {}
};


}; // namespace Cosylang::Parser
