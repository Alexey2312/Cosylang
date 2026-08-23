#include "parser.hpp"
#include <iostream>
#include <string>

namespace Cosylang::Parser
{

const Lexer::Token::Token* Parser::peek()
{
    if (cursor >= tokens.size())
    {
        return &tokens.back();
    }
    return &tokens[cursor];
}

const Lexer::Token::Token* Parser::consume(Lexer::Token::TokenType type)
{
    const Lexer::Token::Token* token = peek();
    if (token && token->type == type)
    {
        if (cursor < tokens.size())
        {
            cursor++;
        }
        return token;
    }
    else
    {
        return nullptr;
    }
}

const Lexer::Token::Token* Parser::consume()
{
    const Lexer::Token::Token* token = peek();
    if (cursor < tokens.size())
    {
        cursor++;
    }
    return token;
}

Node* Parser::parseExpression(int left_binding_power)
{
    const Lexer::Token::Token* token = consume();

    if (!token || token->type == Lexer::Token::TokenType::END_OF_FILE)
    {
        return nullptr;
    }

    Node* lhs = nullptr;

    switch (token->type)
    {
        case Lexer::Token::TokenType::INTEGER:
        case Lexer::Token::TokenType::FLOAT:
        case Lexer::Token::TokenType::STRING:
        {
            lhs = arena.alloc<Node>(NodeType::LITERAL, token);
            break;
        }
        case Lexer::Token::TokenType::ID:
        {
            if (peek()-> type == Lexer::Token::TokenType::LEFT_BRACKET)
            {
                consume();

                Node* first_arg = nullptr;
                Node* last_arg = nullptr;

                if (peek()->type != Lexer::Token::TokenType::RIGHT_BRACKET)
                {
                    while (peek()->type != Lexer::Token::TokenType::RIGHT_BRACKET)
                    {
                        Node* arg = parseExpression(0);

                        if (!first_arg)
                        {
                            first_arg = arg;
                        }
                        else
                        {
                            last_arg->next_sibling = arg;
                        }
                        last_arg = arg;

                        if (peek()->type == Lexer::Token::TokenType::COMMA)
                        {
                            consume();
                            continue;
                        }

                        if (peek()->type != Lexer::Token::TokenType::RIGHT_BRACKET)
                        {
                            std::cerr << "Expected ',' or ')' in argument list. \n";
                        }
                    }
                }
                consume(Lexer::Token::TokenType::RIGHT_BRACKET);

                lhs = arena.alloc<Node>(NodeType::FUNCTION_CALL, token, first_arg, nullptr);
            }
            else if (peek()-> type == Lexer::Token::TokenType::LEFT_SQUARE_BRACKET)
            {
                lhs = arena.alloc<Node>(NodeType::ID, token);
                consume();
                lhs->next_sibling = parseExpression(0);
                lhs = arena.alloc<Node>(NodeType::INDEX, token, lhs);
                if (peek()-> type != Lexer::Token::TokenType::RIGHT_SQUARE_BRACKET)
                {
                    std::cerr << "Expected ']' after array index, but got: " << peek()->name << " \n";
                }
                consume(Lexer::Token::TokenType::RIGHT_SQUARE_BRACKET);
                break;
            }
            else
            {
                lhs = arena.alloc<Node>(NodeType::ID, token);
            }
            break;
        }
        case Lexer::Token::TokenType::LEFT_BRACKET:
        {
            lhs = parseExpression(0);
            if (peek()->type != Lexer::Token::TokenType::RIGHT_BRACKET)
            {
                std::cerr << "Expected ')' after expression \n";
            }
            consume(Lexer::Token::TokenType::RIGHT_BRACKET);
            break;
        }
        case Lexer::Token::TokenType::MINUS:
        {
            constexpr int UNARY_BINDING_POWER = 40;
            Node* operand = parseExpression(UNARY_BINDING_POWER);
            lhs = arena.alloc<Node>(NodeType::UNARY_OP, token, operand, nullptr);
            break;
        }
        default:
            std::cerr << "Unexpected token: " + std::string(peek()->name) + "\n";
    }

    for (;;)
    {
        const Lexer::Token::Token* next_token = peek();
        if (!next_token || next_token->type == Lexer::Token::TokenType::END_OF_FILE)
        {
            break;
        }

        if (next_token->type == Lexer::Token::TokenType::RIGHT_BRACKET ||
            next_token->type == Lexer::Token::TokenType::SEMICOLON ||
            next_token->type == Lexer::Token::TokenType::COMMA)
        {
            break;
        }

        if (next_token->type == Lexer::Token::TokenType::ASSIGN)
        {
            consume();

            Node* rhs = parseExpression(0);
            lhs->next_sibling = rhs;
            lhs = arena.alloc<Node>(NodeType::ASSIGN, next_token, lhs);
            break;
        }

        int right_binding_power = getInfixBindingPower(next_token->type);

        if (right_binding_power < left_binding_power)
        {
            break;
        }

        const Lexer::Token::Token* operation_token = consume();
        Node* rhs = parseExpression(right_binding_power);
        lhs->next_sibling = rhs;

        lhs = arena.alloc<Node>(NodeType::BINARY_OP, operation_token, lhs);
    }

    return lhs;
}

inline int Parser::getInfixBindingPower(Lexer::Token::TokenType type)
{
    switch (type)
    {
        case Lexer::Token::TokenType::EQUALS: return 10;
        case Lexer::Token::TokenType::PLUS:
        case Lexer::Token::TokenType::MINUS: return 20;
        case Lexer::Token::TokenType::MUL:
        case Lexer::Token::TokenType::DIV: return 30;
        default:
            return -1;
    }
}

} // namespace Cosylang::Parser
