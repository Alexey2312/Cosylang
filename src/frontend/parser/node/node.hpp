#pragma once
#include "../../lexer/token/token.hpp"

namespace Cosylang::Parser
{

enum class NodeType
{
    ID,
    LITERAL,

    BINARY_OP,
    UNARY_OP,
    ASSIGN,
    FUNCTION_CALL,
    PARAMS,
    INDEX,
    DOT,
    COLON,
    RANGE,

    CODE_BLOCK,
    VAR,
    CONST,

    IF,
    ELIF,
    ELSE,
    WHILE,
    FOR,
    MATCH,

    DEFAULT,

    ARROW,

    RETURN,
    BREAK,
    CONTINUE,

    FUNC,
    TYPE,
    NAMESPACE,
    IMPORT,

    BODY,

    PROGRAM,
};

struct Node
{
    NodeType type;
    const Lexer::Token::Token* token;

    Node* first_child = nullptr;
    Node* next_sibling = nullptr;

    void addChild(Node* child)
    {
        if (first_child)
        {
            if (!first_child->next_sibling)
            {
                first_child->next_sibling = child;
                return;
            }
            Node* last_child = first_child->next_sibling;
            while (last_child->next_sibling)
            {
                last_child = last_child->next_sibling;
                continue;
            }
            last_child->next_sibling = child;
            return;
        }
        else
        {
            first_child = child;
            return;
        }
    }

    Node(NodeType this_type, const Lexer::Token::Token* this_token, Node* this_first_child = nullptr, Node* this_next_sibling = nullptr)
        : type(this_type), token(this_token), first_child(this_first_child), next_sibling(this_next_sibling) {}
};


}; // namespace Cosylang::Parser
