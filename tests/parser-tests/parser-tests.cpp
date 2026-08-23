#include "../../src/frontend/parser/parser.hpp"
#include "../../src/frontend/lexer/lexer.hpp"
#include "../../external/silteli.hpp"

UNIT_TEST(ParsePlusExpression)
{
    Cosylang::Lexer::Lexer lexer("2+2");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(128);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    Silteli::expect(node->type == Cosylang::Parser::NodeType::BINARY_OP &&
        node->first_child &&
        node->first_child->type == Cosylang::Parser::NodeType::LITERAL &&
        node->first_child->next_sibling &&
        node->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL);
}

UNIT_TEST(ParseOperatorPrecedence)
{
    Cosylang::Lexer::Lexer lexer("2 + 3 * 4");
    auto tokens = lexer.tokenize();
    Arena arena(256);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::BINARY_OP &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::PLUS &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->token->name == "2" &&
                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::BINARY_OP &&
                    node->first_child->next_sibling->token->type == Cosylang::Lexer::Token::TokenType::MUL &&
                    node->first_child->next_sibling->first_child &&
                    node->first_child->next_sibling->first_child->token->name == "3" &&
                    node->first_child->next_sibling->first_child->next_sibling &&
                    node->first_child->next_sibling->first_child->next_sibling->token->name == "4";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseParenthesesPrecedence)
{
    Cosylang::Lexer::Lexer lexer("(2 + 3) * 4");
    auto tokens = lexer.tokenize();
    Arena arena(256);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::BINARY_OP &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::MUL &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::BINARY_OP &&
                    node->first_child->token->type == Cosylang::Lexer::Token::TokenType::PLUS &&
                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->next_sibling->token->name == "4";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseUnaryExpression)
{
    Cosylang::Lexer::Lexer lexer("-42");
    auto tokens = lexer.tokenize();
    Arena arena(128);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::UNARY_OP &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::MINUS &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->token->name == "42";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseFunctionCallExpression)
{
    Cosylang::Lexer::Lexer lexer("function(12, 42)");
    auto tokens = lexer.tokenize();
    Arena arena(256);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::FUNCTION_CALL &&
                    node->token->name == "function" &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL;

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseEmptyFunctionCallExpression)
{
    Cosylang::Lexer::Lexer lexer("foo()");
    auto tokens = lexer.tokenize();
    Arena arena(128);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::FUNCTION_CALL &&
                    node->token->name == "foo" &&
                    node->first_child == nullptr;

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseAssignmentExpression)
{
    Cosylang::Lexer::Lexer lexer("x = 100");
    auto tokens = lexer.tokenize();
    Arena arena(128);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::ASSIGN &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::ASSIGN &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->token->name == "x" &&
                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->next_sibling->token->name == "100";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseIndexExpression)
{
    Cosylang::Lexer::Lexer lexer("arr[1]");
    auto tokens = lexer.tokenize();
    Arena arena(128);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::INDEX &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->token->name == "arr" &&
                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->next_sibling->token->name == "1";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParsePrimaryLiterals)
{
    Cosylang::Lexer::Lexer lexer("'hello world'");
    auto tokens = lexer.tokenize();
    Arena arena(64);
    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseExpression(0);

    bool is_valid = node && node->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::STRING &&
                    node->token->name == "hello world";

    Silteli::expect(is_valid);
}
