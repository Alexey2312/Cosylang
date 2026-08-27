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

UNIT_TEST(ParseFunctionDeclarationTest)
{
    Cosylang::Lexer::Lexer lexer("func name() { var x = 42 }");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(256);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();

    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::FUNC &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::FUNC &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::PARAMS &&
                    node->first_child->next_sibling->next_sibling &&
                    node->first_child->next_sibling->next_sibling->type == Cosylang::Parser::NodeType::CODE_BLOCK &&
                    node->first_child->next_sibling->next_sibling->first_child &&
                    node->first_child->next_sibling->next_sibling->first_child->type == Cosylang::Parser::NodeType::VAR;

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseFullFunctionDeclarationTest)
{
    Cosylang::Lexer::Lexer lexer("func name(a: int, b: int = 12) { var c = a + b }");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(512);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();

    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::FUNC &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::FUNC &&

                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->token->name == "name" &&

                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::PARAMS &&

                    node->first_child->next_sibling->first_child &&
                    node->first_child->next_sibling->first_child->type == Cosylang::Parser::NodeType::COLON &&
                    node->first_child->next_sibling->first_child->first_child->token->name == "a" &&
                    node->first_child->next_sibling->first_child->first_child->next_sibling->token->name == "int" &&

                    node->first_child->next_sibling->first_child->next_sibling &&
                    node->first_child->next_sibling->first_child->next_sibling->type == Cosylang::Parser::NodeType::ASSIGN &&
                    node->first_child->next_sibling->first_child->next_sibling->first_child->first_child->token->name == "b" &&
                    node->first_child->next_sibling->first_child->next_sibling->first_child->first_child->next_sibling->token->name == "int" &&
                    node->first_child->next_sibling->first_child->next_sibling->first_child->next_sibling->token->name == "12" &&

                    node->first_child->next_sibling->next_sibling &&
                    node->first_child->next_sibling->next_sibling->type == Cosylang::Parser::NodeType::CODE_BLOCK &&
                    node->first_child->next_sibling->next_sibling->first_child &&
                    node->first_child->next_sibling->next_sibling->first_child->type == Cosylang::Parser::NodeType::VAR &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->type == Cosylang::Parser::NodeType::ASSIGN &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->first_child &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->first_child->token->name == "c" &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->first_child->next_sibling &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::BINARY_OP &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->first_child->next_sibling->token->type == Cosylang::Lexer::Token::TokenType::PLUS &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->first_child->next_sibling->first_child->token->name == "a" &&
                    node->first_child->next_sibling->next_sibling->first_child->first_child->first_child->next_sibling->first_child->next_sibling->token->name == "b";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseReturnWithValueTest)
{
    Cosylang::Lexer::Lexer lexer("return a + b");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(256);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();
    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::RETURN &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::RETURN &&
                    node->first_child &&
                    node->first_child->type == Cosylang::Parser::NodeType::BINARY_OP &&
                    node->first_child->token->type == Cosylang::Lexer::Token::TokenType::PLUS &&
                    node->first_child->first_child &&
                    node->first_child->first_child->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->token->name == "a" &&
                    node->first_child->first_child->next_sibling &&
                    node->first_child->first_child->next_sibling->type == Cosylang::Parser::NodeType::ID &&
                    node->first_child->first_child->next_sibling->token->name == "b";

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseEmptyReturnTest)
{
    Cosylang::Lexer::Lexer lexer("return");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(128);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();

    bool is_valid = node &&
                    node->type == Cosylang::Parser::NodeType::RETURN &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::RETURN &&
                    node->first_child == nullptr;

    Silteli::expect(is_valid);
}

UNIT_TEST(ParseReturnNewlineTest)
{
    const char* code =
        "return\n"
        "x + y";

    Cosylang::Lexer::Lexer lexer(code);

    auto tokens = lexer.tokenize();

    Arena arena = Arena(256);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* return_node = parser.parseStatement();
    Cosylang::Parser::Node* expr_node = parser.parseStatement();

    bool is_valid = return_node &&
                    return_node->type == Cosylang::Parser::NodeType::RETURN &&
                    return_node->first_child == nullptr &&
                    expr_node &&
                    expr_node->type == Cosylang::Parser::NodeType::BINARY_OP &&
                    expr_node->token->type == Cosylang::Lexer::Token::TokenType::PLUS &&
                    expr_node->first_child->token->name == "x" &&
                    expr_node->first_child->next_sibling->token->name == "y";

    Silteli::expect(is_valid);
}


UNIT_TEST(ParseIfElifElseStatementTest)
{
    Cosylang::Lexer::Lexer lexer("if a { var x = 1 } elif b { var y = 2 } else { var z = 3 }");

    auto tokens = lexer.tokenize();

    Arena arena = Arena(512);

    Cosylang::Parser::Parser parser(tokens, arena);

    Cosylang::Parser::Node* node = parser.parseStatement();

    bool is_valid = node &&
                    node->token->type == Cosylang::Lexer::Token::TokenType::IF &&

                    node->first_child &&
                    node->first_child->token->name == "a" &&

                    node->first_child->next_sibling &&
                    node->first_child->next_sibling->type == Cosylang::Parser::NodeType::CODE_BLOCK &&
                    node->first_child->next_sibling->first_child->first_child->first_child->token->name == "x" &&

                    node->first_child->next_sibling->next_sibling &&
                    node->first_child->next_sibling->next_sibling->type == Cosylang::Parser::NodeType::ELIF &&
                    node->first_child->next_sibling->next_sibling->token->type == Cosylang::Lexer::Token::TokenType::ELIF &&

                    node->first_child->next_sibling->next_sibling->first_child &&
                    node->first_child->next_sibling->next_sibling->first_child->token->name == "b" &&

                    node->first_child->next_sibling->next_sibling->first_child->next_sibling &&
                    node->first_child->next_sibling->next_sibling->first_child->next_sibling->type == Cosylang::Parser::NodeType::CODE_BLOCK &&
                    node->first_child->next_sibling->next_sibling->first_child->next_sibling->first_child->first_child->first_child->token->name == "y" &&

                    node->first_child->next_sibling->next_sibling->next_sibling &&
                    node->first_child->next_sibling->next_sibling->next_sibling->type == Cosylang::Parser::NodeType::ELSE &&
                    node->first_child->next_sibling->next_sibling->next_sibling->token->type == Cosylang::Lexer::Token::TokenType::ELSE &&

                    node->first_child->next_sibling->next_sibling->next_sibling->first_child &&
                    node->first_child->next_sibling->next_sibling->next_sibling->first_child->type == Cosylang::Parser::NodeType::CODE_BLOCK &&
                    node->first_child->next_sibling->next_sibling->next_sibling->first_child->first_child->first_child->first_child->token->name == "z";

    Silteli::expect(is_valid);
}
