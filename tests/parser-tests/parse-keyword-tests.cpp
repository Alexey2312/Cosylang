#include "../../src/frontend/parser/parser.hpp"
#include "../../src/frontend/lexer/lexer.hpp"
#include "../../external/silteli.hpp"

UNIT_TEST(ParseVariableDeclarationStatement)
{
    Cosylang::Lexer::Lexer lexer("var x = 42");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(128);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();
    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::VAR &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::VAR &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ASSIGN &&
                    node->first_child->first_child &&
                    node->first_child->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->token->name == "x" &&
                    node->first_child->first_child->next_sibling &&
                    node->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->first_child->next_sibling->token->name == "42";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseTypedVariableDeclarationStatement)
{
    Cosylang::Lexer::Lexer lexer("var x: int = 42");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(128);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();

    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::VAR &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ASSIGN &&
                    node->first_child->first_child &&
                    node->first_child->first_child->type == Cosylang::Parser::NodeType::COLON &&
                    node->first_child->first_child->first_child &&
                    node->first_child->first_child->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->first_child->token->name == "x" &&
                    node->first_child->first_child->first_child->next_sibling &&
                    node->first_child->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->first_child->next_sibling->token->name == "int" &&
                    node->first_child->first_child->next_sibling &&
                    node->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->first_child->next_sibling->token->name == "42";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseConstanteDeclarationStatement)
{
    Cosylang::Lexer::Lexer lexer("const x = 42");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(128);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();
    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::CONST &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::CONST &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ASSIGN &&
                    node->first_child->first_child &&
                    node->first_child->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->token->name == "x" &&
                    node->first_child->first_child->next_sibling &&
                    node->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->first_child->next_sibling->token->name == "42";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseTypedConstanteDeclarationStatement)
{
    Cosylang::Lexer::Lexer lexer("const x: int = 42");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(128);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();

    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::CONST &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ASSIGN &&
                    node->first_child->first_child &&
                    node->first_child->first_child->type == Cosylang::Parser::NodeType::COLON &&
                    node->first_child->first_child->first_child &&
                    node->first_child->first_child->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->first_child->token->name == "x" &&
                    node->first_child->first_child->first_child->next_sibling &&
                    node->first_child->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->first_child->next_sibling->token->name == "int" &&
                    node->first_child->first_child->next_sibling &&
                    node->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::LITERAL &&
                    node->first_child->first_child->next_sibling->token->name == "42";

    Silteli::expect(is_valid);
}
