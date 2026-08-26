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

const Lexer::Token::Token* Parser::previousToken()
{
    if (cursor >= tokens.size() && cursor - 1 > 0)
    {
        return &tokens[cursor - 1];
    }
    return &tokens[0];
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

const Lexer::Token::Token* Parser::consume(Lexer::Token::TokenType type, std::string error)
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
        std::cerr << error << "\n";
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

Node* Parser::parseStatement()
{
    const Lexer::Token::Token* token = peek();
    switch (token->type)
    {
        case Lexer::Token::TokenType::VAR:
        {
            return parseVariableDeclaration();
        }
        case Lexer::Token::TokenType::CONST:
        {
            return parseConstanteDeclaration();
        }
        default:
        {
            return parseExpression(0);
        }
    }
}

Node* Parser::parseVariableDeclaration()
{
    const Lexer::Token::Token* var_token = consume(Lexer::Token::TokenType::VAR);
    if (!var_token)
    {
        std::cerr << "Expected 'var' keyword in variable declaration\n";
        return nullptr;
    }

    Node* var_keyword = arena.alloc<Node>(NodeType::VAR, var_token);

    const Lexer::Token::Token* id_token = consume(Lexer::Token::TokenType::ID);
    if (!id_token)
    {
        std::cerr << "Expected variable name\n";
        return nullptr;
    }

    Node* variable_name = arena.alloc<Node>(NodeType::ID, id_token);
    Node* left_assign_operand = variable_name;

    if (peek()->type == Lexer::Token::TokenType::COLON)
    {
        const Lexer::Token::Token* colon_token = consume();

        const Lexer::Token::Token* type_token = consume(Lexer::Token::TokenType::ID);
        if (!type_token)
        {
            std::cerr << "Expected type after ':'\n";
            return nullptr;
        }

        variable_name->next_sibling = arena.alloc<Node>(NodeType::ID, type_token);
        Node* colon = arena.alloc<Node>(NodeType::COLON, colon_token, variable_name);
        left_assign_operand = colon;
    }

    if (peek()->type == Lexer::Token::TokenType::ASSIGN)
    {
        const Lexer::Token::Token* assign_token = consume();

        Node* rhs = parseExpression(0);
        left_assign_operand->next_sibling = rhs;
        Node* assign = arena.alloc<Node>(NodeType::ASSIGN, assign_token, left_assign_operand);

        var_keyword->first_child = assign;
        return var_keyword;
    }
    var_keyword->first_child = left_assign_operand;
    return var_keyword;
}

Node* Parser::parseConstanteDeclaration()
{
    const Lexer::Token::Token* const_token = consume(Lexer::Token::TokenType::CONST);
    if (!const_token)
    {
        std::cerr << "Expected 'const' keyword in variable declaration\n";
        return nullptr;
    }

    Node* const_keyword = arena.alloc<Node>(NodeType::CONST, const_token);

    const Lexer::Token::Token* id_token = consume(Lexer::Token::TokenType::ID);
    if (!id_token)
    {
        std::cerr << "Expected variable name\n";
        return nullptr;
    }

    Node* const_name = arena.alloc<Node>(NodeType::ID, id_token);
    Node* left_assign_operand = const_name;

    if (peek()->type == Lexer::Token::TokenType::COLON)
    {
        const Lexer::Token::Token* colon_token = consume();

        const Lexer::Token::Token* type_token = consume(Lexer::Token::TokenType::ID);
        if (!type_token)
        {
            std::cerr << "Expected type after ':'\n";
            return nullptr;
        }

        const_name->next_sibling = arena.alloc<Node>(NodeType::ID, type_token);
        Node* colon = arena.alloc<Node>(NodeType::COLON, colon_token, const_name);
        left_assign_operand = colon;
    }

    if (peek()->type == Lexer::Token::TokenType::ASSIGN)
    {
        const Lexer::Token::Token* assign_token = consume();

        Node* rhs = parseExpression(0);
        left_assign_operand->next_sibling = rhs;
        Node* assign = arena.alloc<Node>(NodeType::ASSIGN, assign_token, left_assign_operand);

        const_keyword->first_child = assign;
        return const_keyword;
    }
    else
    {
        std::cerr << "Constant must be initialized \n";
        return nullptr;
    }
    const_keyword->first_child = left_assign_operand;
    return const_keyword;
}

} // namespace Cosylang::Parser
