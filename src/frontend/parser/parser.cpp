#include "parser.hpp"
#include "node/node.hpp"
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
            return parseConstantDeclaration();
        }
        case Lexer::Token::TokenType::FUNC:
        {
            return parseFunctionDeclaration();
        }
        case Lexer::Token::TokenType::LEFT_CURLY_BRACKET:
        {
            return parseCodeBlock();
        }
        case Lexer::Token::TokenType::RETURN:
        {
            return parseReturn();
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

Node* Parser::parseConstantDeclaration()
{
    const Lexer::Token::Token* const_token = consume(Lexer::Token::TokenType::CONST);
    if (!const_token)
    {
        std::cerr << "Expected 'const' keyword in constant declaration\n";
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

Node* Parser::parseFunctionDeclaration()
{
    const Lexer::Token::Token* func_token = consume(Lexer::Token::TokenType::FUNC);
    if (!func_token)
    {
        return nullptr;
    }

    Node* func_node = arena.alloc<Node>(NodeType::FUNC, func_token);

    const Lexer::Token::Token* name_token = consume(Lexer::Token::TokenType::ID);
    if (!name_token)
    {
        std::cerr << "Expected function name\n";
        return nullptr;
    }
    Node* name_node = arena.alloc<Node>(NodeType::ID, name_token);
    func_node->first_child = name_node;

    Node* params_node = parseParams();
    name_node->next_sibling = params_node;

    Node* last_header_node = params_node;
    if (peek()->type == Lexer::Token::TokenType::COLON)
    {
        const Lexer::Token::Token* colon_token = consume();
        const Lexer::Token::Token* type_token = consume(Lexer::Token::TokenType::ID);

        Node* return_type = arena.alloc<Node>(NodeType::ID, type_token);
        Node* colon_node = arena.alloc<Node>(NodeType::COLON, colon_token, return_type);

        params_node->next_sibling = colon_node;
        last_header_node = colon_node;
    }

    Node* body_node = parseCodeBlock();
    last_header_node->next_sibling = body_node;

    return func_node;
}

Node* Parser::parseParams()
{
    if (peek()->type != Lexer::Token::TokenType::LEFT_BRACKET)
    {
        std::cerr << "Expected '(' in params list\n";
        return nullptr;
    }

    Node* params_node = arena.alloc<Node>(NodeType::PARAMS, consume(Lexer::Token::TokenType::LEFT_BRACKET));

    Node* first_child = nullptr;
    Node* last_child = nullptr;

    while (peek()->type != Lexer::Token::TokenType::RIGHT_BRACKET && peek()->type != Lexer::Token::TokenType::END_OF_FILE)
    {
        Node* param = parseParam();

        if (!first_child)
        {
            first_child = param;
            params_node->first_child = first_child;
        }
        else
        {
            last_child->next_sibling = param;
        }
        last_child = param;

        if (peek()->type == Lexer::Token::TokenType::COMMA)
        {
            consume();
        }
    }

    consume(Lexer::Token::TokenType::RIGHT_BRACKET, "Expected ')' at end of params list");
    return params_node;
}

Node* Parser::parseParam()
{
    const Lexer::Token::Token* param_name_token = consume(Lexer::Token::TokenType::ID);
    if (!param_name_token)
    {
        std::cerr << "Expected param name\n";
        return nullptr;
    }

    Node* param_name = arena.alloc<Node>(NodeType::ID, param_name_token);
    Node* out_node = param_name;

    if (peek()->type == Lexer::Token::TokenType::COLON)
    {
        const Lexer::Token::Token* colon_token = consume();
        const Lexer::Token::Token* type_token = consume(Lexer::Token::TokenType::ID);

        param_name->next_sibling = arena.alloc<Node>(NodeType::ID, type_token);
        out_node = arena.alloc<Node>(NodeType::COLON, colon_token, param_name);
    }

    if (peek()->type == Lexer::Token::TokenType::ASSIGN)
    {
        const Lexer::Token::Token* assign_token = consume();
        Node* default_value = parseExpression(0);

        out_node->next_sibling = default_value;
        out_node = arena.alloc<Node>(NodeType::ASSIGN, assign_token, out_node);
    }

    return out_node;
}

Node* Parser::parseReturn()
{
    const Lexer::Token::Token* return_token = consume(Lexer::Token::TokenType::RETURN);
    if (!return_token)
    {
        return nullptr;
    }

    Node* return_node = arena.alloc<Node>(NodeType::RETURN, return_token);

    const Lexer::Token::Token* next_token = peek();

    bool is_same_line = (next_token->line == return_token->line);

    bool is_valid_expr_start = (next_token->type != Lexer::Token::TokenType::RIGHT_CURLY_BRACKET) &&
                               (next_token->type != Lexer::Token::TokenType::SEMICOLON) &&
                               (next_token->type != Lexer::Token::TokenType::END_OF_FILE);

    if (is_same_line && is_valid_expr_start)
    {
        return_node->first_child = parseExpression(0);
    }

    return return_node;
}

Node* Parser::parseCodeBlock()
{
    if (peek()->type != Lexer::Token::TokenType::LEFT_CURLY_BRACKET)
    {
        std::cerr << "Expected '{' in code block \n";
        return nullptr;
    }
    Node* block = arena.alloc<Node>(NodeType::CODE_BLOCK, consume(Lexer::Token::TokenType::LEFT_CURLY_BRACKET));

    Node* first_child = nullptr;
    Node* last_child = nullptr;
    while (peek()->type != Lexer::Token::TokenType::RIGHT_CURLY_BRACKET)
    {
        Node* child = parseStatement();

        if (!first_child)
        {
            first_child = child;
            block->first_child = first_child;
        }
        else
        {
            last_child->next_sibling = child;
        }
        last_child = child;
    }
    consume(Lexer::Token::TokenType::RIGHT_CURLY_BRACKET, "Expected '}' at end of code block");
    return block;
}

} // namespace Cosylang::Parser
